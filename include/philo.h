/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 08:12:54 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/26 08:12:54 by mvachon          ###   ########.fr       */
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
	int				thread_created;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
	int				fork_l;
	int				*fork_r;
	int				have_fork_l;
	int				have_fork_r;
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
	int				monitor_created;
	int				created_threads;
}	t_env;

int		ft_error(const char *msg);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *msg);
size_t	get_current_time(void);
void	free_all(t_env env);
int		init_program(t_env *env, int ac, char **av);
int		init_philo(char **av, t_philo *philos, pthread_mutex_t *forks,
			size_t start_time);
int		handle_single_philo_routine(t_philo *philo, t_program *prog);
void	put_down_fork(t_philo *philo);
void	init_thread_flags(t_env *env);
void	init_philo_forks(t_philo *philos, pthread_mutex_t *forks, int i);
void	init_mutexes(pthread_mutex_t *forks, t_program *prog, int num_philos);
int		setup_env(t_env *env, char **av);
void	assign_mutex_pointers(t_philo *philos, t_program *prog,
			t_thread_arg *args, int num_philos);
int		create_monitor_thread(t_env *env);
void	cleanup(t_philo *philos, pthread_mutex_t *forks, t_program *prog,
			int num_philos);
int		create_threads(t_env *env);
void	join_threads(t_env *env);
void	cleanup_created_threads(t_env *env, int created_count);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);
int		philo_cycle(t_philo *philo, t_program *prog);
int		is_dead(t_program *prog);
void	print_status(t_philo *philo, t_program *prog, const char *msg);
int		check_all_philosophers(t_program *prog);
int		check_philosopher_death(t_program *prog, int i);
int		take_forks(t_philo *philo, t_program *prog);
int		check_args(char **av);
void	ft_usleep(size_t ms);

#endif