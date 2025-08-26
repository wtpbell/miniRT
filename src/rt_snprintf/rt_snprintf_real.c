/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf_real.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/22 11:46:08 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/26 12:15:16 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ieee754.h>
#include "rt_snprintf.h"

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

static void	handle_real(t_str *str, double numeric, const double prec)
{
	double	fract;

	numeric = fabs(numeric);
	fract = (numeric - floor(numeric));
	if ((1.0 - fract) < pow(10.0, -(prec + (prec == 1.0))))
	{
		numeric = round(numeric);
		fract = 0.0;
	}
	utos(str, numeric, '\0');
	if (prec == 0.0)
		return ;
	ctos(str, '.');
	fdectos(str, round(fract * pow(10.0, prec)), prec);
}

void	ftos(t_str *str, double r)
{
	const double			prec = 2.0;
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
		stos(str, "0");
		if (prec == 0.0)
			return ;
		stos(str, ".");
		fdectos(str, 0, prec);
	}
	else
		handle_real(str, r, prec);
}
