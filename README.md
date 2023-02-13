# dict
 A minimal dictionary implementation written in C. 
 
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 2 [Example](#example)
 >
 >> 2.1 [Example output](#example-output)
 >
 > 3 [Definitions](#definitions)
 >
 >> 3.1 [Type definitions](#type-definitions)
 >>
 >> 3.2 [Function definitions](#function-definitions)
 
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
  This will build the example program, and dynamic / shared libraries

  To build dict for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 ./dict_example
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
