/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 18:52:34 by alex              #+#    #+#             */
/*   Updated: 2025/02/10 18:52:52 by alex             ###   ########.fr       */
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
