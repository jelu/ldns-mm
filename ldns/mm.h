#ifndef LDNS_MM_H
#define LDNS_MM_H

#include <stdlib.h>

typedef void *(*ldns_malloc_t)(size_t);
typedef void *(*ldns_calloc_t)(size_t, size_t);
typedef void *(*ldns_realloc_t)(void *, size_t);
typedef void (*ldns_free_t)(void *);
typedef char *(*ldns_strdup_t)(const char *);

void ldns_mm_set_malloc(ldns_malloc_t func);
void ldns_mm_set_calloc(ldns_calloc_t func);
void ldns_mm_set_realloc(ldns_realloc_t func);
void ldns_mm_set_free(ldns_free_t func);
void ldns_mm_set_strdup(ldns_strdup_t func);

typedef void *(*ldns_malloc2_t)(size_t, volatile const char *, volatile int);
typedef void *(*ldns_calloc2_t)(size_t, size_t, volatile const char *, volatile int);
typedef void *(*ldns_realloc2_t)(void *, size_t, volatile const char *, volatile int);
typedef void (*ldns_free2_t)(void *, volatile const char *, volatile int);
typedef char *(*ldns_strdup2_t)(const char *, volatile const char *, volatile int);

void ldns_mm_set_malloc2(ldns_malloc2_t func);
void ldns_mm_set_calloc2(ldns_calloc2_t func);
void ldns_mm_set_realloc2(ldns_realloc2_t func);
void ldns_mm_set_free2(ldns_free2_t func);
void ldns_mm_set_strdup2(ldns_strdup2_t func);

extern ldns_malloc2_t __ldns_mm_malloc2;
extern ldns_calloc2_t __ldns_mm_calloc2;
extern ldns_realloc2_t __ldns_mm_realloc2;
extern ldns_free2_t __ldns_mm_free2;
extern ldns_strdup2_t __ldns_mm_strdup2;

void *__ldns_malloc(size_t size, volatile const char *file, volatile int line);
void *__ldns_calloc(size_t nmemb, size_t size, volatile const char *file, volatile int line);
void *__ldns_realloc(void *ptr, size_t size, volatile const char *file, volatile int line);
void __ldns_free(void *ptr, volatile const char *file, volatile int line);
char *__ldns_strdup(const char *ptr, volatile const char *file, volatile int line);

#undef LDNS_MM_MEMORY_ALLOC

struct ldns_mm_alloc_struct
{
	void *next;
	size_t size;
};
typedef struct ldns_mm_alloc_struct ldns_mm_alloc_t;

#define LDNS_MM_ALLOC_T_STATIC_NEW(x) { 0L, x }

void *ldns_mm_alloc_new(ldns_mm_alloc_t *alloc);
void ldns_mm_alloc_delete(ldns_mm_alloc_t *alloc, void *);

#endif
