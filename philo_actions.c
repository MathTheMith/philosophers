/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:53 by marvin            #+#    #+#             */
/*   Updated: 2025/06/25 04:49:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo, t_program *prog)
{
	print_status(philo, prog, "is eating");
	pthread_mutex_lock(&prog->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&prog->meal_lock);
	usleep(philo->time_to_eat * 1000);
}

static size_t	calculate_think_time(t_philo *philo)
{
	size_t	think_time;

	think_time = philo->time_to_eat;
	if (philo->time_to_eat > philo->time_to_sleep)
		think_time = philo->time_to_eat - philo->time_to_sleep;
	return (think_time);
}

static void	philo_think(t_philo *philo, t_program *prog)
{
	size_t	think_time;

	print_status(philo, prog, "is thinking");
	if (philo->num_of_philos % 2 == 1)
	{
		think_time = calculate_think_time(philo);
		usleep(think_time * 1000);
	}
	else
		usleep(1000);
}

static int	handle_single_philo_routine(t_philo *philo, t_program *prog)
{
	print_status(philo, prog, "has taken a fork");
	usleep(philo->time_to_die * 1000);
	return (0);
}

int	philo_cycle(t_philo *philo, t_program *prog)
{
	if (philo->num_of_philos == 1)
		return (handle_single_philo_routine(philo, prog));
	if (!take_forks(philo, prog))
		return (0);
	if (is_dead(prog))
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	eat(philo, prog);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	if (is_dead(prog))
		return (0);
	print_status(philo, prog, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
	if (is_dead(prog))
		return (0);
	philo_think(philo, prog);
	return (1);
}
