#include <stdio.h>
#include <string.h>

ssize_t __real_write(int fd, const void *buf, size_t count);
int __real_puts(const char* str);

ssize_t __wrap_write (int fd, const void *buf, size_t count)
{
    printf("write:chars#:%lu\n", count);

    /* call the real glibc function and return the result */
    ssize_t result = __real_write(fd, buf, count);
    return result;
}

/* wrapping puts function */
int __wrap_puts (const char* str)
{
    printf("puts:chars#:%lu\n", strlen(str));

    /* call the real glibc function and return the result */
    int result = __real_puts(str);
    return result;
}