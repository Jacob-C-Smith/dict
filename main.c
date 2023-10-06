// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <dict/dict.h>

// Forward declaration
int  print_all_keys         ( dict *d );
void print_value_as_integer ( const void *const p_value, size_t i );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    char     *keys[]      = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };
    dict     *p_dict      = (void *) 0;
    dict     *p_dict_copy = (void *) 0;

    // Make a dictionary with 4 hash table items
    dict_construct(&p_dict, 4, 0);

    // Insert some items
    dict_add(p_dict, "Red"  , (void *)0xff0000);
    dict_add(p_dict, "Green", (void *)0x00ff00);
    dict_add(p_dict, "Blue" , (void *)0x0000ff);

    // Print the dictionarys' keys
    print_all_keys(p_dict);

    // Destroy the dictionary
    dict_destroy(&p_dict);

    // Create a dictionary from provided keys 
    dict_from_keys(&p_dict, keys, 10);

    // Copy the dictionary
    dict_copy(p_dict, &p_dict_copy);

    // Add "Iguana", "Capybara", and "Brown Bear"
    dict_add(p_dict, "Iguana", (void *)0x1234);
    dict_add(p_dict, "Capybara", (void *)0x5678);
    dict_add(p_dict, "Brown Bear", (void *)0x9ABC);

    // Remove "Birds" and "Iguana"
    dict_pop(p_dict, "Birds", 0);
    dict_pop(p_dict, "Iguana", 0);
    dict_pop(p_dict, "Brown Bear", 0);

    // Print the dictionarys' keys
    print_all_keys(p_dict);

    // Print the dictionary that was constructed from the keys
    print_all_keys(p_dict);

    // Print each value as a hexidecimal number
    dict_foreach(p_dict, &print_value_as_integer);

    // Destroy the dictionaries
    dict_destroy(&p_dict);
    dict_destroy(&p_dict_copy);

    // Success
    return EXIT_SUCCESS;
}

// Print the dictionarys' keys
int print_all_keys(dict* p_dict)
{

    if (p_dict == 0)
        return 0;

    // Get the dictionarys' keys
    size_t key_count = dict_keys(p_dict, 0);
    char** keys = calloc(key_count, sizeof(char*));

    dict_keys(p_dict, keys);

    // Print each key
    for (size_t i = 0; i < key_count; i++)
        printf("%s\n", keys[i]);
    
    // Formatting
    putchar('\n');

    // Clean up
    free(keys);

    // Success
    return 1;
}

void print_value_as_integer ( const void *const p_value, size_t i )
{

    // Print the value as a hexidecimal number
    printf("0x%zx\n",(size_t) p_value);

    // Return
    return;
}