#ifndef DOCTOR_H
#define DOCTOR_H

#include <iostream>
#include "Person.h"

class Doctor : public Person {
private:
    char  specialization[55];
    char  contact[15];
    float fee;

public:
    Doctor();
    Doctor(int id, const char* name, const char* spec,
        const char* contact, const char* password, float fee);
    virtual ~Doctor();

    // Getters
    const char* getSpecialization() const;
    const char* getContact()        const;
    float       getFee()            const;

    // Setters
    void setSpecialization(const char* s);
    void setContact(const char* c);
    void setFee(float f);

    // Required overloads
    bool operator==(const Doctor& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Doctor& d);

    // Pure-virtual implementations
    void displayMenu()        override;
    void displayInfo()  const override;

    // Needed by Storage<Doctor>
    int getId() const { return Person::getId(); }
};

#endif

