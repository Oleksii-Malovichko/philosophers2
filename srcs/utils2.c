/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:52:34 by alex              #+#    #+#             */
/*   Updated: 2025/02/11 13:06:32 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	routine_condition(t_as_arg *myarg, t_philo *philo)
{
	pthread_mutex_lock(&myarg->data->program_mutex);
	if (myarg->data->programm_run == 0)
	{
		pthread_mutex_unlock(&myarg->data->program_mutex);
		return (1);
	}
	pthread_mutex_unlock(&myarg->data->program_mutex);
	if (philo->number_to_eat != -1 && check_eating(myarg->data))
	{
		pthread_mutex_lock(&myarg->data->program_mutex);
		myarg->data->programm_run = 0;
		pthread_mutex_unlock(&myarg->data->program_mutex);
		return (1);
	}
	return (0);
}

int	eating_condition(t_philo *philo, t_general_data *data)
{
	int		run;
	int		meals;

	pthread_mutex_lock(&data->program_mutex);
	run = data->programm_run;
	pthread_mutex_unlock(&data->program_mutex);
	if (run != 1 || (philo->left_fork == philo->right_fork))
		return (1);
	pthread_mutex_lock(&philo->has_eaten_mutex);
	meals = philo->has_eaten;
	pthread_mutex_unlock(&philo->has_eaten_mutex);
	if (philo->number_to_eat == meals)
		return (1);
	return (0);
}

void	choose_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
}

void	print_eating(t_philo *philo, long curr_time, int philo_id)
{
	printf("%ld %d has taken a fork\n", curr_time, philo_id);
	printf("%ld %d has taken a fork\n", curr_time, philo_id);
	printf("%ld %d is eating\n", curr_time, philo_id);
	usleep(philo->time_to_eat * 1000);
}
