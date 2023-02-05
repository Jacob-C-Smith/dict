# dict
 A minimal dictionary implementation written in C. 
 
 > 1 [Build instructions](#build-instructions)
 >
 > 2 [Example](#example)
 >
 >> 2.1 [Example code](#example-code)
 >>
 >> 2.2 [Example output](#example-output)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function definitions](#function-definitions)
 
 ## Build instructions
 To build dict for Windows machines, open the base directory in Visual Studio, and build your desired target(s)

 To build on UNIX like machines, run the following commands
 ```bash
 $ cmake .
 $ make
 ```
 ## Example
 ### Example code
 This is the example code for the dict_example CMake target
 ```c
// Standard library
#include <stdio.h>
#include <stdlib.h>

// dict
#include <dict/dict.h>

// Forward declaration
int print_all_keys(dict* d);

// Entry point
int main(int argc, const char* argv[])
{

	// Initialized data
	char    *keys[]   = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };
	dict     *d        = (void *) 0;
	dict     *d_copy   = (void *) 0;

	// Make a dictionary with 4 hash table items
	dict_construct(&d, 4);

	// Insert some items
	dict_add(d, "Red"  , (void *)0xff0000);
	dict_add(d, "Green", (void *)0x00ff00);
	dict_add(d, "Blue" , (void *)0x0000ff);

	// Print the dictionarys' keys
	print_all_keys(d);

	// Destroy the dictionary
	dict_destroy(d);

	// Create a dictionary from provided keys 
	dict_from_keys(&d, keys, 10);

	// Copy the dictionary
	dict_copy(d, &d_copy);

	// Update items "Iguana", "Capybara", and "Brown Bear"
	dict_add(d, "Iguana", (void *)0x1234);
	dict_add(d, "Capybara", (void *)0x5678);
	dict_add(d, "Brown Bear", (void *)0x9ABC);

	// Remove "Birds" and "Iguana"
	dict_pop(d, "Birds", 0);
	dict_pop(d, "Iguana", 0);

	// Print the dictionarys' keys
	print_all_keys(d);

	// Print the dictionary that was constructed from the keys
	print_all_keys(d_copy);

	// Destroy the dictionaries
	dict_destroy(d);
	dict_destroy(d_copy);

	// Success
	return 0;
}

// Print the dictionarys' keys
int print_all_keys(dict* d)
{

	// Get the dictionarys' keys
	size_t key_count = dict_keys(d, 0);
	char** keys = calloc(key_count, sizeof(char*));

	dict_keys(d, keys);

	// Print each key
	for (size_t i = 0; i < key_count; i++)
		printf("%s\n", keys[i]);

	// Formatting
	putchar('\n');

	// Clean up
	free(keys);
}
 ```
 ### Example output
 ```
Red
Green
Blue

Dogs
Cats
Brown Bear
Fish
Capybara

Dogs
Cats
Birds
Fish
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
