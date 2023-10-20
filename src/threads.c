/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luciefer <luciefer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 10:29:52 by luciefer          #+#    #+#             */
/*   Updated: 2023/07/28 18:54:23 by luciefer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_max_ate(t_env *env)
{
	int		i;

	pthread_mutex_lock(&env->writing);
	i = env->max_ate;
	pthread_mutex_unlock(&env->writing);
	return (i);
}

void	sleep_sync(t_env *env)
{
	if (env->count % 2)
	{
		if (env->time_to_eat >= env->time_to_sleep)
			usleep((env->time_to_eat - env->time_to_sleep) * 1000);
		usleep(500);
	}
}

static void	*routine(void *params)
{
	t_philo	*philo;
	t_env	*env;

	philo = (t_philo *)params;
	env = philo->env;
	if (env->count == 1)
	{
		philo_print("has taken a fork", philo, UNLOCK);
		new_sleep(env->time_to_die);
		philo_print("died", philo, UNLOCK);
		return (NULL);
	}
	if (philo->pos % 2 && !check_max_ate(env))
		usleep(env->time_to_eat * 500);
	while (!env->stop_condition && !check_max_ate(env))
	{
		philo_eat(philo);
		philo_print("is sleeping", philo, UNLOCK);
		new_sleep(env->time_to_sleep);
		philo_print("is thinking", philo, UNLOCK);
		sleep_sync(env);
	}
	return (NULL);
}

static void	exit_threads(t_env *env)
{
	int	i;

	if (env->count == 1)
		pthread_join(env->philos[0].thread_id, NULL);
	else
	{
		i = -1;
		while (++i < env->count)
			pthread_join(env->philos[i].thread_id, NULL);
	}
	i = -1;
	while (++i < env->count)
		pthread_mutex_destroy(&env->forks[i]);
	pthread_mutex_destroy(&env->meal);
	pthread_mutex_destroy(&env->writing);
	i = -1;
	while (++i < env->count)
		free(env->philos[i].pos_str);
	free(env->philos);
	free(env->forks);
}

int	start_threads(t_env *env)
{
	int	i;

	i = -1;
	env->start_time = get_time();
	while (++i < env->count)
	{
		env->philos[i].last_ate = get_time();
		if (pthread_create(&env->philos[i].thread_id,
				NULL, routine, &(env->philos[i])))
			return (0);
	}
	philo_dead(env, env->philos);
	if (env->stop_condition)
		pthread_mutex_unlock(&env->writing);
	exit_threads(env);
	return (1);
}
