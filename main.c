#include <stdio.h>
#include <stdlib.h>

#include <dict/dict.h>

int main(int argc, const char* argv[])
{

	// Constant data
	char* keys[] = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };

	// Initialized data
	dict *d      = (void *) 0;
	dict *d_copy = (void *) 0;

	// Make a dictionary with 4 hash table items
	dict_construct(&d, 4);

	// Insert some items
	dict_add(d, "Red"  , (void *)0xff0000);
	dict_add(d, "Green", (void *)0x00ff00);
	dict_add(d, "Blue" , (void *)0x0000ff);

	// Copy the dictionary
	dict_copy(d, &d_copy);

	// Destroy the dictionary
	dict_destroy(d);

	// Create a dictionary from the keys 
	dict_from_keys(&d, keys, 10);

	// Allocate for items
	dict_item **items  = calloc(dict_keys(d,0) + 1, sizeof(void*));

	// Update items "A", "B", and "C"
	dict_add(d, "A", (void *)0x1234);
	dict_add(d, "B", (void *)0x5678);
	dict_add(d, "C", (void *)0x9ABC);

	// Destroy the dictionaries
	dict_destroy(d);
	dict_destroy(d_copy);

}
