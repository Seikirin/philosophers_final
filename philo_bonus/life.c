/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:17:49 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/18 15:17:07 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

sem_t	*getsem(const char *main, int number, int type, int value)
{
	char	*str;
	sem_t	*ret;

	ret = 0;
	if (number)
		str = ft_strjoin((char *)main, ft_itoa(number));
	else
		str = (char *)main;
	if (!type)
		ret = sem_open(str, O_RDWR);
	else if (type == 2)
		sem_unlink(str);
	else
		ret = sem_open(str, O_CREAT, 0777, value);
	if (number)
		free(str);
	return (ret);
}

int	processlife(t_shared shared, int number)
{
	t_philo	philo;

	memset(&philo, 0, sizeof(t_philo));
	philo.number = number;
	philo.shared = shared;
	philo.sem = getsem("/forks", 0, 0, 0);
	philo.deadlock = getsem("/data_", number, 0, 0);
	if (philo.number % 2 == 0)
		actualsleep(5, philo.shared.start, &philo);
	while (!checkdeath(&philo, 0, 1) && !checkate(&philo, -1))
	{
		if (takefork(&philo) && takefork(&philo))
		{
			printstate(&philo, EATING);
			actualsleep(philo.shared.eat_t, philo.shared.start, &philo);
			postandsleep(&philo);
		}
	}
	endlife(&philo);
	return (0);
}

void	postandsleep(t_philo *philo)
{
	if (checkdeath(philo, 0, 1))
		return ;
	sem_post(philo->sem);
	sem_post(philo->sem);
	checklastate(philo, timestamp(philo->shared.start));
	checkate(philo, 1);
	if (checkate(philo, -1))
		return ;
	printstate(philo, SLEEPING);
	actualsleep(philo->shared.sleep_t, philo->shared.start, philo);
	printstate(philo, THINKING);
}

void	endlife(t_philo *philo)
{
	if (checkdeath(philo, 0, 0) && getsem("/philodied", 0, 0, 0) == SEM_FAILED)
	{
		sem_wait(getsem("/philodied", 0, 1, 1));
		printf("%zu %d died\n",
			timestamp(philo->shared.start), philo->number);
	}
	else if (checkate(philo, -1))
	{
		getsem("/ateenough", philo->number, 1, 1);
		printf("%zu %d ate enough\n",
			timestamp(philo->shared.start), philo->number);
	}
	sem_post(philo->sem);
	sem_post(philo->sem);
}
