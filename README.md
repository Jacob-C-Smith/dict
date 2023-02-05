# Dictionary
A very simple dictionary implementation written in C. Loosely inspired by Python dictionary.

## Example code
```c
#include <stdio.h>
#include <stdlib.h>

#include <dict/dict.h>

int print_dict_item ( dict_item *item );

int main(int argc, const char* argv[])
{
	// Constant data
	char* keys[] = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };

	// Initialized data
	dict *d      = (void *) 0;

	// Make a dictionary with 4 hash table items
	dict_construct(&d, 4);

	// Insert some items
	dict_add(d, "Red"  , 0xff0000);
	dict_add(d, "Green", 0x00ff00);
	dict_add(d, "Blue" , 0x0000ff);

	// Print out "Blue" and its value
	print_dict_item(dict_get(d, "Blue"));

	// Copy the dictionary
	dict_copy(d, &d_copy);

	// Destroy the dictionary
	dict_destroy(d);

	// Create a dictionary from the keys 
	dict_from_keys(&d, &keys, 10);

	// Allocate for items
	dict_item **items  = calloc(dict_items(d, 0) + 1, sizeof(void*));

	// Update items "A", "B", and "C"
	dict_add(d, "A", 0x1234);
	dict_add(d, "B", 0x5678);
	dict_add(d, "C", 0x9ABC);

	// Write items to item list
	dict_items(d, items);

	// Print items
	for (size_t i = 0; i < d->n_entries; i++)
		print_dict_item(items[i]);

	// Destroy the dictionaries
	dict_destroy(d);
	dict_destroy(d_copy);

	return 0;
}

int print_dict_item ( dict_item *item )
{
	// Check for null pointer
	if (item == 0)
		return 0;

	// Print the key and the value
	printf("%s : 0x%p\n", item->key, item->value);

	return 0;
}
```

## Data types
```c
struct dict_item_s
{
	char* key;
	void* value;

	struct dict_item_s* next;
};

struct dict_s {
	struct dict_item_s  **entries;
	size_t                entry_count,
		                  n_entries;
};
```
## Type definitions
```c
typedef struct dict_item_s  dict_item;
typedef struct dict_s       dict;
```
## Function definitions
```c 
// Allocaters
int        dict_create    ( dict **dictionary );

// Constructors
int        dict_construct ( dict **dictionary, size_t     count );
int        dict_from_keys ( dict **dictionary, char      **keys, size_t    count );

// Accessors
void      *dict_get       ( dict  *dictionary, char       *name );
int        dict_items     ( dict  *dictionary, dict_item **items );
int        dict_values    ( dict  *dictionary, char      **values );
int        dict_keys      ( dict  *dictionary, char      **keys );

// Mutators
int        dict_add       ( dict  *dictionary, char       *name, void      *val );
int        dict_pop       ( dict  *dictionary, char       *name, void      **value);

// Shallow copy
dict      *dict_copy      ( dict  *dictionary, dict      **target );

// Clear all items
dict      *dict_clear     ( dict  *dictionary );

// Destructors
int        dict_destroy   ( dict  *dictionary );
```