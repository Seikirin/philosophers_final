/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 13:53:19 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/12 10:57:23 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	skipspace(const char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\n'
		|| str[*i] == '\t' || str[*i] == '\v'
		|| str[*i] == '\f' || str[*i] == '\r')
		*i += 1;
}

int	skipsign(const char *str, int *i)
{
	if (str[*i] == '-' || str[*i] == '+')
	{
		*i += 1;
		if (str[(*i) - 1] == '-')
			return (-1);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long	final;
	int		i;
	int		neg;

	i = 0;
	final = 0;
	skipspace(str, &i);
	neg = skipsign(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
		final = (final * 10) + (str[i++] - '0');
	final *= neg;
	if (final > INT_MAX)
		return (-1);
	else if (final < INT_MIN)
		return (0);
	return (final);
}

void	ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dst && !src)
		return ;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
}
