#ifndef PERSON_H
#define PERSON_H

#include <iostream>

// Abstract base class for all user types in the system
class Person {
protected:
    int    id;
    char   name[100];
    char   password[100];

public:
    Person();
    Person(int id, const char* name, const char* password);
    virtual ~Person();

    // Getters
    int         getId()      const;
    const char* getName()    const;
    const char* getPassword() const;

    // Setters
    void setId(int id);
    void setName(const char* name);
    void setPassword(const char* pw);

    // Pure virtual methods — every subclass must implement these
    virtual void displayMenu() = 0;
    virtual void displayInfo()   const = 0;

    // Utility: copy a char array safely
    static void copyStr(char* dest, const char* src, int maxLen);
    // Utility: compare two char arrays (returns 0 if equal)
    static int  compareStr(const char* a, const char* b);
    // Utility: length of a char array
    static int  strLen(const char* s);
    // Utility: case-insensitive compare
    static int  compareStrCI(const char* a, const char* b);
    // Utility: convert char to lower
    static char toLowerChar(char c);
    // Utility: convert int to string into buffer
    static void intToStr(int n, char* buf, int bufSize);
    // Utility: convert float to string into buffer
    static void floatToStr(float f, char* buf, int bufSize, int decimals = 2);
    // Utility: string to int
    static int  strToInt(const char* s);
    // Utility: string to float
    static float strToFloat(const char* s);
    // Utility: check if all chars are digits
    static bool isAllDigits(const char* s);
    // Utility: trim leading/trailing whitespace in place
    static void trim(char* s);
};

#endif

