/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_var.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 09:34:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/14 09:36:54 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "color.h"

void	v3f_print(t_v3f v)
{
	printf("v3f <%.4f, %.4f, %.4f>\n", v.x, v.y, v.z);
}

void	col32_print(t_col32 c)
{
	printf("col32 <%i, %i, %i, %i>\n", get_r(c), get_g(c), get_b(c), get_a(c));
}
