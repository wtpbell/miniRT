/**
 * @file ft_wrapper.c
 * @author josejboon
 * @brief Mock (wrap) function calls.
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

int	__malloc_count = 700;
int __write_count = 10;

void *__real_malloc(size_t size);

void *__wrap_malloc(size_t size)
{
	if (__malloc_count-- > 0)
	{
		// printf("%d allocations left\n", __malloc_count);
		return (__real_malloc(size));
	}
	dprintf(STDERR_FILENO, "Malloc: denied %zu bytes\n", size);
	return (NULL);
}

void	*__real_calloc(size_t n, size_t a);

void	*__wrap_calloc(size_t n, size_t a)
{
	if (__malloc_count-- > 0)
	{
		// printf("%d allocations left\n", __malloc_count);
		return (__real_calloc(n, a));
	}
	dprintf(STDERR_FILENO, "Calloc: denied %zu bytes\n", n * a);
	return (NULL);
}

// ssize_t __real_write(int fd, void *buf, size_t n);

// ssize_t __wrap_write(int fd, void *buf, size_t n)
// {
// 	if(__write_count-- > 0)
// 		return (__real_write(fd, buf, n));
// 	return (-1);
// }
