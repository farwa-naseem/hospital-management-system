#include "Validator.h"
#include "Person.h"   // for strToInt / strToFloat

int Validator::strLen(const char* s) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

bool Validator::isDigit(char c) { return c >= '0' && c <= '9'; }

// ─── Date Validation ─────────────────────────────────────────────────────────
// Expected format: DD-MM-YYYY  (length exactly 10)
bool Validator::validateDate(const char* s, int currentYear) {
    if (strLen(s) != 10) return false;
    if (s[2] != '-' || s[5] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isDigit(s[i])) return false;
    }
    int day = (s[0] - '0') * 10 + (s[1] - '0');
    int month = (s[3] - '0') * 10 + (s[4] - '0');
    int year = (s[6] - '0') * 1000 + (s[7] - '0') * 100 + (s[8] - '0') * 10 + (s[9] - '0');
    if (day < 1 || day   > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < currentYear)     return false;
    return true;
}

// ─── Time Slot Validation ─────────────────────────────────────────────────────
bool Validator::validateTimeSlot(const char* s) {
    // 8 valid slots
    const char* slots[] = {
        "09:00","10:00","11:00","12:00",
        "13:00","14:00","15:00","16:00"
    };
    for (int i = 0; i < 8; ++i) {
        bool match = true;
        for (int j = 0; j < 5; ++j) {
            if (s[j] != slots[i][j]) { match = false; break; }
        }
        if (match && s[5] == '\0') return true;
    }
    return false;
}

// ─── Contact Validation ───────────────────────────────────────────────────────
bool Validator::validateContact(const char* s) {
    if (strLen(s) != 11) return false;
    for (int i = 0; i < 11; ++i)
        if (!isDigit(s[i])) return false;
    return true;
}

// ─── Password Validation ─────────────────────────────────────────────────────
bool Validator::validatePassword(const char* s) {
    return strLen(s) >= 6;
}

// ─── Positive Float Validation ───────────────────────────────────────────────
bool Validator::validatePositiveFloat(const char* s) {
    if (s[0] == '\0') return false;
    int  i = 0;
    bool dot = false;
    bool hasDigit = false;
    for (; s[i] != '\0'; ++i) {
        if (s[i] == '.') {
            if (dot) return false;
            dot = true;
        }
        else if (isDigit(s[i])) {
            hasDigit = true;
        }
        else {
            return false;
        }
    }
    if (!hasDigit) return false;
    float val = Person::strToFloat(s);
    return val > 0.0f;
}

// ─── Positive Int Validation ─────────────────────────────────────────────────
bool Validator::validatePositiveInt(const char* s) {
    if (s[0] == '\0') return false;
    for (int i = 0; s[i] != '\0'; ++i)
        if (!isDigit(s[i])) return false;
    return Person::strToInt(s) > 0;
}

// ─── Menu Choice Validation ───────────────────────────────────────────────────
bool Validator::validateMenuChoice(int choice, int maxOption) {
    return choice >= 1 && choice <= maxOption;
}
