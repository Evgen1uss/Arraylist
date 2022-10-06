
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "arraylist.h"


#if INTERFACE

struct arraylist {
	unsigned int size; // Count of items currently in list
	unsigned int capacity; // Allocated memory size, in items
	void** body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};


#define arraylist_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])

#endif


#define ARRAYLIST_INITIAL_CAPACITY 4


#define arraylist_memshift(s, offset, length) memmove((s) + (offset), (s), (length)* sizeof(s));


arraylist* arraylist_create()
{
	arraylist* new_list = malloc(sizeof(arraylist));
	new_list->size = 0;
	
	new_list->body = malloc(sizeof(void*) * ARRAYLIST_INITIAL_CAPACITY);
	assert(new_list->body);
	new_list->capacity = ARRAYLIST_INITIAL_CAPACITY;
	return new_list;
}


void arraylist_allocate(arraylist* l, unsigned int size)
{
	assert(size > 0);
	if (size > l->capacity) {
		unsigned int new_capacity = l->capacity;
		while (new_capacity < size) {
			new_capacity *= 2;
		}
		l->body = realloc(l->body, sizeof(void*) * new_capacity);
		assert(l->body);
		l->capacity = new_capacity;
	}
}


extern inline unsigned int arraylist_size(arraylist*l) {
	return l->size;
}

/**
 * Add item at the end of the list.
 */
void arraylist_add(arraylist* l, void* item)
{
	arraylist_allocate(l, l->size + 1);
	l->body[l->size++] = item;
}

/**
 * Pop (remove and return) an item off the end of the list.
 */
void* arraylist_pop(arraylist* l)
{
	assert(l->size > 0);
	return l->body[--l->size];
}

/**
 * Return item located at index.
 */
void* arraylist_get(arraylist* l, unsigned int index)
{
	assert(index < l->size);
	return l->body[index];
}

/**
 * Replace item at index with given value.
 */
void arraylist_set(arraylist* l, unsigned int index, void* value)
{
	assert(index < l->size);
	l->body[index] = value;
}

/**
 * Insert item at index, shifting the following items by one spot.
 */
void arraylist_insert(arraylist* l, unsigned int index, void* value)
{
	
	arraylist_allocate(l, l->size + 1);


	arraylist_memshift(l->body + index, 1, l->size - index);
	l->body[index] = value;
	l->size++;
}

/**
 * Remove the item at index, shifting the following items back by one spot.
 */
void* arraylist_remove(arraylist* l, unsigned int index)
{
	void* value = l->body[index];
	arraylist_memshift(l->body + index + 1, -1, l->size - index);
	l->size--;
	return value;
}

/**
 * Clear list of all items.
 */
void arraylist_clear(arraylist* l)
{
	l->size = 0;
}

void arraylist_destroy(arraylist* l)
{
	free(l->body);
	free(l);
}

