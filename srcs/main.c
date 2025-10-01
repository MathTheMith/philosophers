/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:11:34 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:11:34 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	handle_thread_creation_error(t_env *env)
{
	printf("Error: Thread creation failed, cleaning up...\n");
	pthread_mutex_lock(&env->prog->dead_lock);
	env->prog->dead_flag = 1;
	pthread_mutex_unlock(&env->prog->dead_lock);
	join_threads(env);
	cleanup(env->philos, env->forks, env->prog, env->num_philos);
	free_all(*env);
	return (1);
}

static int	run_simulation(t_env *env)
{
	if (create_threads(env) != 0)
		return (handle_thread_creation_error(env));
	join_threads(env);
	cleanup(env->philos, env->forks, env->prog, env->num_philos);
	free_all(*env);
	return (0);
}

int	main(int ac, char **av)
{
	t_env		env;
	pthread_t	monitor;

	if (init_program(&env, ac, av) != 0)
		return (1);
	if (!setup_env(&env, av))
		return (0);
	env.monitor = &monitor;
	return (run_simulation(&env));
}
