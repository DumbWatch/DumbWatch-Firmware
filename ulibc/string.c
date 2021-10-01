/**
 * 
 */

#include <stdlib.h>

char* strncpy(char* dest, const char* src, size_t num)
{
    char* cpy_ptr = dest;
    while ((*src != '\0') && (num--))
    {
        *cpy_ptr++ = *src++;
    }

    for (; num > 0; num--)
        *cpy_ptr++ = '\0';

    return dest;
}