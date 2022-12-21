/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:05 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/21 08:07:21 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlinkallsemaphores(int murder, t_vars *vars)
{
	int	i;

	i = 0;
	(void)murder;
	while (i < vars->shared.ph_n)
	{
		if (murder)
			kill(vars->pids[i], SIGINT);
		getsem("/data_", i + 1, 2, 1);
		getsem("/ateenough", i + 1, 2, 1);
		i++;
	}
	getsem("/forks", 0, 2, 1);
	i = 0;
	while (i < vars->shared.ph_n)
		waitpid(vars->pids[i++], 0, 0);
	getsem("/philodied", 0, 2, 1);
	getsem("/print", 0, 2, 1);
}

int	endall(t_vars *vars)
{
	unlinkallsemaphores(1, vars);
	free(vars->pids);
	return (0);
}

void	processmainthread(t_vars *vars)
{
	int		i;
	int		allate;
	sem_t	**ateenoughs;

	ateenoughs = malloc(vars->shared.ph_n * sizeof(sem_t *));
	memset(ateenoughs, 0, vars->shared.ph_n * sizeof(sem_t *));
	while (getsem("/philodied", 0, 0, 0) == SEM_FAILED)
	{
		i = 0;
		allate = 1;
		while (i < vars->shared.ph_n)
		{
			if (!ateenoughs[i] || ateenoughs[i] == SEM_FAILED)
				ateenoughs[i] = getsem("/ateenough", i + 1, 0, 0);
			if (ateenoughs[i] == SEM_FAILED)
				allate = 0;
			i++;
			usleep(100);
		}
		if (allate == 1)
			break ;
	}
	free(ateenoughs);
	endall(vars);
}

int	createprocesses(t_vars *vars)
{
	int		i;
	pid_t	pid;

	i = 0;
	vars->pids = malloc(sizeof(pid_t) * vars->shared.ph_n);
	unlinkallsemaphores(0, vars);
	sem_open("/forks", O_CREAT, 0777, vars->shared.ph_n);
	sem_open("/print", O_CREAT, 0777, 1);
	vars->shared.start = timestamp(0);
	while (i < vars->shared.ph_n)
	{
		pid = fork();
		if (pid == 0)
		{
			free(vars->pids);
			return (processlife(vars->shared, i + 1));
		}
		else
			vars->pids[i] = pid;
		i++;
	}
	processmainthread(vars);
	return (0);
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
	createprocesses(&vars);
	return (0);
}
