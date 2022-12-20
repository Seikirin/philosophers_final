/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 15:09:04 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/19 08:37:13 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

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
