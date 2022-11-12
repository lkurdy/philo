/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 21:22:46 by lkurdy            #+#    #+#             */
/*   Updated: 2022/07/29 18:30:04 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	feast(t_philos *philo)
{
	t_info	*info;
	long long	time;

	info = philo->info;
	time = info->eat_time;
	pthread_mutex_lock(&(info->fork[philo->l_fork]));
	ft_print(timestamp() - philo->begin, info, philo, "has taken a fork");
	if (info->number_philo == 1)
	{
		usleep(info->die * 1000);
		pthread_mutex_unlock(&(info->fork[philo->l_fork]));
		return ;
	}
	pthread_mutex_lock(&(info->fork[philo->r_fork]));
	ft_print(timestamp() - philo->begin, info, philo, "has taken a fork");
	pthread_mutex_lock(&(info->mutex));
	if (!ft_print(timestamp() - philo->begin, info, philo, "is eating"))
	{
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&(info->mutex));
		while (!info->dead && time > 0)
		{
			time -= 10;
			usleep(10 * 1000);
		}
		pthread_mutex_lock(&(info->mutex));
		philo->meals = philo->meals + 1;
	}
	pthread_mutex_unlock(&(info->mutex));
	pthread_mutex_unlock(&(info->fork[philo->l_fork]));
	pthread_mutex_unlock(&(info->fork[philo->r_fork]));
}

void	*cycle(void *p)
{
	t_philos	*philo;
	t_info		*info;

	philo = (t_philos *)p;
	info = philo->info;
	if (philo->position % 2)
		usleep(15000);
	while (1)
	{
		feast(philo);
		pthread_mutex_lock(&(info->mutex));
		if (info->all_ate == info->number_philo || info->dead)
			return (pthread_mutex_unlock(&(info->mutex)), NULL);
		if (ft_print(timestamp() - philo->begin, info, philo, "is sleeping"))
			return (pthread_mutex_unlock(&(info->mutex)), NULL);
		pthread_mutex_unlock(&(info->mutex));
		usleep(info->sleep * 1000);
		pthread_mutex_lock(&(info->mutex));
		if (info->dead)
			return (pthread_mutex_unlock(&(info->mutex)), NULL);
		if (ft_print(timestamp() - philo->begin, info, philo, "is thinking"))
			return (pthread_mutex_unlock(&(info->mutex)), NULL);
		pthread_mutex_unlock(&(info->mutex));
	}
	return (NULL);
}

void	launch(t_info *info)
{
	int			i;
	pthread_t	*tid;

	tid = malloc(sizeof(pthread_t) * info->number_philo);
	i = 0;
	while (i < info->number_philo)
	{
		pthread_create(&tid[i], NULL, &cycle, &info->philo[i]);
		i++;
	}
	death(info);
	while (--i >= 0)
		pthread_join(tid[i], NULL);
	i++;
	while (i < info->number_philo)
	{
		pthread_mutex_destroy(&info->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&info->mutex);
	pthread_mutex_destroy(&info->print);
	free(tid);
}

int	parse(int argc, char **argv, t_info *info)
{
	info->number_philo = ft_atoi(argv[1]);
	info->die = ft_atoi(argv[2]);
	info->eat_time = ft_atoi(argv[3]);
	info->sleep = ft_atoi(argv[4]);
	if (info->number_philo <= 0 || info->die < 0
		|| info->eat_time < 0 || info->sleep < 0)
		return (0);
	info->dead = 0;
	info->all_ate = 0;
	info->philo = malloc(sizeof(t_philos) * info->number_philo);
	if (!info->philo)
		return (0);
	if (argc == 6)
	{
		info->eat = ft_atoi(argv[5]);
		if (info->eat < 1)
			return (0);
	}
	else
		info->eat = -1;
	return (parse2(info));
}

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc < 5 || argc > 6)
		return (write(2, "Wrong number of arguments.\n", 27));
	if (!parse(argc, argv, &info))
		return (write(2, "Malloc/mutex error or wrong argument\n", 37));
	launch(&info);
	free(info.philo);
	return (0);
}
