/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 13:40:28 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 21:42:55 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "libft.h"
#include "rt_snprintf.h"

static void	null_terminate(t_str *str)
{
	if (str->len < str->cap)
		*(str->s + str->len) = '\0';
	else
		*(str->s + str->cap) = '\0';
}

static const char	*convert(t_str *str, const char *format, va_list args)
{
	if (*format == 'i' || *format == 'd')
		itos(str, va_arg(args, int));
	else if (*format == 'u')
		utos(str, va_arg(args, int), '\0');
	else if (*format == 's')
		stos(str, va_arg(args, char *));
	else if (*format == 'c')
		ctos(str, va_arg(args, int));
	else if (*format == '%')
		ctos(str, '%');
	else if (*format == 'f')
		ftos(str, va_arg(args, double));
	return (format + 1);
}

int	rt_snprintf(char *s, size_t size, const char *format, ...)
{
	va_list		args;
	t_str		str;
	size_t		copy_len;
	const char	*spc;

	if (format == NULL || size == 0)
		return (-1);
	str = (t_str){s, 0, size - 1};
	va_start(args, format);
	while (*format && str.len < str.cap)
	{
		spc = ft_strchrnul(format, '%');
		copy_len = (spc - format);
		if (str.len + copy_len > str.cap)
			copy_len = str.cap - str.len;
		ft_memmove(s + str.len, format, copy_len);
		str.len += copy_len;
		if (*spc != '\0' && str.len < str.cap)
			format = convert(&str, spc + 1, args);
		else
			format = spc;
	}
	va_end(args);
	null_terminate(&str);
	return (str.len);
}
