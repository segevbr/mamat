#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H
#include "generic-string.h"
#include "string.h"
#define MAX_ELEMENTS 4


class StringArray {
    GenericString** str_array;
    
public:
    int size;
    
    /* Constructor */
    StringArray ();
    StringArray(const StringArray& other);
    void set(GenericString* str, int index);

    GenericString* operator[](int index) const;
    
    /* Getter function */
    GenericString* get(int index) const;

    /* Dstructor */
    ~StringArray();
    
};

#endif