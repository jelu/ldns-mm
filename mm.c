#include <ldns/mm.h>
#include <ldns/util.h>

#include <string.h>
#include <pthread.h>

void *__ldns_malloc2(size_t size, volatile const char *file, volatile int line)
{
	return malloc(size);
}

void *__ldns_calloc2(size_t nmemb, size_t size, volatile const char *file, volatile int line)
{
	return calloc(nmemb, size);
}

void *__ldns_realloc2(void *ptr, size_t size, volatile const char *file, volatile int line)
{
	return realloc(ptr, size);
}

void __ldns_free2(void *ptr, volatile const char *file, volatile int line)
{
	free(ptr);
}

char *__ldns_strdup2(const char *ptr, volatile const char *file, volatile int line)
{
	return strdup(ptr);
}

////

ldns_malloc2_t __ldns_mm_malloc2 = __ldns_malloc2;
ldns_calloc2_t __ldns_mm_calloc2 = __ldns_calloc2;
ldns_realloc2_t __ldns_mm_realloc2 = __ldns_realloc2;
ldns_free2_t __ldns_mm_free2 = __ldns_free2;
ldns_strdup2_t __ldns_mm_strdup2 = __ldns_strdup2;

////

void ldns_mm_set_malloc(ldns_malloc_t func)
{
}

void ldns_mm_set_calloc(ldns_calloc_t func)
{
}

void ldns_mm_set_realloc(ldns_realloc_t func)
{
}

void ldns_mm_set_free(ldns_free_t func)
{
}

void ldns_mm_set_strdup(ldns_strdup_t func)
{
}

////

void ldns_mm_set_malloc2(ldns_malloc2_t func)
{
	__ldns_mm_malloc2 = func;
}

void ldns_mm_set_calloc2(ldns_calloc2_t func)
{
	__ldns_mm_calloc2 = func;
}

void ldns_mm_set_realloc2(ldns_realloc2_t func)
{
	__ldns_mm_realloc2 = func;
}

void ldns_mm_set_free2(ldns_free2_t func)
{
	__ldns_mm_free2 = func;
}

void ldns_mm_set_strdup2(ldns_strdup2_t func)
{
	__ldns_mm_strdup2 = func;
}

////

void *__ldns_malloc(size_t size, volatile const char *file, volatile int line)
{
	return __ldns_mm_malloc2(size, file, line);
}

void *__ldns_calloc(size_t nmemb, size_t size, volatile const char *file, volatile int line)
{
	return __ldns_mm_calloc2(nmemb, size, file, line);
}

void *__ldns_realloc(void *ptr, size_t size, volatile const char *file, volatile int line)
{
	return __ldns_mm_realloc2(ptr, size, file, line);
}

void __ldns_free(void *ptr, volatile const char *file, volatile int line)
{
	__ldns_mm_free2(ptr, file, line);
}

char *__ldns_strdup(const char *ptr, volatile const char *file, volatile int line)
{
	return __ldns_mm_strdup2(ptr, file, line);
}

#define __ldns_mm_alloc_size 65536

pthread_mutex_t __ldns_mm_memory_lock = PTHREAD_MUTEX_INITIALIZER;

void *ldns_mm_alloc_new(ldns_mm_alloc_t *alloc)
{
	void *ptr = 0L;

	if (alloc == 0L) {
		return 0L;
	}

	if (pthread_mutex_lock(&__ldns_mm_memory_lock)) {
		return 0L;
	}

	if (!alloc->next) {
		unsigned int i;
		void *batch;

		if (!(batch = LDNS_MALLOC2(__ldns_mm_alloc_size))) {
			exit(-1);
		}

		for (i=0; i<(__ldns_mm_alloc_size / alloc->size); i++) {
			*(void **)batch = alloc->next;
			alloc->next = batch;
			batch = ((char *)batch + alloc->size);
		}
	}

	ptr = alloc->next;
	alloc->next = *(void **)ptr;
	*(void **)ptr = 0L;

	pthread_mutex_unlock(&__ldns_mm_memory_lock);
	return ptr;
}

void ldns_mm_alloc_delete(ldns_mm_alloc_t *alloc, void *ptr)
{
	if (alloc == 0L || ptr == 0L) {
		return;
	}

	if (pthread_mutex_lock(&__ldns_mm_memory_lock)) {
		return;
	}

	*(void **)ptr = alloc->next;
	alloc->next = ptr;

	pthread_mutex_unlock(&__ldns_mm_memory_lock);
}
