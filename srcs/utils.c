/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:03:07 by alex              #+#    #+#             */
/*   Updated: 2025/02/10 18:46:06 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	free_philos(t_general_data *data, int last_index)
{
	int	i;

	if (!data || !data->philo)
		return ;
	i = 0;
	while (i < last_index)
	{
		if (data->philo[i])
			free(data->philo[i]);
		i++;
	}
	free(data->philo);
	free(data);
}

void	free_memory_partial(t_general_data *data, int till)
{
	int	i;

	if (!data || !data->philo)
		return ;
	i = 0;
	while (i < till)
	{
		if (data->philo[i])
			free(data->philo[i]);
		i++;
	}
	free(data->philo);
}

void	clean_all(t_general_data *data, int till)
{
	int	i;

	if (till > 0)
		data->num_of_philos = till;
	i = 0;
	while (i < data->num_of_philos)
	{
		if (data->philo[i])
		{
			if (data->philo[i]->left_fork)
			{
				pthread_mutex_destroy(data->philo[i]->left_fork);
				free(data->philo[i]->left_fork);
			}
			pthread_mutex_destroy(&data->philo[i]->print_lock);
			pthread_mutex_destroy(&data->philo[i]->has_eaten_mutex);
			pthread_mutex_destroy(&data->philo[i]->last_meal_mutex);
			free(data->philo[i]);
		}
		i++;
	}
	free(data->philo);
	pthread_mutex_destroy(&data->program_mutex);
	free(data);
}

int	memory_allocate(t_general_data **data, char **args)
{
	*data = (t_general_data *)malloc(sizeof(t_general_data));
	if (!*data)
		return (1);
	(*data)->num_of_philos = ft_atol(args[1]);
	if ((*data)->num_of_philos <= 0)
		return (free(*data), 1);
	(*data)->philo = (t_philo **)malloc(sizeof(t_philo *)
			* (*data)->num_of_philos);
	if (!(*data)->philo)
		return (free(*data), 1);
	if (pthread_mutex_init(&(*data)->program_mutex, NULL) != 0)
		return (free((*data)->philo), free(*data), 1);
	return (0);
}
