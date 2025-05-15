/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_var.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 09:34:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 14:18:25 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "debug/rt_debug.h"

void	mat4x4_rot_print(t_mat4x4 m)
{
	const t_v3f	right = init_v3f(1.0f, 0.0f, 0.0f);
	const t_v3f	up = init_v3f(0.0f, 1.0f, 0.0f);
	const t_v3f	forward = init_v3f(0.0f, 0.0f, 1.0f);
	t_v3f		angles;

	angles.x = acosf(v3f_dot(init_v3f(m[0], m[1], m[2]), right)) * RADTODEG;
	angles.y = acosf(v3f_dot(init_v3f(m[4], m[5], m[6]), up)) * RADTODEG;
	angles.z = acosf(v3f_dot(init_v3f(m[8], m[9], m[10]), forward)) * RADTODEG;
	v3f_print(angles);
}

void	v3f_print(t_v3f v)
{
	printf("v3f <%.4f, %.4f, %.4f>\n", v.x, v.y, v.z);
}

void	col32_print(t_col32 c)
{
	printf("col32 <%i, %i, %i, %i>\n", get_r(c), get_g(c), get_b(c), get_a(c));
}

void	mat4x4_print(t_mat4x4 m)
{
	const int	width = 4;
	const int	preci = 4;

	printf("\
|%3$ *1$.*2$f %4$ *1$.*2$f %5$ *1$.*2$f %6$ *1$.*2$f|\n\
|%7$ *1$.*2$f %8$ *1$.*2$f %9$ *1$.*2$f %10$ *1$.*2$f|\n\
|%11$ *1$.*2$f %12$ *1$.*2$f %13$ *1$.*2$f %14$ *1$.*2$f|\n\
|%15$ *1$.*2$f %16$ *1$.*2$f %17$ *1$.*2$f %18$ *1$.*2$f|\n\
", width, preci,
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
}
