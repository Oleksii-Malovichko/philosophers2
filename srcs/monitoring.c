/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:43:27 by alex              #+#    #+#             */
/*   Updated: 2025/02/04 19:58:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	timeout(t_philo *philo, t_general_data *data)
{
	if (get_time_ms() - philo->last_meal_time >= philo->time_to_die + 5.0)
	{
		pthread_mutex_lock(&philo->print_lock);
		data->programm_run = 0;
		printf("%ld %d has died\n", \
			get_time_ms() - data->start_time, philo->id);
		pthread_mutex_unlock(&philo->print_lock);
		pthread_mutex_unlock(&philo->last_meal_mutex);
		return (1);
	}
	return (0);
}

void	*death_monitoring(void *arg)
{
	t_general_data	*data;
	t_philo			**philo;
	int				i;

	data = (t_general_data *)arg;
	philo = data->philo;
	while (data->programm_run)
	{
		i = 0;
		while (i < data->num_of_philos)
		{
			pthread_mutex_lock(&philo[i]->last_meal_mutex);
			if (timeout(philo[i], data))
				return (NULL);
			pthread_mutex_unlock(&philo[i]->last_meal_mutex);
			i++;
		}
		usleep(2000);
	}
	return (NULL);
}
