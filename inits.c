/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:45:37 by marvin            #+#    #+#             */
/*   Updated: 2025/05/25 05:45:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	init_philo(char **av, t_philo *philos, pthread_mutex_t *forks,
				size_t start_time)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		init_args(&philos[i], av);
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].start_time = start_time;
		philos[i].last_meal = start_time;
		philos[i].meals_eaten = 0;
		philos[i].has_left_fork = 0;
		philos[i].has_right_fork = 0;
		philos[i].l_fork = &forks[i];
		philos[i].r_fork = &forks[(i + 1) % ft_atoi(av[1])];
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

void	create_threads(t_env *env)
{
	int	i;

	i = 0;
	pthread_create(env->monitor, NULL, monitor_routine, env->prog);
	while (i < env->num_philos)
	{
		pthread_create(&env->philos[i].thread, NULL,
			philo_routine, &env->args[i]);
		i++;
	}
}

void	join_threads(t_philo *philos, pthread_t monitor, int num_philos)
{
	int	i;

	i = 0;
	pthread_join(monitor, NULL);
	while (i < num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}
