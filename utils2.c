#include "philo.h"

int	ft_print(long long t, t_info *info, t_philos *philo, char *str)
{
	pthread_mutex_lock(&(info->print));
	if (!(info->dead))
	{
		printf("%lld %d ", timestamp() - philo->begin, philo->position);
		printf("%s\n", str);
	}
	else
		return (pthread_mutex_unlock(&(info->print)), 1);
	pthread_mutex_unlock(&(info->print));
	return (0);
}
