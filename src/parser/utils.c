/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:48:36 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 14:48:36 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	token_count(char **tokens)
{
	size_t	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

bool	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool	parse_v3f(t_v3f *v3f, const char *str)
{
	char	**tokens;
	bool	result;

	if (!str || !v3f)
		return (false);
	printf("Parsing v3f from: '%s'\n", str);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	result = true;
	if (token_count(tokens) == 3)
	{
		result = (
			ft_stof(tokens[0], &v3f->x) &&
			ft_stof(tokens[1], &v3f->y) &&
			ft_stof(tokens[2], &v3f->z)
			);
		if (!result)
			printf("Error: Failed to parse v3f components\n");
	}
	else
	{
		printf("Error: Incorrect number of v3f components\n");
		result = false;
	}
	free_tokens(tokens);
	return (result);
}

bool	parse_float(float *f, const char *str)
{
	printf("Parsing float from: '%s'\n", str);
    
    if (!str || !f)
    {
        printf("Error: Invalid input for float parsing\n");
        return (false);
    }

    while (ft_isspace(*str))
        str++;

    if (!ft_stof(str, f))
    {
        printf("Error: Failed to parse float\n");
        return (false);
    }

    return (true);
}

bool	parse_col(t_col32 *col, const char *str)
{
	char **tokens;
    int rgb[3];
    int i;

    printf("Parsing color from: '%s'\n", str);

    if (!str || !col)
    {
        printf("Error: Invalid input for color parsing\n");
        return (false);
    }

    tokens = ft_split(str, ',');
    if (!tokens)
    {
        printf("Error: Failed to split color string\n");
        return (false);
    }

    if (token_count(tokens) != 3)
    {
        printf("Error: Incorrect number of color components\n");
        free_tokens(tokens);
        return (false);
    }

    for (i = 0; i < 3; i++)
    {
        // Trim whitespace from token
        char *trimmed = ft_strtrim(tokens[i], " \t\n\r");
        if (!trimmed)
        {
            printf("Error: Memory allocation failed for trimming\n");
            free_tokens(tokens);
            return (false);
        }

        if (!ft_stoi(trimmed, &rgb[i]))
        {
            printf("Error: Failed to parse color component '%s'\n", trimmed);
            free(trimmed);
            free_tokens(tokens);
            return (false);
        }
        free(trimmed);

        if (rgb[i] < 0 || rgb[i] > 255)
        {
            printf("Error: Color component %d out of range: %d\n", i, rgb[i]);
            free_tokens(tokens);
            return (false);
        }
    }

    *col = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF;
    free_tokens(tokens);
    return (true);
}
