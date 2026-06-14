#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include "Person.h"

class Appointment {
private:
    int  appointmentId;
    int  patientId;
    int  doctorId;
    char date[20];      // DD-MM-YYYY
    char timeSlot[10];  // HH:MM
    char status[20];    // pending | completed | cancelled | noshow

public:
    Appointment();
    Appointment(int aId, int pId, int dId,
        const char* date, const char* slot, const char* status);
    ~Appointment();

    // Getters
    int         getAppointmentId() const;
    int         getPatientId()     const;
    int         getDoctorId()      const;
    const char* getDate()          const;
    const char* getTimeSlot()      const;
    const char* getStatus()        const;

    // Setters
    void setAppointmentId(int id);
    void setPatientId(int id);
    void setDoctorId(int id);
    void setDate(const char* d);
    void setTimeSlot(const char* ts);
    void setStatus(const char* s);

    // Conflict check: same doctor, same date, same slot, neither cancelled
    bool operator==(const Appointment& other) const;

    // Formatted output
    friend std::ostream& operator<<(std::ostream& os, const Appointment& a);

    // Required by Storage<T>::removeById
    int getId() const { return appointmentId; }

    // Date comparison helpers (returns -1, 0, 1)
    static int compareDates(const char* d1, const char* d2);
    // Slot comparison (returns -1, 0, 1)
    static int compareSlots(const char* s1, const char* s2);
};

#endif

