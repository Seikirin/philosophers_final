/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:13:13 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/11 11:17:20 by mcharrad         ###   ########.fr       */
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
		&& (philo == 0 || (!checkdeath(philo, 0))))
		usleep(100);
	return (1);
}

void	setforks(t_philo *philo, t_vars *vars)
{
	philo->right = &(vars->forks[philo->number - 1]);
	if (philo->number >= vars->shared.ph_n)
		philo->left = &(vars->forks[0]);
	else
		philo->left = &(vars->forks[philo->number]);
}

// int	takefork(t_philo *philo, pthread_mutex_t *fork)
// {
// 	if (!pthread_mutex_lock(fork) && !checkdeath(philo, 0)
// 		&& printstate(philo, FORK))
// 		return (1);
// 	return (0);
// }

int	printstate(time_t start, int number, int state)
{
	if (state == SLEEPING)
		printf("%zu %d is sleeping\n",
			timestamp(start), number);
	if (state == EATING)
		printf("%zu %d is eating\n",
			timestamp(start), number);
	if (state == DIED)
		printf("%zu %d died\n",
			timestamp(start), number);
	if (state == THINKING)
		printf("%zu %d is thinking\n",
			timestamp(start), number);
	if (state == FORK)
		printf("%zu %d has taken a fork\n",
			timestamp(start), number);
	return (0);
}
