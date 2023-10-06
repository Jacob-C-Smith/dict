# dict
[![CMake](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml)
 
 A minimal, thread-safe dictionary implementation written in C. 
 
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

 ## Download
 To download dict, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/dict
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd dict
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build dict for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./dict_example
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
 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./dict_test
 ```
 [Source](dict_test.c)
 
 [Tester output](test_output.txt)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct dict_s dict;
 ```
 ### Function definitions
 ```c 
 // Allocaters
 int dict_create ( dict **pp_dict );
 
 // Constructors
 int dict_construct ( dict **pp_dict, size_t   size, crypto_hash_function_64_t pfn_hash_function );
 int dict_from_keys ( dict **pp_dict, char   **keys, size_t keys_length );
 
 // Accessors
 void   *dict_get    ( dict *p_dict, char  *key );
 size_t  dict_values ( dict *p_dict, char **values );
 size_t  dict_keys   ( dict *p_dict, char **keys );
 
 // Mutators
 int dict_add ( dict *p_dict, const char *key, void  *p_value );
 int dict_pop ( dict *p_dict, char       *key, void **pp_value );
 
 // Shallow copy
 int dict_copy ( dict *p_dict, dict **pp_dict );
 
 // Clear all items
 int dict_clear      ( dict *p_dict );
 int dict_free_clear ( dict *p_dict, void (*free_func) (void *) );

 // Destructors
 int dict_destroy ( dict **pp_dict );
 ```

