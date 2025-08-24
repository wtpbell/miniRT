/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_exit.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 13:27:46 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/24 13:56:51 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "MLX42/MLX42.h"
#include "rt_error.h"

int	rt_exit(void)
{
	if (errno == 0 && mlx_errno == MLX_SUCCESS)
		return (EXIT_SUCCESS);
	if (mlx_errno != MLX_SUCCESS)
		rt_mlx_error("MLX - miniRT");
	else if (errno != 0)
		sys_error("SYS - miniRT");
	return (EXIT_FAILURE);
}
