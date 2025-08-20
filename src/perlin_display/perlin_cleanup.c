/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:08:12 by bewong            #+#    #+#             */
/*   Updated: 2025/08/20 14:08:12 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

void	cleanup_display(t_pdisplay *display)
{
	if (display->ui)
		destroy_ui(display->ui);
	free(display->values);
	mlx_terminate(display->mlx);
}
