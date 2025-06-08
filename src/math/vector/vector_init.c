/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:35:46 by jboon             #+#    #+#             */
/*   Updated: 2025/06/08 18:18:59 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

const t_v3f	g_v3f_zero = (t_v3f){.x = 0.0f, .y = 0.0f, .z = 0.0f};
const t_v3f	g_v3f_one = (t_v3f){.x = 1.0f, .y = 1.0f, .z = 1.0f};
const t_v3f	g_v3f_foward = (t_v3f){.x = 0.0f, .y = 0.0f, .z = 1.0f};
const t_v3f	g_v3f_back = (t_v3f){.x = 0.0f, .y = 0.0f, .z = -1.0f};
const t_v3f	g_v3f_up = (t_v3f){.x = 0.0f, .y = 1.0f, .z = 0.0f};
const t_v3f	g_v3f_down = (t_v3f){.x = 0.0f, .y = -1.0f, .z = 0.0f};
const t_v3f	g_v3f_right = (t_v3f){.x = 1.0f, .y = 0.0f, .z = 0.0f};
const t_v3f	g_v3f_left = (t_v3f){.x = -1.0f, .y = 0.0f, .z = 0.0f};

inline t_v2f	init_v2f(float x, float y)
{
	return ((t_v2f){
		.x = x,
		.y = y
	});
}

inline t_v3f	init_v3f(float x, float y, float z)
{
	return ((t_v3f){
		.x = x,
		.y = y,
		.z = z
	});
}
