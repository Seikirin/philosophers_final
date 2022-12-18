/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:39:05 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/18 13:11:40 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		k;
	int		j;
	char	*ret;

	i = 0;
	k = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	ret = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ret)
		return (0);
	while (s1[k] != '\0')
		ret[i++] = s1[k++];
	while (s2[j] != '\0')
		ret[i++] = s2[j++];
	ret[i] = '\0';
	free(s2);
	return (ret);
}

void unlinkallsemaphores(int murder, t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->shared.ph_n)
	{
		if (murder)
			kill(vars->pids[i], SIGINT);
		getsem("/data_", i + 1, 2, 1);
		getsem("/ateenough", i + 1, 2, 1);
		i++;
	}
	getsem("/philodied", 0, 2, 1);
	getsem("/forks", 0, 2, 1);
}

int	endall(t_vars *vars)
{
	unlinkallsemaphores(1, vars);
	free(vars->pids);
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

sem_t *getsem(const char *main, int number, int type, int value)
{
	char *str;
	sem_t *ret;

	ret = 0;
	if (number)
		str = ft_strjoin((char *)main, ft_itoa(number));
	else
		str = (char *)main;
	if (!type)
		ret = sem_open(str, O_RDWR);
	else if (type == 2)
		sem_unlink(str);
	else
		ret = sem_open(str, O_CREAT, 0777, value);
	if (number)
		free(str);
	return (ret);
}

int processlife(t_shared shared, int number)
{
	t_philo philo;

	memset(&philo, 0, sizeof(t_philo));
	philo.number = number;
	philo.shared = shared;
	philo.sem = getsem("/forks", 0, 0, 0);
	philo.deadlock = getsem("/data_", number, 0, 0);
	if (philo.number % 2 == 0)
		actualsleep(5, philo.shared.start, &philo);
	while (!checkdeath(&philo, 0, 1) && !checkate(&philo, -1))
	{
		if (takefork(&philo) && takefork(&philo))
		{
			printstate(&philo, EATING);
			actualsleep(philo.shared.eat_t, philo.shared.start, &philo);
			postandsleep(&philo);
		}
	}
	if (checkdeath(&philo, 0, 0) && getsem("/philodied", 0, 0, 0) == SEM_FAILED)
	{
		sem_wait(getsem("/philodied", 0, 1, 1));
		printf("%zu %d died\n", timestamp(philo.shared.start), number);
	}
	else if (checkate(&philo, -1))
	{
		getsem("/ateenough", number, 1, 1);
		printf("%zu %d ate enough\n", timestamp(philo.shared.start), number);
	}
	sem_post(philo.sem);
	sem_post(philo.sem);
	return (0);
}

void	postandsleep(t_philo *philo)
{
	if (checkdeath(philo, 0, 1))
		return ;
	sem_post(philo->sem);
	sem_post(philo->sem);
	checklastate(philo, timestamp(philo->shared.start));
	checkate(philo, 1);
	if (checkate(philo, -1))
		return ;
	printstate(philo, SLEEPING);
	actualsleep(philo->shared.sleep_t, philo->shared.start, philo);
	printstate(philo, THINKING);
}

void processmainthread(t_vars *vars)
{
	int i;
	int allate;
	sem_t **ateenoughs;


	ateenoughs = malloc(vars->shared.ph_n * sizeof(sem_t*));
	memset(ateenoughs, 0, vars->shared.ph_n * sizeof(sem_t*));
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
		}
		if (allate == 1)
			break ;
		usleep(10);
	}
	endall(vars);
}

int createprocesses(t_vars *vars)
{
	int i;
	sem_t *sem;
	pid_t pid;

	i = 0;
	vars->pids = malloc(sizeof(pid_t) * vars->shared.ph_n);
	unlinkallsemaphores(0, vars);
	sem = sem_open("/forks", O_CREAT, 0777, vars->shared.ph_n);
	vars->shared.start = timestamp(0);
	while (i < vars->shared.ph_n)
	{
		pid = fork();
		if (pid == 0)
		{
			free(vars->pids);
			return processlife(vars->shared, i + 1);
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
