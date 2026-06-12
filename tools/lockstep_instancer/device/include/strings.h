#pragma once
#include <stddef.h>
int strcasecmp(const char *a, const char *b);
int strncasecmp(const char *a, const char *b, size_t n);
int bcmp(const void *a, const void *b, size_t n);
void bzero(void *d, size_t n);
int ffs(int v);
