/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 19:48:47 by lkurdy            #+#    #+#             */
/*   Updated: 2022/07/27 21:22:27 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

struct		s_list;

typedef struct t_philo
{
	int				position;
	int				meals;
	long long		last_meal;
	long long		begin;
	int				l_fork;
	int				r_fork;
	struct s_list	*info;
}		t_philos;

typedef struct s_list
{
	int				number_philo;
	int				die;
	int				eat_time;
	int				sleep;
	int				eat;
	int				dead;
	int				all_ate;
	pthread_mutex_t fork[250];
	pthread_mutex_t	mutex;
	pthread_mutex_t	print;
	t_philos		*philo;
}		t_info;

int			ft_atoi(const char *npt);
long long	timestamp(void);
void		death(t_info *info);
void		counter(t_info *info);
int			parse2(t_info *info);
int			ft_print(long long t, t_info *info, t_philos *philo, char *str);

#endif
