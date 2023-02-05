#include <dict/dict.h>

struct dict_item_s
{
	char               *key;   // The key
	void               *value; // The value
    size_t              index; // The index in the iterable lists

	struct dict_item_s *next;  // The next entry
};

struct dict_s {
	struct dict_item_s **entries;       // Hash table contents
	size_t               entry_max,     // Hash table elements
		                 entry_count,   // Entries
                         iterable_max;  // Iterable array bound
    char               **keys;          // Iterable keys
    void               **values;        // Iterable values
};

unsigned long long mmh64 ( void* k, size_t l )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (k == (void*)0)
                goto no_k;
        #endif
    }
    
    // Constant data
    const unsigned long long m    = 0xc6a4a7935bd1e995; 
    const int                r    = 47;

    // Initialized data
    unsigned long long       h    = 0x41C64E6D ^ (l * m);
    unsigned long long      *data = (unsigned long long*)k;
    unsigned long long      *end  = (l >> 3) + data;

    // Just go look it up on Wikipedia. I'm not going to pretend I know how this works. 
    // https://en.wikipedia.org/wiki/MurmurHash
    while (data != end)
    {
        unsigned long long k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;

    }

    unsigned char* d2 = data;

    switch (l & 7)
    {
        case 7: h ^= (unsigned long long)(d2[6]) << 48;
        case 6: h ^= (unsigned long long)(d2[5]) << 40;
        case 5: h ^= (unsigned long long)(d2[4]) << 32;
        case 4: h ^= (unsigned long long)(d2[3]) << 24;
        case 3: h ^= (unsigned long long)(d2[2]) << 16;
        case 2: h ^= (unsigned long long)(d2[1]) << 8;
        case 1: h ^= (unsigned long long)(d2[0]);
            h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                printf("[dict] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);

                // Error
                return 0;
        }
    }
}

int dict_create ( dict **pp_dict )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_dict == (void *) 0 )
                goto no_dictionary;
        #endif
    }

    // Allocate memory for a dictionary
    dict *p_dict = calloc(1, sizeof(dict));

    // Error checking
    {
        #ifndef NDEBUG
            if( p_dict == (void *) 0 )
                goto no_mem;
        #endif
    }

    // Return the allocated memory
    *pp_dict = p_dict;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int dict_construct ( dict **pp_dict, size_t size )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_dict == (void *) 0 )
                goto no_dictionary;
        #endif
        
        if (size == 0)
            goto zero_size;
    }

    // Initialized data
    dict *p_dict = 0;

    // Allocate a dictionary
    if ( dict_create(pp_dict) == 0 )
        goto failed_to_create_dict;
    
    // Get a pointer to the allocated dictionary
    p_dict               = *pp_dict;

    // Set the count and iterator max
    p_dict->entry_max    = size;
    p_dict->iterable_max = 1;

    // Allocate "size" number of properties
    p_dict->entries      = calloc(size, sizeof(dict_item *));

    // Allocate key and value lists
    p_dict->keys         = calloc(1, sizeof(char *));
    p_dict->values       = calloc(1, sizeof(void *));

    // Error checking
    {
        if ( p_dict->entries == (void *) 0 )
            goto no_mem;

        if ( p_dict->keys    == (void *) 0 )
            goto no_mem;

        if ( p_dict->values  == (void *) 0 )
            goto no_mem;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for parameter \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    printf("[dict] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
                
        }

        // Dictionary errors
        {
            failed_to_create_dict:
                #ifndef NDEBUG
                    printf("[dict] Failed to create dictionary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int dict_from_keys ( dict **pp_dict, char **keys, size_t keys_length )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_dict == (void *) 0 )
                goto no_dictionary;
        #endif

        if ( keys == 0 )
            return 0;
        
        if ( keys_length == 0 )
            return 0;
    }

    // Initialized data
    dict* p_dict = 0;

    // Allocation failed
    if ( dict_construct(pp_dict, keys_length) == 0 )
        return 0;
    
    p_dict = *pp_dict;

    // Set the count and allocate for entries
    p_dict->entry_max = keys_length;
    p_dict->entries     = calloc(keys_length, sizeof(dict_item *));

    // Error checking
    {
        if (p_dict->entries == (void*)0)
            goto no_mem;
    }

    // Add keys to the dictionary
    for (size_t i = 0; keys[i]; i++)
        dict_add(p_dict, keys[i], (void *)0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
            #ifndef NDEBUG
                printf("[dict] Null pointer provided for \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error 
            return 0;
        }

        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error 
            return 0;
        }
    }
}

void *dict_get ( dict *p_dict, char *key )
{

    // Argument check
    {
        #ifndef NDEBUG
            if( p_dict == (void *) 0 )
                goto no_dictionary;
            if( key == (void *) 0 )
                goto no_name;
        #endif
    }

    // Initialized data
    dict_item *ret = p_dict->entries[mmh64(key, strlen(key)) % p_dict->entry_max];
    
    // Iterate over each item in the linked list
    for (; ret != 0; ret = ret->next)

        // Is it the one we're looking for?
        if ( strcmp(key, ret->key) == 0 )
            break;
    
    // Return the value if it exists, otherwise null pointer
    return (ret) ? (void *)ret->value : 0;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

size_t dict_values ( dict *p_dict, char **values )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( p_dict == (void *) 0 )
                goto no_dictioanry;
        #endif
    }

    // Initialized data
    size_t entry_count = p_dict->entry_count;

    // Error check
    {
        if (values == 0)
            return entry_count;
    }

    // Copy memory
    memcpy(values, p_dict->values, entry_count * sizeof(void *));

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_dictioanry:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
        
    }
}

size_t dict_keys ( dict *p_dict, char **keys )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_dict == (void *) 0 )
                goto no_dictioanry;
        #endif
    }

    // Initialized data
    size_t entry_count = p_dict->entry_count;

    // Error check
    {
        if (keys == 0)
            return entry_count;
    }

    // Copy memory
    memcpy(keys, p_dict->keys, entry_count * sizeof(void *));

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_dictioanry:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
    }
}

int dict_add ( dict *p_dict, const char *key, void *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_dict == (void *)0)
                goto no_dictionary;
        #endif

        if (key == (void *)0)
            goto no_name;
    }

    // Initialized data
    unsigned long long  h        = mmh64(key, strlen(key));
    dict_item          *property = p_dict->entries[h % p_dict->entry_max];

    // Find the key in the hash table
    {
        for (; property != 0; property = property->next)
            if ( strcmp(key, property->key) == 0 )
                break;
    }

    // Make a new property
    if (property == (void*)0)
    {

        // Allocate a new dict_item
        property = (dict_item*) calloc(1, sizeof(dict_item));

        // Error checking
        {
            #ifndef NDEBUG
                if(property == (void *)0)
                    goto no_mem;
            #endif
        }

        // Set the property
        property->key = key;
        property->value = p_value;

        // Insert the hash
        property->next = p_dict->entries[(h % p_dict->entry_max)];
        property->index = p_dict->entry_count;

        p_dict->entries[(h % p_dict->entry_max)] = property;

        // Update the iterables
        p_dict->keys[p_dict->entry_count]   = key;
        p_dict->values[p_dict->entry_count] = p_value;

        // Increment the entry counter
        p_dict->entry_count++;

        // Resize iterable max?
        if ( p_dict->entry_count >= p_dict->iterable_max)
        {
        
            // Double the size
            p_dict->iterable_max *= 2;
    
            // Reallocate iterable arrays
            p_dict->keys   = realloc(p_dict->keys  , p_dict->iterable_max * sizeof(void *));
            p_dict->values = realloc(p_dict->values, p_dict->iterable_max * sizeof(void *));
    
            // Error checking
            {
                #ifndef NDEBUG
                    if ( p_dict->keys == (void *) 0 )
                        goto no_mem;
                    if ( p_dict->keys == (void *) 0 )
                        goto no_mem;
                #endif
            }
        }
    }

    // Update an existing property
    else
    {
        
        // Update the property
        property->value = p_value;

        // Update the iterable
        p_dict->values[property->index] = property->value;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int dict_pop ( dict *p_dict, char *key, void **pp_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_dict == (void *)0)
                goto no_dictionary;
            if (key == (void*)0)
                goto no_name;
        #endif
    }


    // Create a pointer to the head of the list
    unsigned long long  h = mmh64(key, strlen(key));
    dict_item          *i = p_dict->entries[h % p_dict->entry_max];
    dict_item          *k = 0;
    // Quick sanity check
    if (i == 0)
        goto no_name;
    
    // Check the head
    if (strcmp(key, i->key) == 0)
    {
        dict_item *j = i->next;
        p_dict->entries[h % p_dict->entry_max] = j;
        goto free_item;
    }

    while (i->next)
    {

        if (strcmp(key, i->next->key) == 0) 
        {
            dict_item *j = i->next->next;
            k=i->next;
            // Stitch up the linked list 
            i->next = j;
            goto free_item;
        }

        i = i->next;
    }



    goto no_item;

    free_item:

    if (strcmp(key, i->key) == 0)
        k=i;

    // Return the value
    if (pp_value)
        *pp_value = 0; //i->value;

    // Clean up the iterables
    {

        // Initialized data
        size_t              idx       = k->index;
        char               *swap_key  = p_dict->keys[p_dict->entry_count-1];
        unsigned long long  swap_hash = mmh64(swap_key, strlen(swap_key));
        dict_item          *swap_item = p_dict->entries[swap_hash % p_dict->entry_max];

        if (idx == p_dict->entry_count-1)
        {
            p_dict->keys[p_dict->entry_count-1] = 0;
            p_dict->values[p_dict->entry_count-1] = 0;
            goto done;
        }

        // Clean up key iterable
        p_dict->keys[idx] = p_dict->keys[p_dict->entry_count-1];
        p_dict->keys[p_dict->entry_count-1] = 0;

        // Clean up value iterable
        p_dict->values[idx] = p_dict->values[p_dict->entry_count-1];
        p_dict->values[p_dict->entry_count-1] = 0;

        swap_item->index = idx;
    }

    done:
    // Free the pop'd dict_item
    free(k);

    // Decrement entries
    p_dict->entry_count--;

    // Resize iterable max?
    if ( p_dict->entry_count < p_dict->iterable_max / 2)
    {
    
        // Double the size
        p_dict->iterable_max /= 2;

        // Reallocate iterable arrays
        p_dict->keys   = realloc(p_dict->keys  , p_dict->iterable_max * sizeof(void *));
        p_dict->values = realloc(p_dict->values, p_dict->iterable_max * sizeof(void *));

        // Error checking
        {
            #ifndef NDEBUG
                if ( p_dict->keys == (void *) 0 )
                    goto no_mem;
                if ( p_dict->keys == (void *) 0 )
                    goto no_mem;
            #endif
        }
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n\"", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"name\" in call to function \"%s\"\n\"", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Failed to find item
        {
            no_item:
                #ifndef NDEBUG
                    printf("[Dictionary] Failed to find property from key \"%s\" in call to function \"%s\"\n", key, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int dict_copy ( dict  *p_dict, dict** pp_dict )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if (p_dict == (void *)0)
                goto no_dictionary;
            if (pp_dict == (void*)0)
                goto no_target;
        #endif
    }

    // Initialized data
    char **keys   = calloc(p_dict->entry_count + 1, sizeof(char*));
    void **values = calloc(p_dict->entry_count + 1, sizeof(char*));

    // Error checking
    {
        #ifndef NDEBUG
            if(keys == (void *)0)
                goto no_mem;
            if (values == (void*)0)
                goto no_mem;
        #endif 
    }

    // Construct a new dictionary of the same size
    dict_construct(pp_dict, p_dict->entry_max);

    // Get the keys and values out of the first dictionary
    dict_keys(p_dict, keys);
    dict_values(p_dict, values);

    // Insert each value
    for (size_t i = 0; i < p_dict->entry_count && keys[i]; i++)
        dict_add(*pp_dict, keys[i], values[i]);

    // Free the lists
    free(keys);
    free(values);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_target:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"pp_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
		{
			no_mem:
			    #ifndef NDEBUG
			    	printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
			    #endif

                // Error
			    return 0;
		}
    }

}

int dict_clear ( dict  *p_dict )
{
    // Argument check
    {
        #ifndef NDEBUG
            if (p_dict == (void*)0)
                goto no_dictionary;
        #endif
    }

    // Iterate over each hash table item
    for (size_t i = 0; i < p_dict->entry_max; i++)

        // p_dict for an item
        if (p_dict->entries[i])
        {
            dict_item *di = p_dict->entries[i];

            while (di)
            {
                dict_item *n = di->next;
                
                // Free the item
                free(di);

                di = n;
            }

            
        }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int dict_destroy ( dict  *p_dict )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_dict == (void *) 0 )
                goto no_dictionary;
        #endif
    }

    // Remove all dictionary entries
    dict_clear(p_dict);
    
    // Free the table
    free(p_dict->entries);

    // Free the iterables
    free(p_dict->keys);
    free(p_dict->values);

    // Free the dictionary
    free(p_dict);
    
    // Success
    return 1;

    // Error handling
    {
        no_dictionary:
            #ifndef NDEBUG
                printf("[dict] Null pointer provided for \"p_dict\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}