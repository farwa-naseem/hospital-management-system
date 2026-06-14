#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include "Person.h"

class Admin : public Person {
public:
    Admin();
    Admin(int id, const char* name, const char* password);
    virtual ~Admin();

    void displayMenu()        override;
    void displayInfo()  const override;

    int getId() const { return Person::getId(); }
};

#endif

