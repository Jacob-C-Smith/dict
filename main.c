#include <stdio.h>
#include <stdlib.h>

#include <dict/dict.h>

int print_dict_item ( dict_item *item )
{
	// Check for null pointer
	if (item == 0)
		return 0;

	// Print the key and the value
	printf("%s : 0x%p\n", item->key, item->value);

	return 0;
}

int main(int argc, const char* argv[])
{
	// Constant data
	char* keys[] = { "A", "B", "C", "D", (void*)0 };

	// Initialized data
	dict* d = (void *) 0;

	// Create a dictionary from the keys 
	dict_from_keys(&d, &keys, 10);

	// Allocate for items
	dict_item **items  = calloc(dict_items(d, 0) + 1, sizeof(void*));

	// Update the item "A"
	dict_add(d, "A", 0x1234);
	dict_add(d, "C", 0x5678);
	dict_add(d, "D", 0x9ABC);

	void *p = dict_pop(d, "D");

	printf("0x%p\n", p);

	// Write items to item list
	dict_items(d, items);

	// Print items
	for (size_t i = 0; i < d->n_entries; i++)
		print_dict_item(items[i]);

	// Destroy the dictionary
	dict_destroy(d);

	return 0;
}