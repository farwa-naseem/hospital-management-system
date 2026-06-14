#ifndef VALIDATOR_H
#define VALIDATOR_H

// All input validation logic lives here.
// No other class may contain validation logic.
class Validator {
public:
    // Returns true if s is a valid DD-MM-YYYY date
    // (day 01-31, month 01-12, year >= currentYear)
    static bool validateDate(const char* s, int currentYear);

    // Returns true if s is one of the 8 valid time slots
    static bool validateTimeSlot(const char* s);

    // Returns true if s is exactly 11 numeric digits
    static bool validateContact(const char* s);

    // Returns true if password length >= 6
    static bool validatePassword(const char* s);

    // Returns true if s represents a positive float > 0
    static bool validatePositiveFloat(const char* s);

    // Returns true if s represents a valid positive integer > 0
    static bool validatePositiveInt(const char* s);

    // Returns true if the menu choice is in [1, maxOption]
    static bool validateMenuChoice(int choice, int maxOption);

private:
    static int strLen(const char* s);
    static bool isDigit(char c);
};

#endif
