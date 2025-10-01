/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:11:39 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:11:39 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat(t_philo *philo, t_program *prog)
{
	print_status(philo, prog, "is eating");
	pthread_mutex_lock(&prog->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&prog->meal_lock);
	usleep(philo->time_to_eat * 1000);
}

static	size_t	calculate_think_time(t_philo *philo)
{
	size_t	think_time;

	think_time = philo->time_to_eat;
	if (philo->time_to_eat > philo->time_to_sleep)
		think_time = philo->time_to_eat - philo->time_to_sleep;
	return (think_time);
}

static	void	philo_think(t_philo *philo, t_program *prog)
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

void	ft_usleep(size_t ms)
{
	size_t	start;

	if (ms == 0)
		return ;
	start = get_current_time();
	while ((get_current_time() - start) < ms)
		usleep(500);
}

int	philo_cycle(t_philo *philo, t_program *prog)
{
	philo_think(philo, prog);
	if (philo->num_of_philos == 1)
		return (handle_single_philo_routine(philo, prog));
	while (take_forks(philo, prog) != 1 && is_dead(prog) == 0)
	{
		ft_usleep(1);
	}
	if (philo->have_fork_l == 1 && philo->have_fork_l == 1)
	{
		eat(philo, prog);
		put_down_fork(philo);
	}
	if (is_dead(prog))
		return (0);
	print_status(philo, prog, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
	if (is_dead(prog))
		return (0);
	return (1);
}
