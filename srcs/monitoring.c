/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:43:27 by alex              #+#    #+#             */
/*   Updated: 2025/02/04 16:22:05 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*death_monitoring(void *arg)
{
	s_general_data *data = (s_general_data *)arg;
	s_philo **philo = data->philo;
	int	i;

	while (data->programm_run)
	{
		i = 0;
		while (i < data->num_of_philos)
		{
			pthread_mutex_lock(&philo[i]->last_meal_mutex);
			if (get_time_ms() - philo[i]->last_meal_time >= philo[i]->time_to_die)
			{				
				pthread_mutex_lock(&philo[i]->print_lock);
				data->programm_run = 0;
				printf("%ld %d has died\n", get_time_ms() - data->start_time, philo[i]->id);
				pthread_mutex_unlock(&philo[i]->print_lock);
				pthread_mutex_unlock(&philo[i]->last_meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philo[i]->last_meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
