#ifndef BILL_H
#define BILL_H

#include <iostream>
#include "Person.h"

class Bill {
private:
    int   billId;
    int   patientId;
    int   appointmentId;
    float amount;
    char  status[20];  // unpaid | paid | cancelled
    char  date[20];    // DD-MM-YYYY

public:
    Bill();
    Bill(int bId, int pId, int aId, float amount,
        const char* status, const char* date);
    ~Bill();

    // Getters
    int         getBillId()        const;
    int         getPatientId()     const;
    int         getAppointmentId() const;
    float       getAmount()        const;
    const char* getStatus()        const;
    const char* getDate()          const;

    // Setters
    void setBillId(int id);
    void setPatientId(int id);
    void setAppointmentId(int id);
    void setAmount(float a);
    void setStatus(const char* s);
    void setDate(const char* d);

    // Required by Storage<T>::removeById
    int getId() const { return billId; }

    friend std::ostream& operator<<(std::ostream& os, const Bill& b);
};

#endif

