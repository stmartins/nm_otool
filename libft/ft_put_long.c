/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_long.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/07 16:56:29 by stmartin          #+#    #+#             */
/*   Updated: 2016/10/07 16:56:30 by stmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	ft_put_long(long n)
{
	if (n == -9223372036854775807 - 1)
	{
		ft_putstr("-9");
		n = 223372036854775808;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n > 9)
	{
		ft_put_long(n / 10);
		ft_put_long(n % 10);
	}
	else
		ft_putchar('0' + n);
}
