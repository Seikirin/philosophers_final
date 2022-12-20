/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:12 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/20 11:53:35 by mcharrad         ###   ########.fr       */
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
# include <semaphore.h>
# include <signal.h>

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
	int				waiting;
	time_t			lastate;
	t_vars			*vars;
	t_shared		shared;
	pthread_t		id;
	sem_t			*deadlock;
	sem_t			*print;
	sem_t			*sem;

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
	t_philo			**philos;
	pid_t			*pids;
	int				over;
}	t_vars;

int		ft_atoi(const char *str);
void	ft_memcpy(void *dst, const void *src, size_t n);
time_t	timestamp(time_t start);
int		actualsleep(time_t num, time_t start, t_philo *philo);
int		checkdeath(t_philo *philo, int value, int all);
int		checklastate(t_philo *philo, int val);
int		checkate(t_philo *philo, int val);
int		printstate(t_philo *philo, int state);
int		takefork(t_philo *philo);
int		endall(t_vars *vars);
int		mainthread(t_vars *vars);
char	*ft_itoa(int n);
void	postandsleep(t_philo *philo);
sem_t	*getsem(const char *main, int number, int type, int value);
int		checkwaiting(t_philo *philo, int val);
int		processlife(t_shared shared, int number);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strlen(char *str);
void	endlife(t_philo *philo);

#endif