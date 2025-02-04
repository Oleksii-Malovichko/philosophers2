/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:03:07 by alex              #+#    #+#             */
/*   Updated: 2025/02/04 20:17:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	clean_all(t_general_data *data, int till)
{
	int	i;

	i = 0;
	if (till > 0)
		data->num_of_philos = till;
	while (i < data->num_of_philos)
	{
		if (data->philo[i])
		{
			if (data->philo[i]->left_fork != NULL)
			{
				pthread_mutex_destroy(data->philo[i]->left_fork);
				free(data->philo[i]->left_fork);
				data->philo[i]->left_fork = NULL;
			}			
			pthread_mutex_destroy(&data->philo[i]->print_lock);
			pthread_mutex_destroy(&data->philo[i]->has_eaten_mutex);
			pthread_mutex_destroy(&data->philo[i]->last_meal_mutex);			
			free(data->philo[i]);
		}
		i++;
	}
	free(data->philo);
	free(data);
}

long	ft_atol(const char *str)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] <= 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * sign);
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
	return (0);
}
