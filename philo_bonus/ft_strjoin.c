/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 11:44:53 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/11 11:43:53 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ft_strlen(char const *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char *s2)
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
