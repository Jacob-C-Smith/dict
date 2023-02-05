# dict
 A minimal dictionary implementation written in C. 
 >  1.   [Example code](#example-code)
 >
 >  2.   [Definitions](#definitions)
 >>
 >> 2.1. [Type definitions](#type-definitions)
 >> 
 >> 2.2. [Function definitions](#function-definitions)
 
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
 
 ## Definitions
 ### Type definitions
 ```c
 typedef struct dict_s dict;
 ```
 ### Function definitions
 ```c 
 // Allocaters
 int     dict_create    ( dict **pp_dict );
 
 // Constructors
 int     dict_construct ( dict **pp_dict, size_t      size );
 int     dict_from_keys ( dict **pp_dict, char      **keys, size_t   keys_length );
 
 // Accessors
 void   *dict_get       ( dict  *p_dict, char        *key );
 size_t  dict_values    ( dict  *p_dict, char       **values );
 size_t  dict_keys      ( dict  *p_dict, char       **keys );
 
 // Mutators
 int     dict_add       ( dict  *p_dict, const char  *key, void     *p_value );
 int     dict_pop       ( dict  *p_dict, char        *key, void    **pp_value );
 
 // Shallow copy
 int     dict_copy      ( dict  *p_dict, dict       **pp_dict );
 
 // Clear all items
 int     dict_clear     ( dict  *p_dict );
 
 // Destructors
 int     dict_destroy   ( dict  *p_dict );
 ```