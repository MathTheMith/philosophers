/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:36:56 by marvin            #+#    #+#             */
/*   Updated: 2025/09/30 23:36:56 by marvin           ###   ########.fr       */
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
	ft_usleep(philo->time_to_eat, prog);
}

static void	philo_think(t_philo *philo, t_program *prog)
{
	size_t	think_time;

	print_status(philo, prog, "is thinking");
	if (philo->num_of_philos % 2 == 0)
		return ;
	think_time = 5;
	if (think_time > 0)
		ft_usleep(think_time, prog);
}


static int	handle_single_philo_routine(t_philo *philo, t_program *prog)
{
	print_status(philo, prog, "has taken a fork");
	ft_usleep(philo->time_to_die, prog);
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
		release_forks(philo);
		return (0);
	}
	eat(philo, prog);
	release_forks(philo);
	if (is_dead(prog))
		return (0);
	print_status(philo, prog, "is sleeping");
	ft_usleep(philo->time_to_sleep, prog);
	if (is_dead(prog))
		return (0);
	philo_think(philo, prog);
	return (1);
}
