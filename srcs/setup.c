/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:13:02 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:13:02 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_args(char **av)
{
	if (ft_atoi(av[1]) < 1 || ft_atoi(av[1]) >= 250)
		return (ft_error("Error: argument 1 is not good\n"));
	if (ft_atoi(av[2]) < 1)
		return (ft_error("Error: argument 2 is not good\n"));
	if (ft_atoi(av[3]) < 1)
		return (ft_error("Error: argument 3 is not good\n"));
	if (ft_atoi(av[4]) < 1)
		return (ft_error("Error: argument 4 is not good\n"));
	if (av[5] && ft_atoi(av[5]) < 1)
		return (ft_error("Error: argument 5 is not good\n"));
	return (0);
}

void	assign_mutex_pointers(t_philo *philos, t_program *prog,
				t_thread_arg *args, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		philos[i].write_lock = &prog->write_lock;
		philos[i].dead_lock = &prog->dead_lock;
		philos[i].meal_lock = &prog->meal_lock;
		philos[i].thread_created = 0;
		args[i].philo = &philos[i];
		args[i].prog = prog;
		i++;
	}
}

void	cleanup(t_philo *philos, pthread_mutex_t *forks, t_program *prog,
				int num_philos)
{
	int	i;

	(void)philos;
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&prog->dead_lock);
	pthread_mutex_destroy(&prog->write_lock);
	pthread_mutex_destroy(&prog->meal_lock);
}

int	setup_env(t_env *env, char **av)
{
	env->num_philos = ft_atoi(av[1]);
	if (env->num_philos == 0)
		return (0);
	env->prog->start_time = get_current_time();
	env->prog->num_philos = env->num_philos;
	env->prog->philos = env->philos;
	init_mutexes(env->forks, env->prog, env->num_philos);
	init_philo(av, env->philos, env->forks, env->prog->start_time);
	assign_mutex_pointers(env->philos, env->prog, env->args, env->num_philos);
	return (1);
}

int	init_program(t_env *env, int ac, char **av)
{
	int	num_philos;

	num_philos = ft_atoi(av[1]);
	env->philos = malloc(sizeof(t_philo) * num_philos);
	env->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	env->args = malloc(sizeof(t_thread_arg) * num_philos);
	env->prog = malloc(sizeof(t_program));
	if (!env->philos || !env->forks || !env->args || !env->prog)
	{
		free_all(*env);
		return (ft_error("Allocation failed\n"));
	}
	if (ac != 5 && ac != 6)
	{
		free_all(*env);
		return (ft_error("Wrong argument count\n"));
	}
	if (check_args(av))
	{
		free_all(*env);
		return (ft_error("Error: End of the program\n"));
	}
	return (0);
}
