#include <dict/dict.h>

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
    
    // While you're there, you should donate a few bucks to Wikipedia.
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

    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                printf("[Dict] Null pointer provided for \"k\" in call to function \"%s\"\n", __FUNCSIG__);
                return 0;
        }
    }
}

int   dict_create    ( dict **dictionary )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (dictionary == (void*)0)
                goto no_dictionary;
        #endif
    }

    // Allocate for dictionary
    dict *ret = calloc(1, sizeof(dict));

    *dictionary = ret;

    // Error checking
    {
        #ifndef NDEBUG
            if(*dictionary == (void *)0)
                goto no_mem;
        #endif
    }

    // Success
    return 1;

    // Error handling
    {
        // Argument errors
        {
            no_dictionary:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int   dict_construct ( dict **dictionary, size_t      count )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (dictionary == (void*)0)
                goto no_dictionary;
        #endif
        
        if (count == 0)
            return 0;
    }

    // Initialized data
    dict* i_dictionary = 0;

    // Allocation failed
    if (dict_create(dictionary) == 0)
        return 0;
    
    i_dictionary = *dictionary;

    // Set the count and allocate for entries
    i_dictionary->entry_count = count;
    i_dictionary->entries     = calloc(count, sizeof(dict_item *));

    // Error checking
    {
        if (i_dictionary->entries == (void*)0)
            goto no_mem;
    }

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int   dict_from_keys ( dict **dictionary, char      **keys, size_t      count )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (dictionary == (void*)0)
                goto no_dictionary;
        #endif

        if (keys == 0)
            return 0;
        
        if (count == 0)
            return 0;
    }

    // Initialized data
    dict* i_dictionary = 0;

    // Allocation failed
    if (dict_create(dictionary) == 0)
        return 0;
    
    i_dictionary = *dictionary;

    // Set the count and allocate for entries
    i_dictionary->entry_count = count;
    i_dictionary->entries     = calloc(count, sizeof(dict_item *));

    // Error checking
    {
        if (i_dictionary->entries == (void*)0)
            goto no_mem;
    }

    // Add keys to the dictionary
    for (size_t i = 0; keys[i]; i++)
        dict_add(i_dictionary, keys[i], (void *)0);

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int   dict_add       ( dict  *dictionary, char       *name, void       *val )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
        #endif

        if (name == (void *)0)
            goto no_name;
    }

    // Initialized data
    dict_item* ret = dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count];

    // Find the name in the hash table
    {
        for (; ret != 0; ret = ret->next)
            if (strcmp(name, ret->key) == 0)
                break;
    }

    // Null pointer check
    if (ret == (void*)0)
    {
        // Allocate a new dict_item
        ret = (dict_item*) calloc(1, sizeof(dict_item));

        // Error checking
        {
            #ifndef NDEBUG
                if(ret == (void *)0)
                    goto no_mem;
            #endif
        }

        ret->key = name;

        // Compute the hash
        unsigned long long h = mmh64(name, strlen(ret->key));

        // Insert the hash
        ret->next = dictionary->entries[(h % dictionary->entry_count)];
        dictionary->entries[(h % dictionary->entry_count)] = ret;
        dictionary->n_entries++;

    }

    // Assign the correct value
    ret->value = val;

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
                return 0;
            no_name:
                printf("[Dict] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

void *dict_get       ( dict  *dictionary, char       *name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
            if(name == (void *)0)
                goto no_name;
        #endif
    }

    // Initialized data
    dict_item *ret = dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count];
    
    // Iterate over each item in the linked list
    for (; ret != 0; ret = ret->next)

        // Is it the one we're looking for?
        if (strcmp(name, ret->key) == 0)
            break;
    
    // Return the value if it exists, otherwise null pointer
    return (ret) ? (void *)ret->value : 0;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_name:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int   dict_items     ( dict  *dictionary, dict_item **items)
{
    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictioanry;
        #endif
    }

    size_t c = dictionary->n_entries;

    if (items == 0)
        return c;

    // Iterate through table
    for (size_t i = 0; i < dictionary->entry_count; i++)
        
        // Is there an item on the iterator
        if (dictionary->entries[i])
        {

            // Get the item
            dict_item *item = dictionary->entries[i];

            // Write the item to the list
            items[--c] = dictionary->entries[i];

            // And its collisions
            while(item = item->next)
                items[--c] = item;
        }

    return 1;


    // Error handling
    {
        
        // Argument errors
        {
            no_dictioanry:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
        
    }
}

int   dict_values    ( dict  *dictionary, char      **values)
{
    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictioanry;
        #endif
    }

    size_t c = dictionary->n_entries;

    if (values == 0)
        return c;

    // Iterate through table
    for (size_t i = 0; i < dictionary->entry_count; i++)
        
        // Is there an item on the iterator
        if (dictionary->entries[i])
        {

            // Get the item
            dict_item *item = dictionary->entries[i];

            // Write the item to the list
            values[--c] = item->value;

            // And its collisions
            while(item = item->next)
                values[--c] = item->value;
        }

    return 1;


    // Error handling
    {
        
        // Argument errors
        {
            no_dictioanry:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
        
    }
}

int   dict_keys      ( dict  *dictionary, char      **keys)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictioanry;
        #endif
    }

    size_t c = dictionary->n_entries;

    if (keys == 0)
        return c;

    // Iterate through table
    for (size_t i = 0; i < dictionary->entry_count; i++)
        
        // Is there an item on the iterator
        if (dictionary->entries[i])
        {

            // Get the item
            dict_item *item = dictionary->entries[i];

            // Write the item to the list
            keys[--c] = item->key;

            // And its collisions
            while(item = item->next)
                keys[--c] = item->key;
        }

    return 1;


    // Error handling
    {
        
        // Argument errors
        {
            no_dictioanry:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
        
    }
}

int   dict_pop       ( dict  *dictionary, char       *name, void      **value)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Initialized data
    dict_item *ret  = dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count],
              *next = (ret) ? ret->next : 0,
              *i    = 0;

    // Is there a hash table entry?
    if (ret)
    {

        // Iterate through linked list
        while (ret->next)
        {

            // Look for the right element
            if (strcmp(ret->key, name) == 0)

                // Found the right element
                break;
            ret = ret->next;
        }
    }
    else
        goto no_item;

    
    i = ret;
    dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count] = ret->next;
    

    *value = i->value;

    free(i);

    dictionary->n_entries--;

    return 1;

    // Error handling
    {

        // Argument error
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n\"", __FUNCSIG__);
                #endif
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"name\" in call to function \"%s\"\n\"", __FUNCSIG__);
                #endif
                return 0;
        }

        // Failed to find item
        {
            no_item:
            #ifndef NDEBUG
                printf("[Dictionary] Failed to find value from key \"%s\" in call to function \"%s\"\n", name, __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int   dict_pop_item  ( dict  *dictionary, char       *name, dict_item *item )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Initialized data
    dict_item *ret  = dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count],
              *next = (ret) ? ret->next : 0,
              *i    = 0;

    // Is there a hash table entry?
    if (ret)
    {

        // Iterate through linked list
        while (ret->next)
        {

            // Look for the right element
            if (strcmp(ret->key, name) == 0)

                // Found the right element
                break;
            ret = ret->next;
        }
    }
    else
        goto no_item;
        
    i = ret;
    dictionary->entries[mmh64(name, strlen(name)) % dictionary->entry_count] = ret->next;
    
    item->key   = name,
    item->value = i->value;

    free(i);

    dictionary->n_entries--;

    return 1;

    // Error handling
    {

        // Argument error
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n\"", __FUNCSIG__);
                #endif
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"name\" in call to function \"%s\"\n\"", __FUNCSIG__);
                #endif
                return 0;
        }

        // Failed to find item
        {
            no_item:
            #ifndef NDEBUG
                printf("[Dictionary] Failed to find value from key \"%s\" in call to function \"%s\"\n", name, __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int   dict_copy      ( dict  *dictionary, dict** target)
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
            if (target == (void*)0)
                goto no_target;
        #endif
    }

    // Initialized data
    char **keys   = calloc(dictionary->n_entries + 1, sizeof(char*));;
    void **values = calloc(dictionary->n_entries + 1, sizeof(char*));;

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
    dict_construct(target, dictionary->entry_count);

    // Get the keys and values out of the first dictionary
    dict_keys(dictionary, keys);
    dict_values(dictionary, values);

    // Insert each value
    for (size_t i = 0; i < dictionary->n_entries && keys[i]; i++)
        dict_add(*target, keys[i], values[i]);

    // Free the lists
    free(keys);
    free(values);

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_target:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"target\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}
    }

}

int   dict_clear     ( dict  *dictionary)
{
    // Argument check
    {
        #ifndef NDEBUG
            if (dictionary == (void*)0)
                goto no_dictionary;
        #endif
    }

    // Iterate over each hash table item
    for (size_t i = 0; i < dictionary->entry_count; i++)

        // Check for an item
        if (dictionary->entries[i])
        {
            dict_item *di = dictionary->entries[i];

            while (di)
            {
                dict_item *n = di->next;
                
                // Free the item
                free(di);

                di = n;
            }

            
        }
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_dictionary:
                #ifndef NDEBUG
                    printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int   dict_destroy   ( dict  *dictionary )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(dictionary == (void *)0)
                goto no_dictionary;
        #endif
    }

    dict_clear(dictionary);
    
    // Free the table
    free(dictionary->entries);

    dictionary->entry_count = 0;

    // Free the dictionary
    free(dictionary);
    
    return 1;

    // Error handling
    {
        no_dictionary:
            #ifndef NDEBUG
                printf("[Dict] Null pointer provided for \"dictionary\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
    }
}
