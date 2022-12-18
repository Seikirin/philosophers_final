/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:18:27 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/18 15:04:29 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checkdeath(t_philo *philo, int value, int all)
{
	int	ret;

	ret = 0;
	sem_wait(philo->deadlock);
	if (!value)
		ret = (timestamp(philo->shared.start) - philo->lastate
				>= philo->shared.die_t);
	else
		philo->dead = 1;
	sem_post(philo->deadlock);
	if (all && getsem("/philodied", 0, 0, 0) != SEM_FAILED)
		return (1);
	return (ret);
}

int	checklastate(t_philo *philo, int val)
{
	int	ret;

	ret = 0;
	sem_wait(philo->deadlock);
	ret = philo->lastate;
	if (val)
		philo->lastate = val;
	sem_post(philo->deadlock);
	return (ret);
}

int	checkate(t_philo *philo, int val)
{
	int	ret;

	sem_wait(philo->deadlock);
	ret = philo->ate;
	if (val > 0)
		philo->ate++;
	else if (val == -1)
		ret = (philo->shared.eat_m != 0 && philo->shared.eat_m <= philo->ate);
	sem_post(philo->deadlock);
	return (ret);
}

int	checkwaiting(t_philo *philo, int val)
{
	int	ret;

	sem_wait(philo->deadlock);
	ret = philo->waiting;
	if (val > 0)
		philo->waiting = !philo->waiting;
	sem_post(philo->deadlock);
	return (ret);
}
