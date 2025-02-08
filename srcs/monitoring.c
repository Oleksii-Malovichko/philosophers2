/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:43:27 by alex              #+#    #+#             */
/*   Updated: 2025/02/06 19:39:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	get_num_of_eat(int n, char **args, t_philo *phylo)
{
	if (n == 6)
	{
		phylo->number_to_eat = ft_atol(args[5]);
		if (phylo->number_to_eat <= 0)
			return (1);
	}
	else
		phylo->number_to_eat = -1;
	return (0);
}

int	get_forks(t_philo **philo, int num_of_philo, int i)
{
	philo[i]->left_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!philo[i]->left_fork)
		return (0);
	pthread_mutex_init(philo[i]->left_fork, NULL);
	if (i == num_of_philo - 1)
		philo[i]->right_fork = philo[0]->left_fork;
	else
		philo[i]->right_fork = NULL;
	return (1);
}

int	timeout(t_philo *philo, t_general_data *data)
{
	long	curr_time;
	long	last_meal_time;
	int		philo_id;

	pthread_mutex_lock(&philo->last_meal_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	curr_time = get_time_ms();
	philo_id = philo->id;
	if (curr_time - last_meal_time >= philo->time_to_die + 5.0)
	{
		pthread_mutex_lock(&data->program_mutex);
		data->programm_run = 0;
		pthread_mutex_unlock(&data->program_mutex);
		pthread_mutex_lock(&philo->print_lock);
		printf("%ld %d has died\n", curr_time - data->start_time, philo_id);
		pthread_mutex_unlock(&philo->print_lock);
		return (1);
	}
	return (0);
}

void	*death_monitoring(void *arg)
{
	t_general_data	*data;
	t_philo			**philo;
	int				i;
	int				stop_monitoring;

	data = (t_general_data *)arg;
	philo = data->philo;
	while (1)
	{
		pthread_mutex_lock(&data->program_mutex);
		stop_monitoring = !(data->programm_run);
		pthread_mutex_unlock(&data->program_mutex);
		if (stop_monitoring)
			break ;
		i = 0;
		while (i < data->num_of_philos)
		{
			if (timeout(philo[i], data))
				return (NULL);
			i++;
		}
		usleep(2000);
	}
	return (NULL);
}
