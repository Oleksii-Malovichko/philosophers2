/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:02:54 by alex              #+#    #+#             */
/*   Updated: 2025/02/06 19:53:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*philo_routine(void *arg)
{
	t_as_arg	*myarg;
	t_philo		*philo;
	int			i;

	myarg = (t_as_arg *)arg;
	philo = myarg->data->philo[myarg->i];
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&myarg->data->program_mutex);
		if (myarg->data->programm_run == 0)
		{
			pthread_mutex_unlock(&myarg->data->program_mutex);
			break ;
		}
		pthread_mutex_unlock(&myarg->data->program_mutex);
		if (philo->number_to_eat != -1 && check_eating(myarg->data))
		{
			pthread_mutex_lock(&myarg->data->program_mutex);
			myarg->data->programm_run = 0;
			pthread_mutex_unlock(&myarg->data->program_mutex);
			break ;
		}
		eating(philo, myarg->data);
		sleeping(philo, myarg->data);
		thinking(philo, myarg->data);
		i++;
	}
	free(myarg);
	return (NULL);
}

int	init_part(t_general_data *data, int i, char **args)
{
	data->philo[i]->id = i + 1;
	data->philo[i]->time_to_die = ft_atol(args[2]);
	data->philo[i]->time_to_eat = ft_atol(args[3]);
	data->philo[i]->time_to_sleep = ft_atol(args[4]);
	data->philo[i]->last_meal_time = get_time_ms();
	if (data->philo[i]->time_to_die <= 0 || data->philo[i]->time_to_eat <= 0
		|| data->philo[i]->time_to_sleep <= 0 || data->philo[i]->id <= 0)
		return (1);
	if (get_forks(data->philo, data->num_of_philos, i) == 0)
		return (1);
	if (i > 0)
		data->philo[i - 1]->right_fork = data->philo[i]->left_fork;
	return (0);
}

int	fill_philos(int n, char **args, t_general_data **data)
{
	int	i;

	if (memory_allocate(data, args) == 1)
		return (1);
	(*data)->programm_run = 1;
	(*data)->start_time = get_time_ms();
	i = 0;
	while (i < (*data)->num_of_philos)
	{
		(*data)->philo[i] = (t_philo *)malloc(sizeof(t_philo));
		if (!(*data)->philo[i])
			return (free_philos(*data, i + 1), 1);
		if (get_num_of_eat(n, args, (*data)->philo[i]) == 1)
			return (free_philos(*data, i + 1), 1);
		if (init_part(*data, i, args) == 1)
			return (free_philos(*data, i + 1), 1);
		pthread_mutex_init(&(*data)->philo[i]->print_lock, NULL);
		pthread_mutex_init(&(*data)->philo[i]->has_eaten_mutex, NULL);
		pthread_mutex_init(&(*data)->philo[i]->last_meal_mutex, NULL);
		(*data)->philo[i]->has_eaten = 0;
		i++;
	}
	return (0);
}

int	start_threading_philos(t_general_data *data)
{
	int			i;
	t_as_arg	*arg;

	i = 0;
	while (i < data->num_of_philos)
	{
		arg = (t_as_arg *)malloc(sizeof(t_as_arg));
		if (!arg)
			return (clean_all(data, 0), 1);
		arg->data = data;
		arg->i = i;
		if (pthread_create(&(data)->philo[i]->thread, NULL,
				philo_routine, (void *)arg) != 0)
			return (clean_all(data, 0), free(arg), 1);
		i++;
	}
	return (0);
}

int	main(int n, char **args)
{
	t_general_data	*data;
	pthread_t		death_mon;
	int				i;

	if (n < 5 || n > 6)
		return (1);
	if (fill_philos(n, args, &data) == 1)
		return (1);
	if (start_threading_philos(data) == 1)
		return (1);
	if (pthread_create(&death_mon, NULL, death_monitoring, (void *)data) != 0)
		return (clean_all(data, 0), 1);
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(data->philo[i]->thread, NULL) != 0)
			return (clean_all(data, 0), 1);
		i++;
	}
	if (pthread_join(death_mon, NULL) != 0)
		return (clean_all(data, 0), 1);
	return (clean_all(data, 0), 0);
}
