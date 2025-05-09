/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:21:05 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/09 11:57:52 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libft.h"
#include "container.h"

void my_free(void *item)
{
	(void)item;
}

int	main(void)
{
	t_vector	vec;

	vector_init(&vec, 10);
	vector_add(&vec, ft_strdup("Hello"));
	vector_add(&vec, ft_strdup("BYE"));
	vector_add(&vec, ft_strdup("Hello"));

	for (int i = 0; i < vec.size; ++i)
	{
		printf("%s", (char *)vector_get(&vec, i));
	}
	printf("\n");
	vector_rm(&vec, 1, free);

	for (int i = 0; i < vec.size; ++i)
	{
		printf("%s", (char *)vector_get(&vec, i));
	}

	vector_free(&vec, free);
	return (0);
}
