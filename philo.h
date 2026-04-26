/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 03:26:09 by mobouifr          #+#    #+#             */
/*   Updated: 2025/01/17 01:52:00 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_philo
{
	size_t			id;
	size_t			meals_eaten;
	size_t			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_args	*args;
}					t_philo;

typedef struct s_args
{
	size_t			number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			times_philosopher_must_eat;
	size_t			has_optional_argument;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	died_philo;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	meal_eaten_mutex;
	size_t			break_routine;
	size_t			kill_philo;
	size_t			start_time;
	size_t			program_start_time;
	t_philo			*philosophers;
}					t_args;

int					ft_atoi(char *str);
int					ft_isdigit(int d);
void				print_state(t_philo *philosophers, size_t time,
						char *state);
size_t				get_time(void);
void				ft_usleep(size_t time);
int					parser(int argc, char **argv, t_args *args);
int					init_philosophers(t_args *args);
void				thinking(t_philo *philosophers);
void				eating(t_philo *philosophers);
void				sleeping(t_philo *philosophers);
void				*routine(void *arg);
void				*monitor_routine(void *arg);
int					creat_philosophers(t_args *args);
void				clean_up(t_args *args);
void				philo_is_dead(t_philo *philosophers);
void				switch_last_philo_forks(pthread_mutex_t **left_hand,
						pthread_mutex_t **right_hand);
int					is_full(t_philo *philosophers);
int					only_one_philo(t_philo *philosophers, size_t time);
int					is_dead(t_args *args, size_t i);
int					optional_argument_found(t_args *args);
int					philo_should_be_killed(t_args *args, size_t time);
void				mutex_initialiser(t_args *args);
void				init_args(char **argv, t_args *args);

#endif