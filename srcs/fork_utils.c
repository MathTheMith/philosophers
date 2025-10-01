/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:13:50 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:13:50 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static	int	check_all_ate(t_program *prog, int i)
{
	if (prog->philos[i].num_times_to_eat != -1
		&& prog->philos[i].meals_eaten
		< prog->philos[i].num_times_to_eat)
		return (0);
	return (1);
}

int	handle_single_philo_routine(t_philo *philo, t_program *prog)
{
	print_status(philo, prog, "has taken a fork");
	usleep(philo->time_to_die * 1000);
	return (0);
}

int	create_monitor_thread(t_env *env)
{
	if (pthread_create(env->monitor, NULL, monitor_routine, env->prog) == 0)
	{
		env->monitor_created = 1;
		return (0);
	}
	printf("Error: Failed to create monitor thread\n");
	return (-1);
}

void	init_philo_forks(t_philo *philos, pthread_mutex_t *forks, int i)
{
	philos[i].l_fork = forks[i];
	philos[i].fork_l = 1;
	if (i == 0)
	{
		philos[0].fork_r = &philos[philos->num_of_philos - 1].fork_l;
		philos[0].r_fork = &philos[philos->num_of_philos - 1].l_fork;
	}
	else
	{
		philos[i].fork_r = &philos[i - 1].fork_l;
		philos[i].r_fork = &philos[i - 1].l_fork;
	}
}

int	check_all_philosophers(t_program *prog)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < prog->num_philos && !is_dead(prog))
	{
		pthread_mutex_lock(&prog->meal_lock);
		if (check_philosopher_death(prog, i))
			return (-1);
		if (!check_all_ate(prog, i))
			all_ate = 0;
		pthread_mutex_unlock(&prog->meal_lock);
		i++;
	}
	if (prog->philos[0].num_times_to_eat != -1 && all_ate)
	{
		pthread_mutex_lock(&prog->dead_lock);
		prog->all_ate = 1;
		pthread_mutex_unlock(&prog->dead_lock);
		return (-1);
	}
	return (0);
}
