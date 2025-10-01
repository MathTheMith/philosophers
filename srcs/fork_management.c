/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:49:03 by mvachon           #+#    #+#             */
/*   Updated: 2025/10/01 12:50:18 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	put_down_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->l_fork);
	philo->fork_l = 1;
	pthread_mutex_unlock(&philo->l_fork);
	philo->have_fork_l = 0;
	pthread_mutex_lock(philo->r_fork);
	*(philo->fork_r) = 1;
	pthread_mutex_unlock(philo->r_fork);
	philo->have_fork_r = 0;
}

static	int	take_fork_l(t_philo *philo, t_program *prog)
{
	if (philo->have_fork_l == 1)
		return (1);
	pthread_mutex_lock(&philo->l_fork);
	if (philo->fork_l == 1)
	{
		philo->fork_l = 0;
		pthread_mutex_unlock(&philo->l_fork);
		philo->have_fork_l = 1;
		print_status(philo, prog, "has taken a fork");
		return (1);
	}
	pthread_mutex_unlock(&philo->l_fork);
	return (0);
}

static	int	take_fork_r(t_philo *philo, t_program *prog)
{
	if (philo->have_fork_r == 1)
		return (1);
	pthread_mutex_lock(philo->r_fork);
	if (*(philo->fork_r) == 1)
	{
		*(philo->fork_r) = 0;
		pthread_mutex_unlock(philo->r_fork);
		philo->have_fork_r = 1;
		print_status(philo, prog, "has taken a fork");
		return (1);
	}
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}

int	take_forks(t_philo *philo, t_program *prog)
{
	if (take_fork_l(philo, prog) == 0)
	{
		return (0);
	}
	if (take_fork_r(philo, prog) == 0)
		return (0);
	if (philo->have_fork_l == 1 && philo->have_fork_r == 1)
		return (1);
	return (0);
}
