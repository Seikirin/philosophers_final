/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:18:27 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 12:20:57 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checkdeath(t_philo *philo, int value)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&philo->deadlock);
	if (!value)
		ret = philo->dead;
	else
		philo->dead = 1;
	pthread_mutex_unlock(&philo->deadlock);
	return (ret);
}

int	checklastate(t_philo *philo, int val)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&philo->deadlock);
	ret = philo->lastate;
	if (val)
		philo->lastate = val;
	pthread_mutex_unlock(&philo->deadlock);
	return (ret);
}

int	checkate(t_philo *philo, int val)
{
	int	ret;

	pthread_mutex_lock(&philo->deadlock);
	ret = philo->ate;
	if (val)
		philo->ate++;
	pthread_mutex_unlock(&philo->deadlock);
	return (ret);
}
