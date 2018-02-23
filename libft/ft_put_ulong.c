/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_ulong.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/08 06:43:21 by rabougue          #+#    #+#             */
/*   Updated: 2016/07/09 00:58:32 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	ft_put_ulong(unsigned long n)
{
	if (n == 18446744073709551615UL)
	{
		ft_putstr("1");
		n = 8446744073709551615;
	}
	if (n > 9)
	{
		ft_put_ulong(n / 10);
		ft_put_ulong(n % 10);
	}
	else
		ft_putchar('0' + n);
}
