/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:11:25 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:11:25 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_args(t_philo *philo, char **av)
{
	philo->num_of_philos = ft_atoi(av[1]);
	philo->time_to_die = ft_atoi(av[2]);
	philo->time_to_eat = ft_atoi(av[3]);
	philo->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		philo->num_times_to_eat = ft_atoi(av[5]);
	else
		philo->num_times_to_eat = -1;
	return (0);
}

static void	init_philo_basic(t_philo *philo, char **av, int id,
	size_t start_time)
{
	init_args(philo, av);
	philo->id = id + 1;
	philo->eating = 0;
	philo->thread_created = 0;
	philo->start_time = start_time;
	philo->last_meal = start_time;
	philo->meals_eaten = 0;
	philo->have_fork_r = 0;
	philo->have_fork_l = 0;
}

int	init_philo(char **av, t_philo *philos, pthread_mutex_t *forks,
				size_t start_time)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		init_philo_basic(&philos[i], av, i, start_time);
		init_philo_forks(philos, forks, i);
		i++;
	}
	return (0);
}

void	init_mutexes(pthread_mutex_t *forks, t_program *prog, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	prog->dead_flag = 0;
	prog->all_ate = 0;
	pthread_mutex_init(&prog->dead_lock, NULL);
	pthread_mutex_init(&prog->write_lock, NULL);
	pthread_mutex_init(&prog->meal_lock, NULL);
}

void	init_thread_flags(t_env *env)
{
	int	i;

	i = 0;
	env->monitor_created = 0;
	env->created_threads = 0;
	while (i < env->num_philos)
	{
		env->philos[i].thread_created = 0;
		i++;
	}
}
