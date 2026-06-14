#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include "Person.h"

class Patient : public Person {
private:
    int   age;
    char  gender[5];    // M / F
    char  contact[15];
    float balance;

public:
    Patient();
    Patient(int id, const char* name, int age, const char* gender,
        const char* contact, const char* password, float balance);
    virtual ~Patient();

    // Getters
    int         getAge()     const;
    const char* getGender()  const;
    const char* getContact() const;
    float       getBalance() const;

    // Setters
    void setAge(int a);
    void setGender(const char* g);
    void setContact(const char* c);
    void setBalance(float b);

    // Required overloads
    Patient& operator+=(float amount);              // Top-up balance
    Patient& operator-=(float amount);              // Deduct from balance
    bool     operator==(const Patient& other) const; // Compare by ID
    friend std::ostream& operator<<(std::ostream& os, const Patient& p);

    // Required pure-virtual implementations
    void displayMenu()        override;
    void displayInfo()  const override;

    // Convenience: getId() required by Storage<Patient>
    int getId() const { return Person::getId(); }
};

#endif

