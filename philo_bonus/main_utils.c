/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:13:13 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/20 11:55:41 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	timestamp(time_t start)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	if (!start)
		return ((tm.tv_sec * 1000) + (tm.tv_usec / 1000));
	return (((tm.tv_sec * 1000) + (tm.tv_usec / 1000)) - start);
}

int	actualsleep(time_t num, time_t start, t_philo *philo)
{
	time_t	end;

	end = timestamp(start) + num;
	while (timestamp(start) < end
		&& (philo == 0 || (!checkdeath(philo, 0, 1))))
		usleep(100);
	return (1);
}

void	*runsem_wait(void *content)
{
	t_philo	*philo;

	philo = content;
	sem_wait(philo->sem);
	checkwaiting(philo, 1);
	return (content);
}

int	takefork(t_philo *philo)
{
	philo->waiting = 1;
	pthread_create(&philo->id, 0, runsem_wait, philo);
	while (checkwaiting(philo, 0) && !checkdeath(philo, 0, 1))
		usleep(10);
	if (checkwaiting(philo, 0))
		return (0);
	if (!checkdeath(philo, 0, 1)
		&& printstate(philo, FORK))
		return (1);
	return (0);
}

int	printstate(t_philo *philo, int state)
{
	if (!checkdeath(philo, 0, 1))
	{
		sem_wait(philo->print);
		if (state == SLEEPING)
			printf("%zu %d is sleeping\n",
				timestamp(philo->shared.start), philo->number);
		if (state == EATING)
			printf("%zu %d is eating\n",
				timestamp(philo->shared.start), philo->number);
		if (state == DIED)
			printf("%zu %d died\n",
				timestamp(philo->shared.start), philo->number);
		if (state == THINKING)
			printf("%zu %d is thinking\n",
				timestamp(philo->shared.start), philo->number);
		if (state == FORK)
			printf("%zu %d has taken a fork\n",
				timestamp(philo->shared.start), philo->number);
		sem_post(philo->print);
		return (1);
	}
	return (0);
}
