#include "Prescription.h"

Prescription::Prescription()
    : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
    date[0] = medicines[0] = notes[0] = '\0';
}

Prescription::Prescription(int presId, int apptId, int pId, int dId,
    const char* d, const char* med, const char* n)
    : prescriptionId(presId), appointmentId(apptId), patientId(pId), doctorId(dId) {
    Person::copyStr(date, d, 20);
    Person::copyStr(medicines, med, 500);
    Person::copyStr(notes, n, 300);
}

Prescription::~Prescription() {}

int         Prescription::getPrescriptionId() const { return prescriptionId; }
int         Prescription::getAppointmentId()  const { return appointmentId; }
int         Prescription::getPatientId()      const { return patientId; }
int         Prescription::getDoctorId()       const { return doctorId; }
const char* Prescription::getDate()           const { return date; }
const char* Prescription::getMedicines()      const { return medicines; }
const char* Prescription::getNotes()          const { return notes; }

void Prescription::setPrescriptionId(int id) { prescriptionId = id; }
void Prescription::setAppointmentId(int id) { appointmentId = id; }
void Prescription::setPatientId(int id) { patientId = id; }
void Prescription::setDoctorId(int id) { doctorId = id; }
void Prescription::setDate(const char* d) { Person::copyStr(date, d, 20); }
void Prescription::setMedicines(const char* m) { Person::copyStr(medicines, m, 500); }
void Prescription::setNotes(const char* n) { Person::copyStr(notes, n, 300); }

std::ostream& operator<<(std::ostream& os, const Prescription& p) {
    os << "Date: " << p.date
        << " | Medicines: " << p.medicines
        << " | Notes: " << p.notes;
    return os;
}
