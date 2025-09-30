/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:35:34 by marvin            #+#    #+#             */
/*   Updated: 2025/09/30 23:35:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
	else
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
}

void	free_all(t_env env)
{
	if (env.philos)
		free(env.philos);
	if (env.args)
		free(env.args);
	if (env.forks)
		free(env.forks);
	if (env.prog)
		free(env.prog);
}

int	handle_single_philo(t_philo *philo, t_program *prog)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, prog, "has taken a fork");
	ft_usleep(philo->time_to_die, prog);
	pthread_mutex_unlock(philo->l_fork);
	return (0);
}

static int	check_all_ate(t_program *prog, int i)
{
	if (prog->philos[i].num_times_to_eat != -1
		&& prog->philos[i].meals_eaten < prog->philos[i].num_times_to_eat)
		return (0);
	return (1);
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
