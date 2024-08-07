#include "string.h"

/* Auxiliary functions */
bool white_space(char letter);

char* String::get_string() const {
    return this->string;
}

void String::set_string(const char *str){
    if (this->get_string() != nullptr){
        delete[] this->string;
    } 

    if (str == nullptr){
        this->string = nullptr;
    }

    this->string = new char[strlen(str) + 1];
    strcpy(this->string, str); 
}

/* Constructor Definition */
String::String(const char* str) {
    string = nullptr;
    String::set_string(str);
}

String::String(const String &other) {
    string = nullptr;
    set_string(other.get_string());
}

GenericString& String::operator=(const char *str) {
    /* Deallocate memory of original string */
    if (this->string) {
        delete[] this->string;
        this->string = nullptr;
    }

    /* Allocate memory of new string and copy its content */
    if (str){
        this->string = new char[strlen(str) + 1];
        strcpy(this->string, str);
    }
    
    return *this;
}

bool String::operator==(const char *other) const {
    if (!this->string || !other){
        return false;
    }
    
    return strcmp(this->string, other) == 0;
}

bool String::operator==(const GenericString &other) const {
    const String* otherString = dynamic_cast<const String*>(&other);
    
    /* Will check all conditions to avoid segmentation fault */
    /* Logical operators preform short circuit evaluation */
    if (otherString && this->string && otherString->string) {
        return strcmp(this->string, otherString->string) == 0;
    }
    return false;
}

char String::operator[](size_t index) const {
    if (this->string){
        return this->string[index];
    } 
    /* Otherwise */
    throw std::out_of_range("Index out of range");
}

StringArray String::split(const char *delimiters) const {
    /* Make a clone of current string */
    char* clone_string = new char[strlen(this->string) + 1];
    strcpy(clone_string, this->string);
    
    char* token = std::strtok(clone_string, delimiters);

    StringArray data;

    while (token != nullptr) {
        int i = 0;
        GenericString* str = make_string(token);
        data.set(str, i);
        token = strtok(nullptr, delimiters);
        i++;
    }
    
    delete[] clone_string;
    return data;
}

GenericString& String::trim() {
    if (!this->string) 
        return *this;

    size_t start = 0;
    size_t end = strlen(this->string);

    while (start < end && white_space(this->string[start])) {
        start++;
    }

    end--;
    while (end > start && white_space(this->string[end - 1])) {
        end--;
    }

    size_t new_length = end - start;
    char* trimmed_string = new char[new_length + 1];

    std::memmove(trimmed_string, this->string + start, new_length);
    trimmed_string[new_length] = '\0';

    /* Deleting the memory of original string and setting the pointer to new */
    delete[] this->string;
    this->string = trimmed_string;

    return *this;
}

int String::to_integer() const {
    if (this->string)
        return std::atoi(this->string);
    return 0;
}

String& String::as_string() {
    return *this;
}

const String& String::as_string() const {
    return *this;
}

GenericString* make_string(const char *str) {
    String* newString = new String(str);
    //*newString = str;  ~ no need because String constructor already copies
    return newString;
}

String::~String() {
    if (this->string){
        delete[] this->string;
    }
}

bool white_space(char letter) {
    return letter == ' ' || letter == '\t' || letter == '\n' ||
           letter == '\v' || letter == '\f' || letter == '\r';
}
