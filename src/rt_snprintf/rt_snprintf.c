/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 13:40:28 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 19:20:36 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <math.h>
#include "libft.h"

#include <stdio.h>
#include <limits.h>

#define SIZE 1000

typedef struct s_str
{
	char	*s;
	size_t	len;
	size_t	cap;
}	t_str;

int	int_count(int n)
{
	int	count;

	count = (n <= 0);
	while (n != 0)
	{
		n /= 10;
		++count;
	}
	return (count);
}

void	itos(t_str *str, int n)
{
	size_t			count;
	char			*dst;
	unsigned int	num;

	num = n;
	count = int_count(n);
	if (n < 0)
		num *= -1;
	while (str->len + count >= str->cap)
	{
		num /= 10;
		--count;
	}
	dst = str->s + (str->len + count - 1);
	if (count == 0)
		return ;
	while (num != 0)
	{
		*dst = '0' + (num % 10);
		num /= 10;
		--dst;
	}
	if (n < 0 )
		*dst = '-';
	else if (n == 0)
		*dst = '0';
	str->len += count;
}

void	stos(t_str *str, const char *src)
{
	size_t	len;

	len = ft_strlen(src);
	if (str->len + len >= str->cap)
		len = str->cap - str->len;
	ft_memmove(str->s + str->len, src, len);
	str->len += len;
}

void	ctos(t_str *str, char c)
{
	if (str->len + 1 >= str->cap)
		return ;
	*(str->s + str->len) = c;
	str->len += 1;
}

void	ftos(t_str *str, double f)
{
	const double	prec = 4.0;
	int				numeric;
	int				decimal;

	// TODO: handle differently
	// if (isinf(f) || isnan(f))
	// 	return ;

	numeric = (int)f;
	decimal = (int)((f - numeric) * pow(10.0, prec));

	printf("(%i %i)\n", numeric, decimal);
	itos(str, numeric);
	ctos(str, '.');
	itos(str, decimal);
}

const char	*process(t_str *str, const char *format, va_list args)
{
	if (*format == 'i' || *format == 'd')
		itos(str, va_arg(args, int));
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
	const char	*ptr;

	if (format == NULL)
		return (-1);
	str = (t_str) {s, 0, size};
	va_start(args, format);
	while (*format && str.len < str.cap)
	{
		ptr = ft_strchrnul(format, '%');
		if (str.len + (ptr - format) < str.cap)
		{
			ft_memmove(s  + str.len, format, ptr - format);
			str.len += (ptr - format);
		}
		else
		{
			ft_memmove(s + str.len, format, str.cap - str.len);
			str.len = str.cap;
		}
		if (*ptr != '\0' && str.len < str.cap)
			format = process(&str, ptr + 1, args);
		else
			format = ptr;
	}
	va_end(args);
	if (str.len < str.cap)
		*(str.s + str.len) = '\0';
	return (str.len);
}

int	main(void)
{
	char	s[SIZE];

	ft_bzero(s, SIZE);
	// hello: 2147483647
	// hello: 2147483647|-1|-5|1231-
	rt_snprintf(s, SIZE, "%i\n%i\n%i\n%f\n%f\n%f\n%f\n", 0, INT_MAX, INT_MIN, 1.0f, .99f, 123.321f, -123.321f);
	// rt_snprintf(s, SIZE, "hello: \n");
	s[SIZE - 1] = '\0';
	printf("%s (%zu/%i)", s, ft_strlen(s), SIZE);
	return (0);
}
