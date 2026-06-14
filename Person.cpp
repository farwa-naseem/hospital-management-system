#include "Person.h"

// ─── Constructors / Destructor ───────────────────────────────────────────────

Person::Person() : id(0) {
    name[0] = '\0';
    password[0] = '\0';
}

Person::Person(int id, const char* name, const char* password) : id(id) {
    copyStr(this->name, name, 100);
    copyStr(this->password, password, 100);
}

Person::~Person() {}

// ─── Getters ─────────────────────────────────────────────────────────────────

int         Person::getId()       const { return id; }
const char* Person::getName()     const { return name; }
const char* Person::getPassword() const { return password; }

// ─── Setters ─────────────────────────────────────────────────────────────────

void Person::setId(int i) { id = i; }
void Person::setName(const char* n) { copyStr(name, n, 100); }
void Person::setPassword(const char* p) { copyStr(password, p, 100); }

// ─── Static Utilities ────────────────────────────────────────────────────────

void Person::copyStr(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (i < maxLen - 1 && src[i] != '\0') {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
}

int Person::compareStr(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        ++i;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

int Person::strLen(const char* s) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

char Person::toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

int Person::compareStrCI(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        char la = toLowerChar(a[i]);
        char lb = toLowerChar(b[i]);
        if (la != lb) return (unsigned char)la - (unsigned char)lb;
        ++i;
    }
    return (unsigned char)toLowerChar(a[i]) - (unsigned char)toLowerChar(b[i]);
}

void Person::intToStr(int n, char* buf, int bufSize) {
    if (bufSize <= 0) return;
    bool negative = (n < 0);
    if (negative) n = -n;
    char tmp[32];
    int  idx = 0;
    if (n == 0) { tmp[idx++] = '0'; }
    while (n > 0) { tmp[idx++] = '0' + (n % 10); n /= 10; }
    if (negative) tmp[idx++] = '-';
    // reverse into buf
    int out = 0;
    for (int i = idx - 1; i >= 0 && out < bufSize - 1; --i)
        buf[out++] = tmp[i];
    buf[out] = '\0';
}

void Person::floatToStr(float f, char* buf, int bufSize, int decimals) {
    if (bufSize <= 0) return;
    bool neg = (f < 0);
    if (neg) f = -f;
    int intPart = (int)f;
    float frac = f - intPart;
    // multiply frac by 10^decimals
    int mul = 1;
    for (int i = 0; i < decimals; ++i) mul *= 10;
    int fracPart = (int)(frac * mul + 0.5f);
    // handle carry
    if (fracPart >= mul) { intPart++; fracPart -= mul; }

    char tmp[64];
    int idx = 0;
    if (neg) tmp[idx++] = '-';
    char intBuf[32];
    intToStr(intPart, intBuf, 32);
    for (int i = 0; intBuf[i] != '\0' && idx < 62; ++i) tmp[idx++] = intBuf[i];
    tmp[idx++] = '.';
    // write fracPart with leading zeros
    char fracBuf[32];
    intToStr(fracPart, fracBuf, 32);
    int flen = strLen(fracBuf);
    for (int i = flen; i < decimals && idx < 62; ++i) tmp[idx++] = '0';
    for (int i = 0; fracBuf[i] != '\0' && idx < 62; ++i) tmp[idx++] = fracBuf[i];
    tmp[idx] = '\0';
    copyStr(buf, tmp, bufSize);
}

int Person::strToInt(const char* s) {
    int result = 0;
    bool neg = false;
    int i = 0;
    if (s[0] == '-') { neg = true; i = 1; }
    for (; s[i] != '\0'; ++i) {
        if (s[i] >= '0' && s[i] <= '9') result = result * 10 + (s[i] - '0');
    }
    return neg ? -result : result;
}

float Person::strToFloat(const char* s) {
    float result = 0.0f;
    bool  neg = false;
    int   i = 0;
    if (s[0] == '-') { neg = true; i = 1; }
    for (; s[i] != '\0' && s[i] != '.'; ++i)
        if (s[i] >= '0' && s[i] <= '9') result = result * 10.0f + (s[i] - '0');
    if (s[i] == '.') {
        ++i;
        float divisor = 10.0f;
        for (; s[i] != '\0'; ++i) {
            if (s[i] >= '0' && s[i] <= '9') { result += (s[i] - '0') / divisor; divisor *= 10.0f; }
        }
    }
    return neg ? -result : result;
}

bool Person::isAllDigits(const char* s) {
    if (s[0] == '\0') return false;
    for (int i = 0; s[i] != '\0'; ++i)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

void Person::trim(char* s) {
    int start = 0;
    while (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')
        ++start;
    int len = strLen(s + start);
    int end = len - 1;
    while (end >= 0 && (s[start + end] == ' ' || s[start + end] == '\t' ||
        s[start + end] == '\r' || s[start + end] == '\n'))
        --end;
    int out = 0;
    for (int i = start; i <= start + end; ++i) s[out++] = s[i];
    s[out] = '\0';
}
