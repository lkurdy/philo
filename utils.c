/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 20:01:10 by lkurdy            #+#    #+#             */
/*   Updated: 2022/07/29 14:32:22 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *npt)
{
	int		i;
	long	p;
	int		m;

	i = 0;
	p = 0;
	m = 1;
	while ((npt[i] <= 13 && npt[i] >= 9) || npt[i] == ' ')
		i++;
	if (npt[i] == '+' || npt[i] == '-')
	{
		if (npt[i] == '-')
			m = -m;
		i++;
	}
	while (npt[i] <= '9' && npt[i] >= '0')
	{
		p = p * 10;
		p = p + (npt[i] - 48);
		i++;
	}
	if (npt[i])
		return (-1);
	return (p * m);
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	death(t_info *info)
{
	int	i;

	while (info->all_ate != info->number_philo)
	{
		i = 0;
		while (i < info->number_philo && !(info->dead))
		{
			pthread_mutex_lock(&(info->mutex));
			if ((timestamp() - info->philo[i].last_meal) > info->die)
			{
				pthread_mutex_lock(&(info->print));
				printf("%lld %d died\n", timestamp() - info->philo[i].begin,
					info->philo[i].position);
				pthread_mutex_unlock(&(info->print));
				info->dead = 1;
			}
			pthread_mutex_unlock(&(info->mutex));
			i++;
			usleep(100);
		}
		if (info->dead)
			break ;
		counter(info);
	}
}

void	counter(t_info *info)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(info->mutex));
	while (info->eat > -1 && i < info->number_philo
		&& info->philo[i].meals >= info->eat)
		i++;
	if (i == info->number_philo)
		info->all_ate = info->number_philo;
	pthread_mutex_unlock(&(info->mutex));
}

int	parse2(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->number_philo)
	{
		info->philo[i].position = i + 1;
		info->philo[i].meals = 0;
		info->philo[i].last_meal = timestamp();
		info->philo[i].begin = timestamp();
		info->philo[i].l_fork = i;
		info->philo[i].r_fork = (i + 1) % info->number_philo;
		info->philo[i].info = info;
		i++;
	}
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(info->fork[i]), NULL))
			return (0);
	}
	if (pthread_mutex_init(&(info->mutex), NULL))
		return (0);
	if (pthread_mutex_init(&(info->print), NULL))
		return (0);
	return (1);
}
