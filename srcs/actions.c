/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:26:19 by alex              #+#    #+#             */
/*   Updated: 2025/02/05 13:46:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_eating(t_general_data *data)
{
	int	i;
	int	all_eaten;

	i = 0;
	all_eaten = 1;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->philo[i]->has_eaten_mutex);
		if (data->philo[i]->has_eaten < data->philo[i]->number_to_eat)
			all_eaten = 0;
		pthread_mutex_unlock(&data->philo[i]->has_eaten_mutex);
		i++;
	}
	return (all_eaten);
}

int	unlock_mutexes(t_philo *philo, t_general_data *data)
{
	if (data->programm_run != 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->print_lock);
		pthread_mutex_unlock(&philo->last_meal_mutex);
		return (1);
	}
	return (0);
}

void	eating(t_philo *philo, t_general_data *data)
{
	long	curr_time;

	if (data->programm_run != 1 || (philo->left_fork == philo->right_fork))
		return ;
	if (philo->number_to_eat == philo->has_eaten)
		return ;
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->print_lock);
	pthread_mutex_lock(&philo->last_meal_mutex);
	curr_time = get_time_ms() - data->start_time;
	if (unlock_mutexes(philo, data) == 1)
		return ;
	printf("%ld %d has taken a fork\n", curr_time, philo->id);
	printf("%ld %d has taken a fork\n", curr_time, philo->id);
	printf("%ld %d is eating\n", curr_time, philo->id);
	pthread_mutex_unlock(&philo->print_lock);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	usleep(philo->time_to_eat * 1000);
	philo->has_eaten++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleeping(t_philo *philo, t_general_data *data)
{
	long	curr_time;

	if (data->programm_run != 1 || (philo->left_fork == philo->right_fork))
		return ;
	if (philo->number_to_eat == philo->has_eaten)
		return ;
	pthread_mutex_lock(&philo->print_lock);
	curr_time = get_time_ms() - data->start_time;
	printf("%ld %d is sleeping\n", curr_time, philo->id);
	pthread_mutex_unlock(&philo->print_lock);
	usleep(philo->time_to_sleep * 1000);
}

void	thinking(t_philo *philo, t_general_data *data)
{
	long	curr_time;

	if (data->programm_run != 1 || (philo->left_fork == philo->right_fork))
		return ;
	if (philo->number_to_eat == philo->has_eaten)
		return ;
	pthread_mutex_lock(&philo->print_lock);
	curr_time = get_time_ms() - data->start_time;
	printf("%ld %d is thinking\n", curr_time, philo->id);
	pthread_mutex_unlock(&philo->print_lock);
}
