# linux-internals

The function dlsym() takes a “handle” of a dynamic library returned by dlopen() 
and the null-terminated symbol name, returning the address where that 
symbol is loaded into memory.

So inside the wrapper function we can use dlsym to get the address of the related 
symbol in memory and call the glibc function. Another approach can be calling the 
syscall directly, both approaches will work.

We need to make sure we are generating a position-independent code(PIC) by 
passing -fPIC that is shared -shared. We also need to link our library with 
Dynamically Loaded (DL) libraries -ldl, since we are using dlsym in our code.

Using LD_PRELOAD: There is a shell environment variable in Linux called LD_PRELOAD, 
which can be set to a path of a shared library, and that library will be loaded 
before any other library (including glibc).

Using ‘ld --wrap=symbol‘: This can be used to use a wrapper function for symbol. 
Any further reference to symbol will be resolved to the wrapper function.

gcc -fPIC -shared  -o bin/libpreload.so src/wrap-preload.c -ldl

Use gcc to pass the option to the linker with -Wl, which is equal to calling ld with --wrap option.

gcc -c src/wrap-link.c -o bin/wrap-link.o
gcc -c src/test.c -o bin/test-link.o
gcc -Wl,-wrap,write -Wl,-wrap=write -Wl,-wrap=puts bin/test-link.o bin/wrap-link.o -o bin/test-link-bin


Use a wrapper function for symbol. Any undefined reference to symbol will be resolved to “wrap_symbol”. 
Any undefined reference to “real_symbol” will be resolved to symbol.
This can be used to provide a wrapper for a system function. The wrapper function should 
be called “wrap_symbol”. If it wishes to call the system function, it should call “real_symbol”.

Here is a trivial example:

   void *
   __wrap_malloc (size_t c)
   {
    printf ("malloc called with %zu\n", c);
    return __real_malloc (c);
   }
   
If you link other code with this file using –wrap malloc, then all calls to “malloc” 
will call the function “wrap_malloc” instead. The call to “real_malloc” in “__wrap_malloc” 
will call the real “malloc” function.

You may wish to provide a “real_malloc” function as well, so that links without the –wrap 
option will succeed. If you do this, you should not put the definition of “real_malloc” 
in the same file as “__wrap_malloc”; if you do, the assembler may resolve the call before the 
linker has a chance to wrap it to “malloc”.
