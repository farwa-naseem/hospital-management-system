#include "Patient.h"

Patient::Patient()
    : Person(), age(0), balance(0.0f) {
    gender[0] = contact[0] = '\0';
}

Patient::Patient(int id, const char* nm, int ag, const char* gen,
    const char* cont, const char* pw, float bal)
    : Person(id, nm, pw), age(ag), balance(bal) {
    Person::copyStr(gender, gen, 5);
    Person::copyStr(contact, cont, 15);
}

Patient::~Patient() {}

int         Patient::getAge()     const { return age; }
const char* Patient::getGender()  const { return gender; }
const char* Patient::getContact() const { return contact; }
float       Patient::getBalance() const { return balance; }

void Patient::setAge(int a) { age = a; }
void Patient::setGender(const char* g) { Person::copyStr(gender, g, 5); }
void Patient::setContact(const char* c) { Person::copyStr(contact, c, 15); }
void Patient::setBalance(float b) { balance = b; }

// ─── Operator Overloads ───────────────────────────────────────────────────────

Patient& Patient::operator+=(float amount) {
    balance += amount;
    return *this;
}

Patient& Patient::operator-=(float amount) {
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Patient& p) {
    char balBuf[32];
    Person::floatToStr(p.balance, balBuf, 32, 2);
    os << "ID: " << p.id
        << " | Name: " << p.name
        << " | Age: " << p.age
        << " | Gender: " << p.gender
        << " | Contact: " << p.contact
        << " | Balance: PKR " << balBuf;
    return os;
}

void Patient::displayMenu() {
    std::cout << "\nWelcome, " << name << "\n";
    char balBuf[32];
    Person::floatToStr(balance, balBuf, 32, 2);
    std::cout << "Balance: PKR " << balBuf << "\n";
    std::cout << "========================\n";
    std::cout << "1. Book Appointment\n";
    std::cout << "2. Cancel Appointment\n";
    std::cout << "3. View My Appointments\n";
    std::cout << "4. View My Medical Records\n";
    std::cout << "5. View My Bills\n";
    std::cout << "6. Pay Bill\n";
    std::cout << "7. Top Up Balance\n";
    std::cout << "8. Logout\n";
}

void Patient::displayInfo() const {
    std::cout << *this << "\n";
}
