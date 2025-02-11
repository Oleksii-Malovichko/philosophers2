/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:18:19 by alex              #+#    #+#             */
/*   Updated: 2025/02/11 13:06:53 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <limits.h>

typedef struct s_philo
{
	int				id;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				number_to_eat;
	int				has_eaten;
	pthread_t		thread;
	long			last_meal_time;
	int				exit_flag;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	has_eaten_mutex;
	pthread_mutex_t	last_meal_mutex;
}	t_philo;

typedef struct s_general_data
{
	t_philo			**philo;
	int				num_of_philos;
	int				programm_run;
	pthread_mutex_t	program_mutex;
	long			start_time;
}	t_general_data;

typedef struct s_as_arg
{
	t_general_data	*data;
	int				i;
}	t_as_arg;

long	ft_atol(const char *str);
int		routine_condition(t_as_arg *myarg, t_philo *philo);
int		memory_allocate(t_general_data **data, char **args);
long	get_time_ms(void);
void	*death_monitoring(void *arg);
int		get_forks(t_philo **philo, int num_of_philo, int i);
void	clean_all(t_general_data *data, int till);
int		check_eating(t_general_data *data);
void	eating(t_philo *philo, t_general_data *data);
void	sleeping(t_philo *philo, t_general_data *data);
void	thinking(t_philo *philo, t_general_data *data);
int		unlock_mutexes(t_philo *philo, t_general_data *data);
int		get_num_of_eat(int n, char **args, t_philo *phylo);
int		get_forks(t_philo **philo, int num_of_philo, int i);
void	free_philos(t_general_data *data, int last_index);
void	free_memory_partial(t_general_data *data, int till);
int		eating_condition(t_philo *philo, t_general_data *data);
void	choose_forks(t_philo *philo);
void	print_eating(t_philo *philo, long curr_time, int philo_id);

#endif