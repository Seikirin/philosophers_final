/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:17:49 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/11 11:21:20 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	*live(void *content)
// {
// 	t_philo	*philo;

// 	philo = content;
// 	if (philo->number % 2 == 0)
// 		actualsleep(1, philo->shared.start, philo);
// 	while (!checkdeath(philo, 0))
// 	{
// 		if (takefork(philo, philo->right) && takefork(philo, philo->left))
// 		{
// 			printstate(philo, EATING);
// 			actualsleep(philo->shared.eat_t, philo->shared.start, philo);
// 			unlockandsleep(philo);
// 		}
// 	}
// 	pthread_mutex_unlock(philo->left);
// 	pthread_mutex_unlock(philo->right);
// 	return (content);
// }

// void	unlockandsleep(t_philo *philo)
// {
// 	if (checkdeath(philo, 0))
// 		return ;
// 	pthread_mutex_unlock(philo->left);
// 	pthread_mutex_unlock(philo->right);
// 	checklastate(philo, timestamp(philo->shared.start));
// 	checkate(philo, 1);
// 	printstate(philo, SLEEPING);
// 	actualsleep(philo->shared.sleep_t, philo->shared.start, philo);
// 	printstate(philo, THINKING);
// }
