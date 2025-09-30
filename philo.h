/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:37:14 by marvin            #+#    #+#             */
/*   Updated: 2025/05/24 15:37:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				has_left_fork;
	int				has_right_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef struct s_program
{
	int				dead_flag;
	int				all_ate;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
	size_t			start_time;
	int				num_philos;
}					t_program;

typedef struct s_thread_arg
{
	t_philo		*philo;
	t_program	*prog;
}					t_thread_arg;

typedef struct s_env
{
	t_philo			*philos;
	t_thread_arg	*args;
	t_program		*prog;
	pthread_mutex_t	*forks;
	pthread_t		*monitor;
	int				num_philos;
}	t_env;

int		handle_single_philo(t_philo *philo, t_program *prog);
void	release_forks(t_philo *philo);
void	ft_usleep(size_t milliseconds, t_program *prog);
int		ft_error(const char *msg);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *msg);
int		init_philo(char **av, t_philo *philos, pthread_mutex_t *forks,
			size_t start_time);
size_t	get_current_time(void);
void	free_all(t_env env);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);
int		is_dead(t_program *prog);
void	print_status(t_philo *philo, t_program *prog, const char *msg);
void	init_mutexes(pthread_mutex_t *forks, t_program *prog, int num_philos);
void	create_threads(t_env *env);
void	join_threads(t_philo *philos, pthread_t monitor, int num_philos);
int		philo_cycle(t_philo *philo, t_program *prog);
int		check_all_philosophers(t_program *prog);
int		check_philosopher_death(t_program *prog, int i);
int		take_forks(t_philo *philo, t_program *prog);
void	get_fork_order(t_philo *philo, pthread_mutex_t **first,
			pthread_mutex_t **second);

#endif