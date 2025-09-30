/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 05:42:29 by marvin            #+#    #+#             */
/*   Updated: 2025/06/25 05:42:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	main(int ac, char **av)
{
	t_env			env;
	pthread_t		monitor;

	env.philos = malloc(sizeof(t_philo) * 250);
	env.forks = malloc(sizeof(pthread_mutex_t) * 250);
	env.args = malloc(sizeof(t_thread_arg) * 250);
	env.prog = malloc(sizeof(t_program));
	if (!env.philos || !env.forks || !env.args || !env.prog)
		return (free_all(env), ft_error("Allocation failed\n"));
	if (ac != 5 && ac != 6)
		return (free_all(env), ft_error("Wrong argument count\n"));
	if (check_args(av))
		return (free_all(env), ft_error("Error: End of the program\n"));
	if (!setup_env(&env, av))
		return (0);
	env.monitor = &monitor;
	create_threads(&env);
	join_threads(env.philos, monitor, env.num_philos);
	cleanup(env.philos, env.forks, env.prog, env.num_philos);
	free_all(env);
	return (0);
}
