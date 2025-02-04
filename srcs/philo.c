/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:02:54 by alex              #+#    #+#             */
/*   Updated: 2025/02/04 17:28:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eating(s_philo *philo, s_general_data *data)
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
	if (data->programm_run != 1)
	{
		pthread_mutex_unlock(&philo->print_lock);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->last_meal_mutex);
		return ;
	}
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

void	sleeping(s_philo *philo, s_general_data *data)
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

void	thinking(s_philo *philo, s_general_data *data)
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

int	check_eating(s_general_data *data)
{
	int i = 0;
	int	all_eaten = 1;

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

void	*philo_routine(void *arg)
{
	s_as_arg *myarg = (s_as_arg *)arg;
	s_philo *philo = myarg->data->philo[myarg->i];
	int		i;

	i = 0;
	while (myarg->data->programm_run)
	{
		if (philo->number_to_eat != -1 && check_eating(myarg->data))
		{
			myarg->data->programm_run = 0;
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

int	get_num_of_eat(int n, char **args, s_philo *phylo)
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

void	get_forks(s_philo **philo, int num_of_philo, int i)
{
	philo[i]->left_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)); //
	pthread_mutex_init(philo[i]->left_fork, NULL);
	
	if (i == num_of_philo - 1)
		philo[i]->right_fork = philo[0]->left_fork;
	else
		philo[i]->right_fork = NULL;
}

int	fill_philos(int n, char **args, s_general_data **data)
{
	int	i;

	if (memory_allocate(data, args) == 1)
		return (1);
	(*data)->programm_run = 1;
	(*data)->start_time = get_time_ms();
	i = 0;
	while (i < (*data)->num_of_philos)
	{
		(*data)->philo[i] = (s_philo *)malloc(sizeof(s_philo));
		if (!(*data)->philo[i])
			return (clean_all(*data), 1);
		(*data)->philo[i]->id = i + 1;
		(*data)->philo[i]->time_to_die = ft_atol(args[2]);
		(*data)->philo[i]->time_to_eat = ft_atol(args[3]);
		(*data)->philo[i]->time_to_sleep = ft_atol(args[4]);
		(*data)->philo[i]->last_meal_time = get_time_ms();
		get_forks((*data)->philo, (*data)->num_of_philos, i);
		if (i > 0)
			(*data)->philo[i - 1]->right_fork = (*data)->philo[i]->left_fork;
		if ((*data)->philo[i]->time_to_die <= 0 || (*data)->philo[i]->time_to_eat <= 0 || (*data)->philo[i]->time_to_sleep <= 0)
			return (clean_all(*data), 1);
		pthread_mutex_init(&(*data)->philo[i]->print_lock, NULL);
		pthread_mutex_init(&(*data)->philo[i]->has_eaten_mutex, NULL);
		pthread_mutex_init(&(*data)->philo[i]->last_meal_mutex, NULL);
		if (get_num_of_eat(n, args, (*data)->philo[i]) == 1)
			return (clean_all(*data), 1);
		(*data)->philo[i]->has_eaten = 0;
		i++;
	}
	return (0);
}

int main(int n, char **args)
{
	s_general_data	*data;
	s_as_arg		*arg;
	pthread_t		death_mon;
	int	i;

	if (n < 5 || n > 6)
		return (1);
	if (fill_philos(n, args, &data) == 1)
		return (1);
	i = 0;
	while (i < data->num_of_philos)
	{
		arg = (s_as_arg *)malloc(sizeof(s_as_arg));
		if (!arg)
			return (clean_all(data), 1);
		arg->data = data;
		arg->i = i;
		if (pthread_create(&(data)->philo[i]->thread, NULL, philo_routine, (void *)arg) != 0)
			return (clean_all(data), free(arg), 1);
		i++;
	}
	if (pthread_create(&death_mon, NULL, death_monitoring, (void *)data) != 0)
		return (clean_all(data), 1);
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(data->philo[i]->thread, NULL) != 0)
			return (clean_all(data), 1);
		i++;
	}
	if (pthread_join(death_mon, NULL) != 0)
		return (clean_all(data), free(arg), 1);
	return (clean_all(data), 0);
}

/* 
синхронизировать вилки
реализовать мониторинг за смертью */

// for (int i = 0; i < data->num_of_philos; i++)
// {
	// printf("id: %d; die: %d; eat: %d; sleep: %d\n", data->philo[i]->id, data->philo[i]->time_to_die, data->philo[i]->time_to_eat, data->philo[i]->time_to_sleep);
// }
/* 
1. thinking
2. eating
3. sleeping */