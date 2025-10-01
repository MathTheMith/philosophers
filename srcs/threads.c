/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:13:09 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:13:09 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	create_philosopher_threads(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num_philos)
	{
		if (pthread_create(&env->philos[i].thread, NULL,
				philo_routine, &env->args[i]) == 0)
		{
			env->philos[i].thread_created = 1;
			env->created_threads++;
		}
		else
		{
			printf("Error: Failed to create philosopher thread %d\n", i);
			break ;
		}
		i++;
	}
	return (0);
}

static int	validate_thread_creation(t_env *env)
{
	if (env->created_threads != env->num_philos)
		return (-1);
	return (0);
}

int	create_threads(t_env *env)
{
	init_thread_flags(env);
	if (create_monitor_thread(env) == -1)
		return (-1);
	create_philosopher_threads(env);
	return (validate_thread_creation(env));
}

void	join_threads(t_env *env)
{
	int	i;

	if (env->monitor_created)
	{
		if (pthread_join(*env->monitor, NULL) != 0)
			printf("Warning: Failed to join monitor thread\n");
	}
	i = 0;
	while (i < env->num_philos)
	{
		if (env->philos[i].thread_created)
		{
			if (pthread_join(env->philos[i].thread, NULL) != 0)
				printf("Warning: Failed to join philosopher thread %d\n", i);
		}
		i++;
	}
}

void	cleanup_created_threads(t_env *env, int created_count)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&env->prog->dead_lock);
	env->prog->dead_flag = 1;
	pthread_mutex_unlock(&env->prog->dead_lock);
	while (i < created_count)
	{
		pthread_join(env->philos[i].thread, NULL);
		i++;
	}
	pthread_join(*env->monitor, NULL);
}
