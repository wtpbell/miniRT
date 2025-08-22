/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/13 20:00:17 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/22 11:55:20 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_SNPRINTF_H
# define RT_SNPRINTF_H

# include <stddef.h>

typedef struct s_str
{
	char	*s;
	size_t	len;
	size_t	cap;
}	t_str;

size_t	int_count(unsigned int n);
void	stos(t_str *str, const char *src);
void	ctos(t_str *str, char c);
void	utos(t_str *str, unsigned int n, char prefix);
void	itos(t_str *str, int n);
void	ftos(t_str *str, double r);
int		rt_snprintf(char *s, size_t size, const char *format, ...);

#endif
