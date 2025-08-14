/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf_num.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/13 20:04:06 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 21:05:07 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ieee754.h>
#include "rt_snprintf.h"

static size_t	int_count(unsigned int n)
{
	size_t	count;

	count = (n == 0);
	while (n != 0)
	{
		n /= 10;
		++count;
	}
	return (count);
}

static void	fdectos(t_str *str, int decimal, int prec)
{
	size_t	count;
	int		pad;

	count = int_count(decimal);
	pad = prec - count;
	while (pad > 0)
	{
		ctos(str, '0');
		--pad;
	}
	utos(str, decimal, '\0');
}

void	utos(t_str *str, unsigned int n, char prefix)
{
	size_t	count;
	char	*dst;

	count = int_count(n) + (prefix != '\0');
	while (str->len + count >= str->cap)
	{
		n /= 10;
		--count;
	}
	if (count == 0)
		return ;
	dst = str->s + (str->len + count - 1);
	if (n == 0)
		*dst-- = '0';
	while (n != 0)
	{
		*dst = '0' + (n % 10);
		n /= 10;
		--dst;
	}
	if (prefix != '\0')
		*dst = prefix;
	str->len += count;
}

void	itos(t_str *str, int n)
{
	if (n < 0)
		utos(str, ~n + 1u, '-');
	else
		utos(str, n, '\0');
}

void	ftos(t_str *str, double r)
{
	const double			prec = 4.0;
	double					numeric;
	union ieee754_double	*d;

	d = (union ieee754_double *) & r;
	if (d->ieee.negative == 1)
		ctos(str, '-');
	if (isinf(r))
		stos(str, "inf");
	else if (isnan(r))
		stos(str, "nan");
	else if (r == 0.0f)
	{
		stos(str, "0.");
		fdectos(str, 0, prec);
	}
	else
	{
		numeric = fabs(r);
		utos(str, numeric, '\0');
		ctos(str, '.');
		fdectos(str, round((numeric - floor(numeric)) * pow(10.0, prec)), prec);
	}
}
