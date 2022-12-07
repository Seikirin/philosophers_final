/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:05 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 07:57:59 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t timestamp(void)
{
	static time_t start;
	struct timeval tm;

	gettimeofday(&tm, NULL);
	if (!start)
		start = ((tm.tv_sec * 1000) + (tm.tv_usec / 1000));
	return ((tm.tv_sec * 1000) + (tm.tv_usec / 1000)) - start;
}

void actualsleep(time_t num)
{
	time_t end;

	end = timestamp() + num;
	while (timestamp() < end)
		usleep(100);
}

void setforks(t_philo *philo, t_vars *vars)
{
	philo->right = &(vars->forks[philo->number - 1]);
	if (philo->number >= vars->number_of_philosophers)
		philo->left = &(vars->forks[0]);
	else
		philo->left = &(vars->forks[philo->number]);
}

void unlockandsleep(t_philo *philo)
{
	if (philo->dead)
		return ;
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	philo->lastate = timestamp();
	philo->ate++;
	printf("%zu %d is sleeping\n", timestamp(), philo->number);
	actualsleep(philo->time_to_sleep);
	printf("%zu %d is thinking\n", timestamp(), philo->number);
}

void *live(void *content)
{
	t_philo *philo;

	philo = content;
	if (philo->number % 2 == 0)
		actualsleep(1);
	while (!philo->dead)
	{
		pthread_mutex_lock(philo->right);
		printf("%zu %d has taken a fork\n", timestamp(), philo->number);
		pthread_mutex_lock(philo->left);
		printf("%zu %d has taken a fork\n", timestamp(), philo->number);
		if (philo->dead)
			break ;
		printf("%zu %d is eating\n", timestamp(), philo->number);
		actualsleep(philo->time_to_eat);
		unlockandsleep(philo);
	}
	return content;
}

int endall(t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->number_of_philosophers)
	{
		vars->philosophers[i]->dead = 1;
		pthread_detach(vars->philosophers[i]->id);
		free(vars->philosophers[i]);
		i++;
	}
	i = 0;
	while (i < vars->number_of_philosophers)
	{
		pthread_mutex_lock(&vars->forks[i]);
		pthread_mutex_destroy(&vars->forks[i++]);
	}
	free(vars->forks);
	free(vars->philosophers);
	vars->over = 1;
	return (0);
}


int mainthread(t_vars *vars)
{
	int i;
	int allate;

	while (1)
	{
		i = 0;
		allate = 1;
		while (i < vars->number_of_philosophers)
		{
			if (timestamp() - vars->philosophers[i]->lastate >= vars->time_to_die)
			{
				printf("%zu %d died\n", timestamp(), i + 1);
				return endall(vars);
			}
			if (vars->philosophers[i]->ate < vars->number_of_times_each_philosopher_must_eat)
				allate = 0;
			i++;
		}
		if (allate && vars->number_of_times_each_philosopher_must_eat != 0)
			return endall(vars);
		actualsleep(5);
	}
}

void createthreads(t_vars *vars)
{
	int i;

	i = 0;
	vars->forks = malloc(vars->number_of_philosophers * sizeof(pthread_mutex_t));
	while (i < vars->number_of_philosophers)
		pthread_mutex_init(&vars->forks[i++], 0);
	i = 0;
	vars->philosophers = malloc(vars->number_of_philosophers * sizeof(t_philo *));
	while (i < vars->number_of_philosophers)
	{
		vars->philosophers[i] = malloc(sizeof(t_philo));
		memset(vars->philosophers[i], 0, sizeof(t_philo));
		vars->philosophers[i]->number = i + 1;
		setforks(vars->philosophers[i], vars);
		vars->philosophers[i]->number_of_philosophers = vars->number_of_philosophers;
		vars->philosophers[i]->time_to_die = vars->time_to_die;
		vars->philosophers[i]->time_to_eat = vars->time_to_eat;
		vars->philosophers[i]->time_to_sleep = vars->time_to_sleep;
		vars->philosophers[i]->number_of_times_each_philosopher_must_eat = vars->number_of_times_each_philosopher_must_eat;
		pthread_create(&vars->philosophers[i]->id, 0, live, vars->philosophers[i]);
		i++;
	}
	mainthread(vars);
}

int main(int argc, char **argv)
{
	t_vars vars;
	if (argc < 5)
		return 0;
	memset(&vars, 0, sizeof(t_vars));
	vars.number_of_philosophers = ft_atoi(argv[1]);
	vars.time_to_die = ft_atoi(argv[2]);
	vars.time_to_eat = ft_atoi(argv[3]);
	vars.time_to_sleep = ft_atoi(argv[4]);
	if (argc > 5)
		vars.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	createthreads(&vars);
	return (0);
}
