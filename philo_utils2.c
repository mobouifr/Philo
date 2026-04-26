/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:24:43 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/16 17:12:27 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	switch_last_philo_forks(pthread_mutex_t **left_hand,
		pthread_mutex_t **right_hand)
{
	pthread_mutex_t	*temp;

	if (*left_hand > *right_hand)
	{
		temp = *left_hand;
		*left_hand = *right_hand;
		*right_hand = temp;
	}
}

int	is_full(t_philo *philosophers)
{
	int	b;

	b = 0;
	pthread_mutex_lock(&philosophers->args->meal_eaten_mutex);
	b = philosophers->args->has_optional_argument
		&& philosophers->meals_eaten
		>= philosophers->args->times_philosopher_must_eat;
	pthread_mutex_unlock(&philosophers->args->meal_eaten_mutex);
	return (b);
}

void	mutex_initialiser(t_args *args)
{
	pthread_mutex_init(&args->print_lock, NULL);
	pthread_mutex_init(&args->last_meal_mutex, NULL);
	pthread_mutex_init(&args->meal_eaten_mutex, NULL);
	pthread_mutex_init(&args->died_philo, NULL);
}
