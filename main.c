/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** main
*/

#include <dlfcn.h>

int main()
{
    void * lib = dlopen("./libmy_malloc.so", RTLD_NOW);
    void (*hey)() = dlsym(lib, "malloc");
    
    hey();
    dlclose(lib);
    return (0);
}