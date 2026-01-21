#include <stdio.h>   // printf
#include <stddef.h>  // size_t

static int xy_is_n_flag(const char *s)
{
    size_t i;

    if (!s || s[0] != '-' || s[1] != 'n')
        return 0;

    i = 2;
    while (s[i] == 'n')
        i++;

    return (s[i] == '\0');
}

size_t xy_strlen(const char *s)
{
    size_t i;

    if (!s)
        return 0;

    i = 0;
    while (s[i] != '\0')
        i++;

    return i;
}

int main(void)
{
    printf("\n");

    printf("%d\n", xy_is_n_flag(""));
    printf("%d\n", xy_is_n_flag(" "));
    printf("%d\n", xy_is_n_flag("-n"));
    printf("%d\n", xy_is_n_flag("-nn"));
    printf("%d\n", xy_is_n_flag("-nnn"));
    printf("%d\n", xy_is_n_flag("-nnnnnnn"));
    printf("%d\n", xy_is_n_flag("-n nnnnnn"));
    printf("%d\n", xy_is_n_flag("-n nn nnnn"));
    printf("%d\n", xy_is_n_flag("-e"));
    printf("%d\n", xy_is_n_flag("e"));
    printf("%d\n", xy_is_n_flag("n-"));
    printf("%d\n", xy_is_n_flag("-ne"));

    return 0;
}