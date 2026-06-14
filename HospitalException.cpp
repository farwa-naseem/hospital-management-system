#include "HospitalException.h"

// ─── HospitalException ────────────────────────────────────────────────────────

HospitalException::HospitalException() { message[0] = '\0'; }

HospitalException::HospitalException(const char* msg) {
    Person::copyStr(message, msg, 200);
}

HospitalException::~HospitalException() {}

const char* HospitalException::what() const { return message; }

// ─── FileNotFoundException ────────────────────────────────────────────────────

FileNotFoundException::FileNotFoundException(const char* filename) {
    char buf[200] = "Error: Could not open required file: ";
    int  len = Person::strLen(buf);
    Person::copyStr(buf + len, filename, 200 - len);
    Person::copyStr(message, buf, 200);
}

// ─── InsufficientFundsException ───────────────────────────────────────────────

InsufficientFundsException::InsufficientFundsException() {
    Person::copyStr(message,
        "Insufficient funds. Please top up your balance.", 200);
}

InsufficientFundsException::InsufficientFundsException(float required, float available) {
    char req[32], avl[32];
    Person::floatToStr(required, req, 32, 2);
    Person::floatToStr(available, avl, 32, 2);
    char buf[200] = "Insufficient funds. Required: PKR ";
    int  len = Person::strLen(buf);
    Person::copyStr(buf + len, req, 200 - len);
    len = Person::strLen(buf);
    Person::copyStr(buf + len, ", Available: PKR ", 200 - len);
    len = Person::strLen(buf);
    Person::copyStr(buf + len, avl, 200 - len);
    Person::copyStr(message, buf, 200);
}

// ─── InvalidInputException ────────────────────────────────────────────────────

InvalidInputException::InvalidInputException(const char* msg) {
    Person::copyStr(message, msg, 200);
}

// ─── SlotUnavailableException ─────────────────────────────────────────────────

SlotUnavailableException::SlotUnavailableException() {
    Person::copyStr(message, "The selected time slot is not available.", 200);
}

SlotUnavailableException::SlotUnavailableException(const char* slot) {
    char buf[200] = "Slot ";
    int  len = Person::strLen(buf);
    Person::copyStr(buf + len, slot, 200 - len);
    len = Person::strLen(buf);
    Person::copyStr(buf + len, " is already taken. Please choose another slot.", 200 - len);
    Person::copyStr(message, buf, 200);
}
