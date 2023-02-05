/** !
 * @file dict/dict.h 
 * @author Jacob Smith
 * 
 * Include header for dictionary library
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Forward declarations
struct dict_item_s;
struct dict_s;

// Type definitions
/** !
 *  @brief The type definition of a dictionary struct
 */
typedef struct dict_s dict;

// Allocaters
/** !
 *  Allocate memory for a dictionary
 *
 * @param pp_dict return
 *
 * @sa destroy_dict
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_create ( dict **pp_dict );

// Constructors
/** !
 *  Construct a dictionary with a specific number of hash table entries
 *
 * @param pp_dict  return
 * @param size     number of hash table entries. 
 *
 * @sa dict_create
 * @sa destroy_dict
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_construct ( dict **pp_dict, size_t size );

/** !
 *  Construct a dictionary from an array of strings
 *
 * @param pp_dict     return
 * @param keys        pointer to array of strings
 * @param keys_length length of keys array
 *
 * @sa dict_create
 * @sa destroy_dict
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_from_keys ( dict **pp_dict, char **keys, size_t keys_length );

// Accessors
/** !
 *  Get a property's value
 *
 * @param p_dict dictionary
 * @param key    the name of the property
 *
 * @sa dict_values
 * @sa dict_keys
 *
 * @return pointer to specified property's value on success, null pointer on error
 */
DLLEXPORT void *dict_get ( dict *p_dict, char *key );

/** !
 *  Get a dictionarys' values, or the number of properties in the dictionary
 *
 * @param p_dict dictionary
 * @param values   return -OR- null pointer
 *
 * @sa dict_get
 * @sa dict_keys
 *
 * @return 1 on success, 0 on error, if values != null, else number of properties in dictionary
 */
DLLEXPORT size_t dict_values ( dict *p_dict, char **values );

/** !
 *  Get a dictionarys' keys, or the number of properties in the dictionary
 *
 * @param p_dict dictionary
 * @param keys   return -OR- null pointer
 *
 * @sa dict_get
 * @sa dict_values
 *
 * @return 1 on success, 0 on error, if keys != null, else number of properties in dictionary
 */
DLLEXPORT size_t dict_keys ( dict *p_dict, char **keys );

// Mutators
/** !
 *  Add a property to a dictionary. 
 *
 * @param p_dict  dictionary
 * @param key     the name of the property
 * @param p_value the value of the property
 *
 * @sa dict_pop
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_add ( dict *p_dict, const char *key, void *p_value );

/** !
 *  Remove a property from a dictionary. 
 *
 * @param p_dict   dictionary
 * @param key      the name of the property
 * @param pp_value return
 *
 * @sa dict_add
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_pop ( dict *p_dict, char *key, void **pp_value );

// Shallow copy
/** !
 *  Make a shallow copy of a dictionary
 *
 * @param p_dict  source dictionary
 * @param pp_dict return
 *
 * @sa dict_clear
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_copy ( dict *p_dict, dict **pp_dict );

// Clear all items
/** !
 *  Remove all properties from a dictionary
 *
 * @param p_dict dictionary
 *
 * @sa dict_copy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_clear ( dict *p_dict );

// Destructors
/** !
 *  Destroy and deallocate a dictionary
 *
 * @param p_dict dictionary
 *
 * @sa dict_create
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int dict_destroy ( dict *p_dict );