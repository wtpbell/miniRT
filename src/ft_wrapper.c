/**
 * @file ft_wrapper.c
 * @author josejboon
 * @brief Mock (wrap) function calls. (man ld)
 * Compile using the -Wl,--wrap= flag (comma separate the --wraps for more).
 * gcc -Wl,--wrap=malloc,--wrap=write main.c ft_wrapper.c libft.a
 * === Alternatively use macros ===
 * #define function_call(args) wrapper_function_call(args)
 * #define malloc(size) wrapper_malloc(size)
 * === ------------------------ ===
 *
 * @version 0.1
 * @date 2024-12-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int	__malloc_count = 700;

void *__real_malloc(size_t size);

void *__wrap_malloc(size_t size)
{
	if (__malloc_count-- > 0)
		return (__real_malloc(size));
	errno = 12;
	return (NULL);
}

void	*__real_calloc(size_t elements, size_t size);

void	*__wrap_calloc(size_t elements, size_t size)
{
	if (__malloc_count-- > 0)
		return (__real_calloc(elements, size));
	errno = 12;
	return (NULL);
}
