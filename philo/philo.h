/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:12 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 07:54:59 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

int	ft_atoi(const char *str);
void ft_memcpy(void *dst, const void *src, size_t n);

typedef struct s_vars t_vars;

typedef struct s_shared {
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	time_t start;
}	t_shared;

typedef struct s_philo
{
	int number;
	int dead;
	int ate;

	time_t lastate;
	t_vars *vars;
	pthread_t id;
	pthread_mutex_t *right;
	pthread_mutex_t *left;
	pthread_mutex_t deadlock;
	t_shared shared;
} t_philo;

typedef struct s_vars
{
	t_shared shared;
	pthread_mutex_t *forks;
	t_philo **philosophers;
	int over;
} t_vars;

#endif