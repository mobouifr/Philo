/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:24:47 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/17 02:09:28 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_up(t_args *args)
{
	size_t	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_destroy(&args->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args->print_lock);
	pthread_mutex_destroy(&args->died_philo);
	pthread_mutex_destroy(&args->last_meal_mutex);
	pthread_mutex_destroy(&args->meal_eaten_mutex);
	if (args->philosophers)
		free(args->philosophers);
	if (args->forks)
		free(args->forks);
	free(args);
}

int	creat_philosophers(t_args *args)
{
	size_t	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		if (pthread_create(&args->philosophers[i].thread, NULL, (void *)routine,
				&args->philosophers[i]) != 0)
		{
			printf("Error: Failed to create thread");
			return (1);
		}
		i++;
	}
	monitor_routine(args);
	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_join(args->philosophers[i].thread, NULL);
		i++;
	}
	return (0);
}

int	init_philosophers(t_args *args)
{
	size_t	i;

	args->philosophers = malloc(sizeof(t_philo) * args->number_of_philosophers);
	args->forks = malloc(sizeof(pthread_mutex_t)
			* args->number_of_philosophers);
	if (!args->philosophers || !args->forks)
	{
		printf("Error: Memory allocation failed");
		return (1);
	}
	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_init(&args->forks[i], NULL);
		args->philosophers[i].id = i;
		args->philosophers[i].left_fork = &args->forks[i];
		args->philosophers[i].right_fork = &args->forks[(i + 1)
			% args->number_of_philosophers];
		args->philosophers[i].args = args;
		args->philosophers[i].last_meal_time = get_time();
		i++;
	}
	mutex_initialiser(args);
	return (0);
}

int	parser(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		return (1);
	}
	init_args(argv, args);
	if (args->number_of_philosophers < 1 || args->time_to_die < 1
		|| args->time_to_eat < 1 || args->time_to_sleep < 1)
	{
		write(2, "Error: Arguments must be positive integers\n", 43);
		return (1);
	}
	if (argc == 6)
	{
		args->times_philosopher_must_eat = ft_atoi(argv[5]);
		if (args->times_philosopher_must_eat < 1)
		{
			write(2, "Error: Arguments must be positive integers\n", 43);
			return (1);
		}
		args->has_optional_argument = 1;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	if (parser(argc, argv, args) == 1)
	{
		free(args);
		return (1);
	}
	if (init_philosophers(args) == 1)
	{
		clean_up(args);
		return (1);
	}
	args->program_start_time = get_time();
	if (creat_philosophers(args) == 1)
	{
		clean_up(args);
		return (1);
	}
	clean_up(args);
	return (0);
}
