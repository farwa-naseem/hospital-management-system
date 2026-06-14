#include "Bill.h"

Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0f) {
    status[0] = date[0] = '\0';
}

Bill::Bill(int bId, int pId, int aId, float amt, const char* st, const char* dt)
    : billId(bId), patientId(pId), appointmentId(aId), amount(amt) {
    Person::copyStr(status, st, 20);
    Person::copyStr(date, dt, 20);
}

Bill::~Bill() {}

int         Bill::getBillId()        const { return billId; }
int         Bill::getPatientId()     const { return patientId; }
int         Bill::getAppointmentId() const { return appointmentId; }
float       Bill::getAmount()        const { return amount; }
const char* Bill::getStatus()        const { return status; }
const char* Bill::getDate()          const { return date; }

void Bill::setBillId(int id) { billId = id; }
void Bill::setPatientId(int id) { patientId = id; }
void Bill::setAppointmentId(int id) { appointmentId = id; }
void Bill::setAmount(float a) { amount = a; }
void Bill::setStatus(const char* s) { Person::copyStr(status, s, 20); }
void Bill::setDate(const char* d) { Person::copyStr(date, d, 20); }

std::ostream& operator<<(std::ostream& os, const Bill& b) {
    char amtBuf[32];
    Person::floatToStr(b.amount, amtBuf, 32, 2);
    os << "Bill ID: " << b.billId
        << " | Appt ID: " << b.appointmentId
        << " | Amount: PKR " << amtBuf
        << " | Status: " << b.status
        << " | Date: " << b.date;
    return os;
}
