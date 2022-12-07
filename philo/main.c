/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:05 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 11:36:58 by mcharrad         ###   ########.fr       */
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

void	setforks(t_philo *philo, t_vars *vars)
{
	philo->right = &(vars->forks[philo->number - 1]);
	if (philo->number >= vars->shared.ph_n)
		philo->left = &(vars->forks[0]);
	else
		philo->left = &(vars->forks[philo->number]);
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

int	printstate(t_philo *philo, int state)
{
	if (!checkdeath(philo, 0))
	{
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
		return (1);
	}
	return (0);
}

void	unlockandsleep(t_philo *philo)
{
	if (checkdeath(philo, 0))
		return ;
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	checklastate(philo, timestamp(philo->shared.start));
	checkate(philo, 1);
	printstate(philo, SLEEPING);
	actualsleep(philo->shared.sleep_t, philo->shared.start, philo);
	printstate(philo, THINKING);
}

int	takefork(t_philo *philo, pthread_mutex_t *fork)
{
	if (!pthread_mutex_lock(fork) && !checkdeath(philo, 0)
		&& printstate(philo, FORK))
		return (1);
	return (0);
}

void	*live(void *content)
{
	t_philo	*philo;

	philo = content;
	if (philo->number % 2 == 0)
		actualsleep(1, philo->shared.start, philo);
	while (!checkdeath(philo, 0))
	{
		if (takefork(philo, philo->right) && takefork(philo, philo->left))
		{
			printstate(philo, EATING);
			actualsleep(philo->shared.eat_t, philo->shared.start, philo);
			unlockandsleep(philo);
		}
	}
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	return (content);
}

int	endall(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->shared.ph_n)
		checkdeath(vars->philos[i++], 1);
	i = 0;
	while (i < vars->shared.ph_n)
	{
		pthread_join(vars->philos[i]->id, 0);
		free(vars->philos[i++]);
	}
	i = 0;
	while (i < vars->shared.ph_n)
	{
		pthread_mutex_lock(&vars->forks[i]);
		pthread_mutex_unlock(&vars->forks[i]);
		pthread_mutex_destroy(&vars->forks[i++]);
	}
	free(vars->forks);
	free(vars->philos);
	vars->over = 1;
	return (0);
}

int	mainthread(t_vars *vars)
{
	int	i;
	int	allate;

	while (1)
	{
		i = 0;
		allate = 1;
		while (i < vars->shared.ph_n)
		{
			if (timestamp(vars->shared.start) - checklastate(vars->philos[i], 0)
				>= vars->shared.die_t)
			{
				printf("%zu %d died\n", timestamp(vars->shared.start), i + 1);
				return (endall(vars));
			}
			if (checkate(vars->philos[i], 0) < vars->shared.eat_m)
				allate = 0;
			i++;
		}
		if (allate && vars->shared.eat_m != 0)
			return (endall(vars));
		actualsleep(1, vars->shared.start, 0);
	}
}

void	createthreads(t_vars *vars)
{
	int	i;

	i = 0;
	vars->forks = malloc(vars->shared.ph_n * sizeof(pthread_mutex_t));
	while (i < vars->shared.ph_n)
		pthread_mutex_init(&vars->forks[i++], 0);
	i = 0;
	vars->philos = malloc(vars->shared.ph_n * sizeof(t_philo *));
	vars->shared.start = timestamp(0);
	while (i < vars->shared.ph_n)
	{
		vars->philos[i] = malloc(sizeof(t_philo));
		memset(vars->philos[i], 0, sizeof(t_philo));
		vars->philos[i]->number = i + 1;
		setforks(vars->philos[i], vars);
		ft_memcpy(&vars->philos[i]->shared, &vars->shared, sizeof(t_shared));
		pthread_mutex_init(&vars->philos[i]->deadlock, 0);
		pthread_create(&vars->philos[i]->id, 0, live, vars->philos[i]);
		i++;
	}
	mainthread(vars);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc < 5)
		return (0);
	memset(&vars, 0, sizeof(t_vars));
	vars.shared.ph_n = ft_atoi(argv[1]);
	vars.shared.die_t = ft_atoi(argv[2]);
	vars.shared.eat_t = ft_atoi(argv[3]);
	vars.shared.sleep_t = ft_atoi(argv[4]);
	if (argc > 5)
		vars.shared.eat_m = ft_atoi(argv[5]);
	createthreads(&vars);
	return (0);
}
