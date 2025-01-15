/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 01:50:31 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/12 13:10:54 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	minmaxcheck(long v)
{
	if (v > INT_MAX || v < INT_MIN)
		exit(1);
}

void	digitcheck(char *str, int i)
{
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i++]))
		{
			write(2, "Error: Invalid input (non-numeric input found)\n", 47);
			exit(1);
		}
	}
}

int	ft_atoi(char *str)
{
	int		i;
	long	v;
	int		s;

	i = 0;
	v = 0;
	s = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i + 1] != '\0' && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		v = v * 10 + (str[i] - 48);
		minmaxcheck((v * s));
		i++;
	}
	digitcheck(str, i);
	v *= s;
	return (v);
}
