/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcharrad <mcharrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:25:39 by mcharrad          #+#    #+#             */
/*   Updated: 2022/12/11 11:41:25 by mcharrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	while (dstsize != 0 && i < (dstsize - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
		dst[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}

static void	ft_putchar(char c, char *str, int *i)
{
	str[*i] = c;
	*i += 1;
}

static void	ft_putnbr(int nb, char *str, int *i)
{
	if (nb == -2147483648)
	{
		ft_putchar('-', str, i);
		ft_putchar('2', str, i);
		ft_putnbr(147483648, str, i);
	}
	else
	{
		if (nb < 0)
		{
			ft_putchar('-', str, i);
			nb = -nb;
		}
		if (nb < 10)
		{
			ft_putchar(nb + '0', str, i);
		}
		else
		{
			ft_putnbr(nb / 10, str, i);
			ft_putnbr(nb % 10, str, i);
		}
	}
}

char	*ft_itoa(int n)
{
	int		store;
	char	str[13];
	char	*final;
	int		i;

	store = n;
	i = 0;
	ft_putnbr(n, str, &i);
	str[i] = '\0';
	final = (char *)malloc(i + 1);
	if (!final)
		return (0);
	ft_strlcpy(final, str, i + 1);
	return (final);
}
