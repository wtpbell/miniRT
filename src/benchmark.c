/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   benchmark.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/11 18:00:10 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/11 18:07:37 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <stdio.h>

static float g_starttime;

void start_time(void)
{
	g_starttime = (float)clock() / CLOCKS_PER_SEC;
}

void end_time(void)
{
	float	endtime;

	endtime = (float)clock() / CLOCKS_PER_SEC;
	printf("TIME: %f\n", endtime - g_starttime);
}
