/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:24:34 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/17 01:30:04 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philosophers, size_t time, char *state)
{
	pthread_mutex_lock(&philosophers->args->died_philo);
	pthread_mutex_lock(&philosophers->args->print_lock);
	if (philosophers->args->kill_philo == 1)
	{
		pthread_mutex_unlock(&philosophers->args->print_lock);
		pthread_mutex_unlock(&philosophers->args->died_philo);
		return ;
	}
	printf("%zu %lu %s\n", get_time() - time, philosophers->id + 1, state);
	pthread_mutex_unlock(&philosophers->args->print_lock);
	pthread_mutex_unlock(&philosophers->args->died_philo);
}

void	philo_is_dead(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->args->died_philo);
	if (philosophers->args->kill_philo == 1)
	{
		pthread_mutex_unlock(&philosophers->args->died_philo);
		return ;
	}
	pthread_mutex_unlock(&philosophers->args->died_philo);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		usleep(500);
	}
}

void	init_args(char **argv, t_args *args)
{
	args->number_of_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->has_optional_argument = 0;
	args->break_routine = 0;
	args->kill_philo = 0;
}
