#include "Doctor.h"

Doctor::Doctor()
    : Person(), fee(0.0f) {
    specialization[0] = contact[0] = '\0';
}

Doctor::Doctor(int id, const char* nm, const char* spec,
    const char* cont, const char* pw, float f)
    : Person(id, nm, pw), fee(f) {
    Person::copyStr(specialization, spec, 55);
    Person::copyStr(contact, cont, 15);
}

Doctor::~Doctor() {}

const char* Doctor::getSpecialization() const { return specialization; }
const char* Doctor::getContact()        const { return contact; }
float       Doctor::getFee()            const { return fee; }

void Doctor::setSpecialization(const char* s) { Person::copyStr(specialization, s, 55); }
void Doctor::setContact(const char* c) { Person::copyStr(contact, c, 15); }
void Doctor::setFee(float f) { fee = f; }

bool Doctor::operator==(const Doctor& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Doctor& d) {
    char feeBuf[32];
    Person::floatToStr(d.fee, feeBuf, 32, 2);
    os << "ID: " << d.id
        << " | Name: " << d.name
        << " | Spec: " << d.specialization
        << " | Contact: " << d.contact
        << " | Fee: PKR " << feeBuf;
    return os;
}

void Doctor::displayMenu() {
    std::cout << "\nWelcome, Dr. " << name
        << " | Specialization: " << specialization << "\n";
    std::cout << "===============================================\n";
    std::cout << "1. View Today's Appointments\n";
    std::cout << "2. Mark Appointment Complete\n";
    std::cout << "3. Mark Appointment No-Show\n";
    std::cout << "4. Write Prescription\n";
    std::cout << "5. View Patient Medical History\n";
    std::cout << "6. Logout\n";
}

void Doctor::displayInfo() const {
    std::cout << *this << "\n";
}
