#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

static  ssize_t (*real_write)(int fd, const void *buf, size_t count) = NULL;
static int (*real_puts)(const char* str) = NULL;

ssize_t write(int fd, const void *buf, size_t count)
{
    printf("write:chars#:%lu\n", count);
    /* reslove the real write function from glibc
     * and pass the arguments.
     */
    real_write = dlsym(RTLD_NEXT, "write");
    real_write(fd, buf, count);

}

int puts(const char* str)
{
    printf("puts:chars#:%lu\n", strlen(str));
    /* resolve the real puts function from glibc
     * and pass the arguments.
     */
    real_puts = dlsym(RTLD_NEXT, "puts");
    real_puts(str);
}