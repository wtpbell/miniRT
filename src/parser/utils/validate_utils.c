/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:55:49 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 18:55:49 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	validate_and_norm_dir(t_v3f *dir, const char *str)
{
	if (!v3f_dir_valid(dir))
	{
		print_error(ERR_RANGE, "direction range -1.0f - 1.0f", str);
		return (false);
	}
	*dir = v3f_norm(*dir);
	return (true);
}
