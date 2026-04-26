/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:24:37 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/17 03:11:57 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_args *args, size_t i)
{
	int	b;

	b = 0;
	pthread_mutex_lock(&args->last_meal_mutex);
	b = get_time() - args->philosophers[i].last_meal_time > args->time_to_die;
	pthread_mutex_unlock(&args->last_meal_mutex);
	return (b);
}

int	optional_argument_found(t_args *args)
{
	size_t	i;

	i = 0;
	if (args->has_optional_argument)
	{
		i = 0;
		args->break_routine = 1;
		while (i < args->number_of_philosophers)
		{
			pthread_mutex_lock(&args->meal_eaten_mutex);
			if (args->philosophers[i].meals_eaten
				< args->times_philosopher_must_eat)
			{
				args->break_routine = 0;
				pthread_mutex_unlock(&args->meal_eaten_mutex);
				break ;
			}
			pthread_mutex_unlock(&args->meal_eaten_mutex);
			i++;
			ft_usleep(1);
		}
		if (args->break_routine == 1)
			return (1);
	}
	return (0);
}

int	philo_should_be_killed(t_args *args, size_t time)
{
	size_t	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_lock(&args->died_philo);
		if (!is_full(&args->philosophers[i]) && is_dead(args, i))
		{
			pthread_mutex_lock(&args->print_lock);
			printf("%zu %lu %s\n", get_time() - time, args->philosophers[i].id
				+ 1, "died");
			args->kill_philo = 1;
			pthread_mutex_unlock(&args->print_lock);
			pthread_mutex_unlock(&args->died_philo);
			return (1);
		}
		pthread_mutex_unlock(&args->died_philo);
		ft_usleep(1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	size_t	time;
	t_args	*args;

	args = (t_args *)arg;
	time = args->program_start_time;
	while (1)
	{
		if (philo_should_be_killed(args, time) == 1)
			return (NULL);
		if (optional_argument_found(args) == 1)
			return (NULL);
		ft_usleep(1);
	}
	return (NULL);
}
