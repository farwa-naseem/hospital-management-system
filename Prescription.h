#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <iostream>
#include "Person.h"

class Prescription {
private:
    int  prescriptionId;
    int  appointmentId;
    int  patientId;
    int  doctorId;
    char date[20];         // DD-MM-YYYY
    char medicines[500];
    char notes[300];

public:
    Prescription();
    Prescription(int presId, int apptId, int pId, int dId,
        const char* date, const char* medicines, const char* notes);
    ~Prescription();

    // Getters
    int         getPrescriptionId() const;
    int         getAppointmentId()  const;
    int         getPatientId()      const;
    int         getDoctorId()       const;
    const char* getDate()           const;
    const char* getMedicines()      const;
    const char* getNotes()          const;

    // Setters
    void setPrescriptionId(int id);
    void setAppointmentId(int id);
    void setPatientId(int id);
    void setDoctorId(int id);
    void setDate(const char* d);
    void setMedicines(const char* m);
    void setNotes(const char* n);

    friend std::ostream& operator<<(std::ostream& os, const Prescription& p);
};

#endif
