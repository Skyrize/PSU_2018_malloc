/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** main
*/

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

int main()
{
    void * lib = dlopen("./libmy_malloc.so", RTLD_NOW);
    void *(*_malloc)() = dlsym(lib, "malloc");
    char *test;
    char *test1;
    char *test2;

    test = _malloc(sizeof(char) * 8);
    test1 = _malloc(sizeof(char) * 8);
    test2 = _malloc(sizeof(char) * 8);
    strcpy(test, "Haloha");
    strcpy(test1, "Good");
    strcpy(test2, "Booh!");
    printf("%s\n", test);
    printf("%s\n", test1);
    printf("%s\n", test2);
    dlclose(lib);
    return (0);
}