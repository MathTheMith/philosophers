/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:53 by marvin            #+#    #+#             */
/*   Updated: 2025/06/25 04:49:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_program *prog)
{
	int	dead;

	pthread_mutex_lock(&prog->dead_lock);
	dead = prog->dead_flag || prog->all_ate;
	pthread_mutex_unlock(&prog->dead_lock);
	return (dead);
}

void	print_status(t_philo *philo, t_program *prog, const char *msg)
{
	size_t	now;
	size_t	timestamp;

	now = get_current_time();
	timestamp = now - prog->start_time;
	pthread_mutex_lock(&prog->dead_lock);
	if (!prog->dead_flag && !prog->all_ate)
	{
		pthread_mutex_lock(&prog->write_lock);
		printf("%zu %d %s\n", timestamp, philo->id, msg);
		pthread_mutex_unlock(&prog->write_lock);
	}
	pthread_mutex_unlock(&prog->dead_lock);
}

int	check_philosopher_death(t_program *prog, int i)
{
	size_t	now;

	now = get_current_time();
	if (now - prog->philos[i].last_meal > prog->philos[i].time_to_die)
	{
		pthread_mutex_lock(&prog->dead_lock);
		prog->dead_flag = 1;
		pthread_mutex_unlock(&prog->dead_lock);
		pthread_mutex_unlock(&prog->meal_lock);
		pthread_mutex_lock(&prog->write_lock);
		printf("%zu %d died\n", now - prog->start_time, prog->philos[i].id);
		pthread_mutex_unlock(&prog->write_lock);
		return (1);
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_thread_arg	*args;
	t_philo			*philo;
	t_program		*prog;

	args = (t_thread_arg *)arg;
	philo = args->philo;
	prog = args->prog;

	if (philo->id % 2 != 0)
		ft_usleep(10, prog);

	while (!is_dead(prog))
	{
		if (!philo_cycle(philo, prog))
			return (NULL);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_program	*prog;

	prog = (t_program *)arg;
	while (!is_dead(prog))
	{
		if (check_all_philosophers(prog) == -1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
