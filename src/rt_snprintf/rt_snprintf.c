/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 13:40:28 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/27 19:01:47 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <math.h>
#include "libft.h"

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <ieee754.h>

#define SIZE 1000

typedef struct s_str
{
	char	*s;
	size_t	len;
	size_t	cap;
}	t_str;

size_t	int_count(unsigned int n)
{
	size_t	count;

	count = (n == 0);
	while (n != 0)
	{
		n /= 10;
		++count;
	}
	return (count);
}

void	utos(t_str *str, unsigned int n, char prefix)
{
	size_t	count;
	char	*dst;

	count = int_count(n) + (prefix != '\0');
	while (str->len + count >= str->cap)
	{
		n /= 10;
		--count;
	}
	if (count == 0)
		return ;
	dst = str->s + (str->len + count - 1);
	if (n == 0)
		*dst-- = '0';
	while (n != 0)
	{
		*dst = '0' + (n % 10);
		n /= 10;
		--dst;
	}
	if (prefix != '\0')
		*dst = prefix;
	str->len += count;
}

void	itos(t_str *str, int n)
{
	if (n < 0)
		utos(str, -n, '-');
	else
		utos(str, n, '\0');
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

// TODO: pad missing precision
// TODO: need a solution for values like 0.0001f
// TODO: round off numbers at the precision
void	ftos(t_str *str, double r)
{
	const double			prec = 4.0;
	int						numeric;
	int						decimal;
	union ieee754_double	*d;

	d = (union ieee754_double *)&r;
	if (d->ieee.negative == 1)
		ctos(str, '-');
	if (isinf(r))
		stos(str, "inf");
	else if (isnan(r))
		stos(str, "nan");
	else if (d->d == 0.0f)
		stos(str, "0.0");
	else
	{
		numeric = (int)(r * -1.0f);
		decimal = (int)((round(r - floor(r))) * pow(10.0, prec));
		itos(str, numeric);
		ctos(str, '.');
		utos(str, decimal, '\0');
	}
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

void dump_write(char *s, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (s[i] == '\0')
			write(1, "+", 1);
		else
			write(1, &s[i], 1);
		++i;
	}
}

int	main(void)
{
	char	s[SIZE];

	ft_bzero(s, SIZE);
	// hello: 2147483647
	// hello: 2147483647|-1|-5|1231-
	printf("%i\n%i\n%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n\n", 0, INT_MAX, INT_MIN, 1.0f, .99f, 123.321f, -123.321f, 0.0f, 0.0001f, -0.001f);
	rt_snprintf(s, SIZE, "snprint:\n%i\n%i\n%i\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", 0, INT_MAX, INT_MIN, 1.0f, .99f, 123.321f, -123.321f, 0.0f, 0.0001f, -0.001f);
	// rt_snprintf(s, SIZE, "hello: \n");
	s[SIZE - 1] = '\0';
	dump_write(s, 1000);
	ft_putchar_fd('\n', 1);
	// printf("%s (%zu/%i)", s, ft_strlen(s), SIZE);

	return (0);
}
