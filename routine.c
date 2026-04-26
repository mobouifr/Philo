/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:24:31 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/17 02:53:38 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *philosophers)
{
	size_t	time;

	time = philosophers->args->program_start_time;
	pthread_mutex_lock(&philosophers->args->died_philo);
	if (philosophers->args->kill_philo == 1)
	{
		pthread_mutex_unlock(&philosophers->args->died_philo);
		return ;
	}
	pthread_mutex_unlock(&philosophers->args->died_philo);
	print_state(philosophers, time, "is thinking");
	usleep(100);
}

void	eating(t_philo *philosophers)
{
	size_t			time;
	pthread_mutex_t	*left_hand;
	pthread_mutex_t	*right_hand;

	left_hand = philosophers->left_fork;
	right_hand = philosophers->right_fork;
	time = philosophers->args->program_start_time;
	switch_last_philo_forks(&left_hand, &right_hand);
	pthread_mutex_lock(left_hand);
	print_state(philosophers, time, "has taken a fork");
	pthread_mutex_lock(right_hand);
	print_state(philosophers, time, "has taken a fork");
	print_state(philosophers, time, "is eating");
	pthread_mutex_lock(&philosophers->args->last_meal_mutex);
	philosophers->last_meal_time = get_time();
	pthread_mutex_unlock(&philosophers->args->last_meal_mutex);
	ft_usleep(philosophers->args->time_to_eat);
	pthread_mutex_lock(&philosophers->args->meal_eaten_mutex);
	philosophers->meals_eaten++;
	pthread_mutex_unlock(&philosophers->args->meal_eaten_mutex);
	pthread_mutex_unlock(left_hand);
	pthread_mutex_unlock(right_hand);
	ft_usleep(1);
}

void	sleeping(t_philo *philosophers)
{
	size_t	time;

	time = philosophers->args->program_start_time;
	pthread_mutex_lock(&philosophers->args->died_philo);
	if (philosophers->args->kill_philo == 1)
	{
		pthread_mutex_unlock(&philosophers->args->died_philo);
		return ;
	}
	pthread_mutex_unlock(&philosophers->args->died_philo);
	print_state(philosophers, time, "is sleeping");
	ft_usleep(philosophers->args->time_to_sleep);
}

int	only_one_philo(t_philo *philosophers, size_t time)
{
	pthread_mutex_lock(&philosophers->args->print_lock);
	if (philosophers->args->number_of_philosophers == 1)
	{
		printf("%zu %lu %s\n", get_time() - time, philosophers->id + 1,
			"has taken a fork");
		ft_usleep(philosophers->args->time_to_die);
		pthread_mutex_unlock(&philosophers->args->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->args->print_lock);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philosophers;
	size_t	time;

	philosophers = (t_philo *)arg;
	time = philosophers->args->program_start_time;
	if (philosophers->id % 2 != 0)
		ft_usleep(60);
	if (only_one_philo(philosophers, time))
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(&philosophers->args->print_lock);
		if (philosophers->args->kill_philo == 1)
		{
			pthread_mutex_unlock(&philosophers->args->print_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philosophers->args->print_lock);
		eating(philosophers);
		if (is_full(philosophers))
			break ;
		thinking(philosophers);
		sleeping(philosophers);
	}
	return (NULL);
}
