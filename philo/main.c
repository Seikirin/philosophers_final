/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:05 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/07 12:20:56 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
