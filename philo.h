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
	size_t	id;
	size_t	meals_eaten;
	size_t	last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_args	*args;
}					t_philo;

typedef struct s_args
{
	size_t	number_of_philosophers;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	number_of_times_each_philosopher_must_eat;
	size_t	has_optional_argument;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	died_philo;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	meal_eaten_mutex;
	size_t	break_routine;
	size_t	kill_philo;
	size_t	start_time;
	size_t	program_start_time;
	t_philo			*philosophers;
}					t_args;

void				parser(int argc, char **argv, t_args *args);
int					ft_atoi(char *str);
int					ft_isdigit(int d);
void				print_state(t_philo *philosophers, size_t time,
						char *state);
size_t				get_time(void);
void				ft_usleep(size_t time);
void				parser(int argc, char **argv, t_args *args);
void				init_philosophers(t_args *args);
void				thinking(t_philo *philosophers);
void				eating(t_philo *philosophers);
void				sleeping(t_philo *philosophers);
void				*routine(void *arg);
void				*monitor_routine(void *arg);
void				creat_philosophers(t_args *args);
void				clean_up(t_args *args);

#endif