#include "philo.h"

void	print_state(t_philo *philosophers, size_t time, char *state)
{
	//
	//
	pthread_mutex_lock(&philosophers->args->died_philo);
	pthread_mutex_lock(&philosophers->args->print_lock);
	if (philosophers->args->kill_philo == 1)
	{
		pthread_mutex_unlock(&philosophers->args->print_lock);
		pthread_mutex_unlock(&philosophers->args->died_philo);
		return ;		
	}
	// if (philosophers->args->break_routine == 0)
	printf("%zu %lu %s\n", get_time() - time, philosophers->id + 1, state);
	pthread_mutex_unlock(&philosophers->args->print_lock);
	pthread_mutex_unlock(&philosophers->args->died_philo);

}

size_t	get_time(void)
{
	struct timeval tv;

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

void parser(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		exit(1);
	}
	args->number_of_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->has_optional_argument = 0;
	args->break_routine = 0;
	args->kill_philo = 0;
	if (args->number_of_philosophers < 1
		|| args->time_to_die < 1 || args->time_to_eat < 1
		|| args->time_to_sleep < 1)
	{
		write(2, "Error: Arguments must be positive integers\n", 43);
		exit (1);
	}
	if (argc == 6)
	{
		args->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
		if (args->number_of_times_each_philosopher_must_eat < 1)
		{
			write(2, "Error: Arguments must be positive integers\n", 43);
			exit (1);	
		}
		args->has_optional_argument = 1;
	}
}
void	init_philosophers(t_args *args)
{
	args->philosophers = malloc(sizeof(t_philo) * args->number_of_philosophers);
	args->forks = malloc(sizeof(pthread_mutex_t) * args->number_of_philosophers);
	size_t i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_init(&args->forks[i], NULL);
		args->philosophers[i].id = i;
		args->philosophers[i].left_fork = &args->forks[i];
		args->philosophers[i].right_fork = &args->forks[(i + 1) % args->number_of_philosophers];
		args->philosophers[i].args = args;
		args->philosophers[i].args->program_start_time = get_time();
		args->philosophers[i].last_meal_time = get_time();
		i++;
	}
	pthread_mutex_init(&args->print_lock, NULL);
}



void	thinking(t_philo *philosophers)
{
	size_t time;

	time = philosophers->args->program_start_time;
		//
	pthread_mutex_lock(&philosophers->args->died_philo);
		if (philosophers->args->kill_philo == 1)
		{
			pthread_mutex_unlock(&philosophers->args->died_philo);
			return ;		
		}
		pthread_mutex_unlock(&philosophers->args->died_philo);
	//
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
	//
	pthread_mutex_lock(&philosophers->args->died_philo);
		if (philosophers->args->kill_philo == 1)
		{
			pthread_mutex_unlock(&philosophers->args->died_philo);
			return ;		
		}
		pthread_mutex_unlock(&philosophers->args->died_philo);
	//
	if (philosophers->id % 2 == 0)
	{
		left_hand = philosophers->right_fork;
		right_hand = philosophers->left_fork;
	}
	pthread_mutex_lock(left_hand);
	print_state(philosophers, time, "has taken a fork");
	pthread_mutex_lock(right_hand);
	print_state(philosophers, time, "has taken a fork");
	print_state(philosophers, time, "is eating");
	philosophers->last_meal_time = get_time();//test
	ft_usleep(philosophers->args->time_to_eat);
	philosophers->meals_eaten++;

	pthread_mutex_unlock(left_hand);
	pthread_mutex_unlock(right_hand);

}


void	sleeping(t_philo *philosophers)
{
	size_t time;

	time = philosophers->args->program_start_time;
		//
	pthread_mutex_lock(&philosophers->args->died_philo);
		if (philosophers->args->kill_philo == 1)
		{
			pthread_mutex_unlock(&philosophers->args->died_philo);
			return ;		
		}
		pthread_mutex_unlock(&philosophers->args->died_philo);
	//
	print_state(philosophers, time, "is sleeping");
	ft_usleep(philosophers->args->time_to_sleep);
}

int is_full(t_philo philosophers)
{
	//lock
	int b = philosophers.args->has_optional_argument 
			&& philosophers.meals_eaten >= philosophers.args->number_of_times_each_philosopher_must_eat;
	return b;
}

void	*routine(void *arg)
{
	t_philo *philosophers;
	size_t	time;

	philosophers = (t_philo *)arg;
	time = philosophers->args->program_start_time;

	if (philosophers->id % 2 != 0)
		ft_usleep(60);
	pthread_mutex_lock(&philosophers->args->print_lock);
	if (philosophers->args->number_of_philosophers == 1)
	{
		print_state(philosophers, time, "has taken a fork");
		ft_usleep(philosophers->args->time_to_die);
		print_state(philosophers, time, "died");
		return (NULL);
	}
	pthread_mutex_unlock(&philosophers->args->print_lock);
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
		if (is_full(*philosophers))
			break ;
		thinking(philosophers);
		sleeping(philosophers);
	}
	return (NULL);
}

int is_dead(t_args *args, size_t i)
{
	//lock meal
	int b = get_time() - args->philosophers[i].last_meal_time > args->time_to_die;
	//unlock meal
	return b;
}

void	*monitor_routine(void *arg)
{
	size_t	i;
	size_t time;
	t_args *args;

	args = (t_args *)arg;
	time = args->program_start_time;
	while (1)
	{
		i = 0;
		while (i < args->number_of_philosophers)
		{
			pthread_mutex_lock(&args->died_philo);
			if (!is_full(args->philosophers[i]) && is_dead(args, i))
			{
				pthread_mutex_lock(&args->print_lock);
				printf("%zu %lu %s\n", get_time() - time, args->philosophers[i].id + 1, "died");
				args->kill_philo = 1;
				pthread_mutex_unlock(&args->print_lock);
				pthread_mutex_unlock(&args->died_philo);
				return (NULL);
			}
			pthread_mutex_unlock(&args->died_philo);
			usleep(500);
			i++;
		}
		if (args->has_optional_argument)
		{
			i = 0;
			args->break_routine = 1;
			while (i < args->number_of_philosophers)
			{
				if (args->philosophers[i].meals_eaten < args->number_of_times_each_philosopher_must_eat)
				{
					args->break_routine = 0;
					break ;
				}
				i++;
				usleep(500);
			}
			if (args->break_routine == 1)
				return (NULL);
		}
		usleep(500);
	}
	// pthread_mutex_unlock(&args->died_philo);
	return (NULL);
}

void	creat_philosophers(t_args *args)
{
	size_t	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_create(&args->philosophers[i].thread, NULL, (void *)routine, &args->philosophers[i]);
		i++;
	}
	monitor_routine(args);
	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_join(args->philosophers[i].thread, NULL);
		i++;
	}
	// if (args->number_of_philosophers % 2 != 0)
	// 	ft_usleep(1);
}

void	clean_up(t_args *args)
{
	size_t	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_destroy(&args->forks[i]);
		i++;
	}
	if (args->philosophers)
		free(args->philosophers);
	if (args->forks)
		free(args->forks);
	free(args);
}

int	main(int argc, char **argv)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	parser(argc, argv, args);
	init_philosophers(args);
	args->program_start_time = get_time();
	//printf("---------------------{%u}\n------------------------------", args->philosophers[]->args->program_start_time);
	creat_philosophers(args);
	clean_up(args);
	return (0);
}