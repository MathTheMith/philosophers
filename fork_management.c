/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:40:01 by marvin            #+#    #+#             */
/*   Updated: 2025/09/30 23:40:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	release_forks(t_philo *philo)
{
	if (philo->has_right_fork)
	{
		pthread_mutex_unlock(philo->r_fork);
		philo->has_right_fork = 0;
	}
	if (philo->has_left_fork)
	{
		pthread_mutex_unlock(philo->l_fork);
		philo->has_left_fork = 0;
	}
}

static int	take_first_fork(t_philo *philo, t_program *prog,
				pthread_mutex_t *first_fork)
{
	pthread_mutex_lock(first_fork);
	if (first_fork == philo->l_fork)
		philo->has_left_fork = 1;
	else
		philo->has_right_fork = 1;
	if (is_dead(prog))
	{
		release_forks(philo);
		return (0);
	}
	print_status(philo, prog, "has taken a fork");
	return (1);
}

static int	take_second_fork(t_philo *philo, t_program *prog,
				pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(second_fork);
	if (second_fork == philo->l_fork)
		philo->has_left_fork = 1;
	else
		philo->has_right_fork = 1;
	if (is_dead(prog))
	{
		release_forks(philo);
		return (0);
	}
	print_status(philo, prog, "has taken a fork");
	return (1);
}

int	take_forks(t_philo *philo, t_program *prog)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->num_of_philos == 1)
		return (handle_single_philo(philo, prog));
	get_fork_order(philo, &first_fork, &second_fork);
	if (!take_first_fork(philo, prog, first_fork))
		return (0);
	if (!take_second_fork(philo, prog, second_fork))
		return (0);
	return (1);
}
