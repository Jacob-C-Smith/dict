#pragma once

#include <stdio.h>
#include <stdlib.h>

struct dict_item_s;
struct dict_s;

struct dict_item_s {

	char* key;
	void* value;

	struct dict_item_s* next;
};

struct dict_s {
	struct dict_item_s  **entries;
	size_t                entry_count,
		                  n_entries;
};

typedef struct dict_item_s  dict_item;
typedef struct dict_s       dict;

// Allocaters
int        dict_create    ( dict **dictionary );

// Constructors
int        dict_construct ( dict **dictionary, size_t count );
int        dict_from_keys ( dict **dictionary, char      **keys,   size_t count );

// Accessors
dict_item *dict_get       ( dict  *dictionary, char       *name );
int        dict_items     ( dict  *dictionary, dict_item **items ); 
int        dict_values    ( dict  *dictionary, char      **values );
int        dict_keys      ( dict  *dictionary, char      **keys );

// Mutators
int        dict_add       ( dict  *dictionary, char       *name,   void  *val );
void      *dict_pop       ( dict  *dictionary, char       *name );
dict_item *dict_pop_item  ( dict  *dictionary, char       *name );

// Shallow copy
dict      *dict_copy      ( dict  *dictionary, dict      **target );

// Clear all items
dict      *dict_clear     ( dict  *dictionary );

// Destructors
int        dict_destroy   ( dict  *dictionary );