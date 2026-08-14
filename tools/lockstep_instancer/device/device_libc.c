// SPDX-License-Identifier: GPL-3.0-or-later
// Minimal device-side libc for the GPU firmware build.
//
// Everything the firmware's cold paths (CLI, blackbox-virtual, EEPROM
// glue) can reference must link; stdio is stubbed out entirely (the
// serial backend is already a sink, and EEPROM runs in RAM mode on the
// GPU). String/ctype/strtol are real implementations because config
// parsing runs through them. Compiled with -fno-builtin so the mem*
// definitions below don't recurse into themselves.

#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

// NOTE: ctype + strcasecmp/strcasestr are intentionally absent — the
// firmware's own common/string_light.c defines them on this target.

int errno;

// ---------------------------------------------------------------------
// mem* / str*
// ---------------------------------------------------------------------

void *memcpy(void *d, const void *s, size_t n)
{
    char *dp = d;
    const char *sp = s;
    while (n--) *dp++ = *sp++;
    return d;
}

void *memmove(void *d, const void *s, size_t n)
{
    char *dp = d;
    const char *sp = s;
    if (dp < sp) {
        while (n--) *dp++ = *sp++;
    } else {
        dp += n; sp += n;
        while (n--) *--dp = *--sp;
    }
    return d;
}

void *memset(void *d, int c, size_t n)
{
    char *dp = d;
    while (n--) *dp++ = (char)c;
    return d;
}

int memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *pa = a, *pb = b;
    for (; n--; pa++, pb++) {
        if (*pa != *pb) return *pa - *pb;
    }
    return 0;
}

int bcmp(const void *a, const void *b, size_t n)
{
    return memcmp(a, b, n);
}

void bzero(void *d, size_t n)
{
    memset(d, 0, n);
}

void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *p = s;
    for (; n--; p++) {
        if (*p == (unsigned char)c) return (void *)p;
    }
    return 0;
}

size_t strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

size_t strnlen(const char *s, size_t n)
{
    size_t i = 0;
    while (i < n && s[i]) i++;
    return i;
}

char *strcpy(char *d, const char *s)
{
    char *r = d;
    while ((*d++ = *s++)) {}
    return r;
}

char *strncpy(char *d, const char *s, size_t n)
{
    char *r = d;
    while (n && (*d = *s)) { d++; s++; n--; }
    while (n--) *d++ = 0;
    return r;
}

char *strcat(char *d, const char *s)
{
    char *r = d;
    while (*d) d++;
    while ((*d++ = *s++)) {}
    return r;
}

char *strncat(char *d, const char *s, size_t n)
{
    char *r = d;
    while (*d) d++;
    while (n-- && *s) *d++ = *s++;
    *d = 0;
    return r;
}

int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, size_t n)
{
    while (n && *a && *a == *b) { a++; b++; n--; }
    return n ? (unsigned char)*a - (unsigned char)*b : 0;
}

static int lowerc(int c)
{
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

char *strchr(const char *s, int c)
{
    for (;; s++) {
        if (*s == (char)c) return (char *)s;
        if (!*s) return 0;
    }
}

char *strrchr(const char *s, int c)
{
    const char *r = 0;
    for (;; s++) {
        if (*s == (char)c) r = s;
        if (!*s) return (char *)r;
    }
}

char *strstr(const char *h, const char *n)
{
    if (!*n) return (char *)h;
    for (; *h; h++) {
        const char *a = h, *b = n;
        while (*a && *b && *a == *b) { a++; b++; }
        if (!*b) return (char *)h;
    }
    return 0;
}

size_t strspn(const char *s, const char *accept)
{
    size_t i = 0;
    while (s[i] && strchr(accept, s[i])) i++;
    return i;
}

size_t strcspn(const char *s, const char *reject)
{
    size_t i = 0;
    while (s[i] && !strchr(reject, s[i])) i++;
    return i;
}

char *strpbrk(const char *s, const char *accept)
{
    for (; *s; s++) {
        if (strchr(accept, *s)) return (char *)s;
    }
    return 0;
}

char *strtok_r(char *s, const char *delim, char **save)
{
    if (!s) s = *save;
    s += strspn(s, delim);
    if (!*s) { *save = s; return 0; }
    char *tok = s;
    s += strcspn(s, delim);
    if (*s) *s++ = 0;
    *save = s;
    return tok;
}

char *strtok(char *s, const char *delim)
{
    static char *save;
    return strtok_r(s, delim, &save);
}

char *strsep(char **sp, const char *delim)
{
    char *s = *sp;
    if (!s) return 0;
    char *end = s + strcspn(s, delim);
    if (*end) { *end = 0; *sp = end + 1; } else { *sp = 0; }
    return s;
}

char *strerror(int e)
{
    (void)e;
    return (char *)"(gpu)";
}

int ffs(int v)
{
    return v ? __builtin_ctz((unsigned)v) + 1 : 0;
}

int pthread_mutex_init(int *m, const void *attr) { (void)m; (void)attr; return 0; }
int pthread_mutex_lock(int *m) { (void)m; return 0; }
int pthread_mutex_trylock(int *m) { (void)m; return 0; }
int pthread_mutex_unlock(int *m) { (void)m; return 0; }
int pthread_mutex_destroy(int *m) { (void)m; return 0; }

// ---------------------------------------------------------------------
// ctype
// ---------------------------------------------------------------------

int isalpha(int c) { return isupper(c) || islower(c); }
int isxdigit(int c) { return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
int isprint(int c) { return c >= 0x20 && c < 0x7f; }
int iscntrl(int c) { return (c >= 0 && c < 0x20) || c == 0x7f; }
int isgraph(int c) { return c > 0x20 && c < 0x7f; }
int ispunct(int c) { return isgraph(c) && !isalnum(c); }
int isblank(int c) { return c == ' ' || c == '\t'; }

// ---------------------------------------------------------------------
// strtol family (errno-free; config/CLI parsing only)
// ---------------------------------------------------------------------

static unsigned long long strtoull_impl(const char *s, char **end, int base, int *neg)
{
    while (isspace((unsigned char)*s)) s++;
    *neg = 0;
    if (*s == '+' || *s == '-') {
        *neg = (*s == '-');
        s++;
    }
    if ((base == 0 || base == 16) && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
        base = 16;
    } else if (base == 0) {
        base = (s[0] == '0') ? 8 : 10;
    }
    unsigned long long v = 0;
    const char *start = s;
    for (;; s++) {
        int d;
        if (isdigit((unsigned char)*s)) d = *s - '0';
        else if (isalpha((unsigned char)*s)) d = lowerc((unsigned char)*s) - 'a' + 10;
        else break;
        if (d >= base) break;
        v = v * (unsigned)base + (unsigned)d;
    }
    if (end) *end = (char *)(s == start ? start : s);
    return v;
}

// strtol/strtoul/atoi intentionally absent: common/strtol.c provides them

long long strtoll(const char *s, char **end, int base)
{
    int neg;
    unsigned long long v = strtoull_impl(s, end, base, &neg);
    return neg ? -(long long)v : (long long)v;
}

unsigned long long strtoull(const char *s, char **end, int base)
{
    int neg;
    unsigned long long v = strtoull_impl(s, end, base, &neg);
    return neg ? (unsigned long long)-(long long)v : v;
}

long atol(const char *s)
{
    return strtol(s, 0, 10);
}

// ---------------------------------------------------------------------
// stdio: pure sinks. Serial output already goes to a sink backend and
// EEPROM runs in RAM mode, so nothing of consequence flows through here.
// ---------------------------------------------------------------------

typedef struct __bfFILE { int dummy; } FILE;
static FILE bfStdin, bfStdout, bfStderr;
FILE *stdin = &bfStdin;
FILE *stdout = &bfStdout;
FILE *stderr = &bfStderr;

int printf(const char *fmt, ...) { (void)fmt; return 0; }
int fprintf(FILE *f, const char *fmt, ...) { (void)f; (void)fmt; return 0; }
int vprintf(const char *fmt, va_list ap) { (void)fmt; (void)ap; return 0; }
int vfprintf(FILE *f, const char *fmt, va_list ap) { (void)f; (void)fmt; (void)ap; return 0; }
int puts(const char *s) { (void)s; return 0; }
int putchar(int c) { return c; }
int fputc(int c, FILE *f) { (void)f; return c; }
int fputs(const char *s, FILE *f) { (void)s; (void)f; return 0; }
int fgetc(FILE *f) { (void)f; return -1; }
char *fgets(char *s, int n, FILE *f) { (void)s; (void)n; (void)f; return 0; }
FILE *fopen(const char *path, const char *mode) { (void)path; (void)mode; return 0; }
int fclose(FILE *f) { (void)f; return 0; }
size_t fread(void *p, size_t sz, size_t n, FILE *f) { (void)p; (void)sz; (void)n; (void)f; return 0; }
size_t fwrite(const void *p, size_t sz, size_t n, FILE *f) { (void)p; (void)sz; (void)n; (void)f; return 0; }
int fseek(FILE *f, long off, int whence) { (void)f; (void)off; (void)whence; return -1; }
long ftell(FILE *f) { (void)f; return -1; }
void rewind(FILE *f) { (void)f; }
int fflush(FILE *f) { (void)f; return 0; }
int feof(FILE *f) { (void)f; return 1; }
int ferror(FILE *f) { (void)f; return 0; }
void perror(const char *s) { (void)s; }
int remove(const char *path) { (void)path; return -1; }
int rename(const char *o, const char *n) { (void)o; (void)n; return -1; }

// vsnprintf: real (if simple) formatter — blackbox and CLI build header
// strings with it. Unknown conversions render as '?'.
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    size_t w = 0;
#define PUT(ch) do { if (w + 1 < n) out[w] = (ch); w++; } while (0)
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            PUT(*fmt);
            continue;
        }
        fmt++;
        // flags/width/precision: parse and ignore (cold path formatting)
        while (*fmt == '-' || *fmt == '+' || *fmt == ' ' || *fmt == '#' || *fmt == '0') fmt++;
        while (isdigit((unsigned char)*fmt)) fmt++;
        if (*fmt == '.') { fmt++; while (isdigit((unsigned char)*fmt)) fmt++; }
        int l = 0;
        while (*fmt == 'l') { l++; fmt++; }
        if (*fmt == 'z' || *fmt == 't') { l = 2; fmt++; }
        char buf[24];
        switch (*fmt) {
        case '%': PUT('%'); break;
        case 'c': PUT((char)va_arg(ap, int)); break;
        case 's': {
            const char *s = va_arg(ap, const char *);
            if (!s) s = "(null)";
            while (*s) PUT(*s++);
            break;
        }
        case 'd': case 'i': {
            long long v = (l >= 2) ? va_arg(ap, long long)
                        : (l == 1) ? va_arg(ap, long) : va_arg(ap, int);
            unsigned long long u = (v < 0) ? (unsigned long long)-v : (unsigned long long)v;
            int i = 0;
            do { buf[i++] = (char)('0' + (u % 10)); u /= 10; } while (u);
            if (v < 0) PUT('-');
            while (i) PUT(buf[--i]);
            break;
        }
        case 'u': case 'x': case 'X': {
            unsigned long long u = (l >= 2) ? va_arg(ap, unsigned long long)
                                 : (l == 1) ? va_arg(ap, unsigned long) : va_arg(ap, unsigned);
            const unsigned base = (*fmt == 'u') ? 10 : 16;
            const char *digits = (*fmt == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
            int i = 0;
            do { buf[i++] = digits[u % base]; u /= base; } while (u);
            while (i) PUT(buf[--i]);
            break;
        }
        case 'p': {
            unsigned long long u = (unsigned long long)(uintptr_t)va_arg(ap, void *);
            PUT('0'); PUT('x');
            int i = 0;
            do { buf[i++] = "0123456789abcdef"[u % 16]; u /= 16; } while (u);
            while (i) PUT(buf[--i]);
            break;
        }
        case 'f': case 'g': case 'e': {
            // fixed 3-decimal rendering; enough for cold-path logs
            double v = va_arg(ap, double);
            if (v < 0) { PUT('-'); v = -v; }
            unsigned long long ip = (unsigned long long)v;
            unsigned frac = (unsigned)((v - (double)ip) * 1000.0 + 0.5);
            if (frac >= 1000) { ip++; frac -= 1000; }
            int i = 0;
            do { buf[i++] = (char)('0' + (ip % 10)); ip /= 10; } while (ip);
            while (i) PUT(buf[--i]);
            PUT('.');
            PUT((char)('0' + frac / 100));
            PUT((char)('0' + (frac / 10) % 10));
            PUT((char)('0' + frac % 10));
            break;
        }
        default: PUT('?'); break;
        }
    }
    if (n) out[w < n ? w : n - 1] = 0;
    return (int)w;
#undef PUT
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int r = vsnprintf(out, n, fmt, ap);
    va_end(ap);
    return r;
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
    return vsnprintf(out, (size_t)1 << 30, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int r = vsprintf(out, fmt, ap);
    va_end(ap);
    return r;
}

// ---------------------------------------------------------------------
// misc
// ---------------------------------------------------------------------

typedef struct __bfDIR { int dummy; } DIR;
struct dirent { unsigned long d_ino; char d_name[256]; };
DIR *opendir(const char *name) { (void)name; return 0; }
struct dirent *readdir(DIR *d) { (void)d; return 0; }
int closedir(DIR *d) { (void)d; return 0; }

void exit(int code)
{
    (void)code;
    __builtin_trap();
}

void abort(void)
{
    __builtin_trap();
}

int abs(int v) { return v < 0 ? -v : v; }
long labs(long v) { return v < 0 ? -v : v; }
long long llabs(long long v) { return v < 0 ? -v : v; }
char *getenv(const char *name) { (void)name; return 0; }
int usleep(unsigned usec) { (void)usec; return 0; }
unsigned sleep(unsigned sec) { (void)sec; return 0; }

