/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:12 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 12:28:05 by mcharrad         ###   ########.fr       */
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

typedef struct s_vars	t_vars;

typedef struct s_shared {
	int		ph_n;
	int		die_t;
	int		eat_t;
	int		sleep_t;
	int		eat_m;
	time_t	start;
}	t_shared;

typedef struct s_philo
{
	int				number;
	int				dead;
	int				ate;
	int				safe;
	time_t			lastate;
	t_vars			*vars;
	t_shared		shared;
	pthread_t		id;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	pthread_mutex_t	deadlock;
}	t_philo;

enum {
	SLEEPING,
	THINKING,
	DIED,
	EATING,
	FORK
};

typedef struct s_vars
{
	t_shared		shared;
	pthread_mutex_t	*forks;
	t_philo			**philos;
	int				over;
}	t_vars;

int		ft_atoi(const char *str);
void	ft_memcpy(void *dst, const void *src, size_t n);
time_t	timestamp(time_t start);
int		actualsleep(time_t num, time_t start, t_philo *philo);
int		checkdeath(t_philo *philo, int value);
void	setforks(t_philo *philo, t_vars *vars);
int		checklastate(t_philo *philo, int val);
int		checkate(t_philo *philo, int val);
int		printstate(t_philo *philo, int state);
void	unlockandsleep(t_philo *philo);
int		takefork(t_philo *philo, pthread_mutex_t *fork);
void	*live(void *content);
int		endall(t_vars *vars);
int		mainthread(t_vars *vars);
void	createthreads(t_vars *vars);

#endif