/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:18:19 by alex              #+#    #+#             */
/*   Updated: 2025/02/04 15:49:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHiLO_H
# define PHiLO_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <limits.h>

typedef struct t_philo
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
	pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t print_lock;
	pthread_mutex_t	has_eaten_mutex;
	pthread_mutex_t	last_meal_mutex;
}	s_philo;

typedef struct t_general_data
{
	s_philo			**philo;
	int				num_of_philos;
	int				programm_run;
	long			start_time;
}	s_general_data;

typedef struct t_as_arg
{
	s_general_data	*data;
	int				i;
}	s_as_arg;

long	ft_atol(const char *str);
int		memory_allocate(s_general_data **data, char **args);
long	get_time_ms(void);
void	*death_monitoring(void *arg);
void	get_forks(s_philo **philo, int num_of_philo, int i);
void	clean_all(s_general_data *data);
int		check_eating(s_general_data *data);

#endif