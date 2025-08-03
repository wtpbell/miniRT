/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_strtok.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/03 12:06:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/03 13:14:10 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*rt_strtok(char *str, const char *delim, char **saveptr)
{
	char	*start;

	if (str == NULL)
		str = *saveptr;
	while (*str && ft_strchr(delim, *str))
		++str;
	if (*str == '\0')
		return (NULL);
	start = str;
	while (*str && !ft_strchr(delim, *str))
		++str;
	*saveptr = str + (*str != '\0');
	*str = '\0';
	return (start);
}

size_t	rt_count_occ(const char *str, char c)
{
	size_t	occ;

	occ = 0;
	while (*str)
	{
		if (*str == c)
			++occ;
		++str;
	}
	return (occ);
}

// int	main(int argc, char *argv[])
// {
// 	char	*token;
// 	char	*subtoken;
// 	char	*sp1;
// 	char	*sp2;
// 	char	*org;
// 	size_t	len;

// 	if (argc < 3)
// 		return (1);
// 	org = argv[1];
// 	len = strlen(org);
// 	while (1)
// 	{
// 		token = rt_strtok(argv[1], argv[2], &sp1);
// 		if (token == NULL)
// 			break ;
// 		printf("+ %s\n", token);
// 		while (1)
// 		{
// 			subtoken = rt_strtok(token, argv[3], &sp2);
// 			if (subtoken == NULL)
// 				break ;
// 			printf("  - %s\n", subtoken);
// 			token = NULL;
// 		}
// 		argv[1] = NULL;
// 		printf("remaining: %s\n", sp1);
// 	}
// 	write(1, org, len);
// 	return (0);
// }
