#include "Appointment.h"

// ─── Constructors / Destructor ───────────────────────────────────────────────

Appointment::Appointment()
    : appointmentId(0), patientId(0), doctorId(0) {
    date[0] = timeSlot[0] = status[0] = '\0';
}

Appointment::Appointment(int aId, int pId, int dId,
    const char* d, const char* ts, const char* st)
    : appointmentId(aId), patientId(pId), doctorId(dId) {
    Person::copyStr(date, d, 20);
    Person::copyStr(timeSlot, ts, 10);
    Person::copyStr(status, st, 20);
}

Appointment::~Appointment() {}

// ─── Getters ─────────────────────────────────────────────────────────────────

int         Appointment::getAppointmentId() const { return appointmentId; }
int         Appointment::getPatientId()     const { return patientId; }
int         Appointment::getDoctorId()      const { return doctorId; }
const char* Appointment::getDate()          const { return date; }
const char* Appointment::getTimeSlot()      const { return timeSlot; }
const char* Appointment::getStatus()        const { return status; }

// ─── Setters ─────────────────────────────────────────────────────────────────

void Appointment::setAppointmentId(int id) { appointmentId = id; }
void Appointment::setPatientId(int id) { patientId = id; }
void Appointment::setDoctorId(int id) { doctorId = id; }
void Appointment::setDate(const char* d) { Person::copyStr(date, d, 20); }
void Appointment::setTimeSlot(const char* ts) { Person::copyStr(timeSlot, ts, 10); }
void Appointment::setStatus(const char* s) { Person::copyStr(status, s, 20); }

// ─── Operator == (conflict check) ────────────────────────────────────────────
// Two appointments conflict if: same doctor, same date, same time slot,
// and NEITHER has status "cancelled".
bool Appointment::operator==(const Appointment& other) const {
    if (doctorId != other.doctorId) return false;
    if (Person::compareStr(date, other.date) != 0) return false;
    if (Person::compareStr(timeSlot, other.timeSlot) != 0) return false;
    if (Person::compareStr(status, "cancelled") == 0) return false;
    if (Person::compareStr(other.status, "cancelled") == 0) return false;
    return true;
}

// ─── Operator << ─────────────────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const Appointment& a) {
    os << "Appt ID: " << a.appointmentId
        << " | Patient: " << a.patientId
        << " | Doctor: " << a.doctorId
        << " | Date: " << a.date
        << " | Slot: " << a.timeSlot
        << " | Status: " << a.status;
    return os;
}

// ─── Date / Slot comparison helpers ──────────────────────────────────────────
// Date format: DD-MM-YYYY  → compare YYYY first, then MM, then DD

int Appointment::compareDates(const char* d1, const char* d2) {
    // Parse year (positions 6-9), month (3-4), day (0-1)
    auto parse = [](const char* s, int start, int len) -> int {
        int val = 0;
        for (int i = start; i < start + len; ++i)
            val = val * 10 + (s[i] - '0');
        return val;
        };
    int y1 = parse(d1, 6, 4), y2 = parse(d2, 6, 4);
    if (y1 != y2) return y1 < y2 ? -1 : 1;
    int m1 = parse(d1, 3, 2), m2 = parse(d2, 3, 2);
    if (m1 != m2) return m1 < m2 ? -1 : 1;
    int day1 = parse(d1, 0, 2), day2 = parse(d2, 0, 2);
    if (day1 != day2) return day1 < day2 ? -1 : 1;
    return 0;
}

int Appointment::compareSlots(const char* s1, const char* s2) {
    // HH:MM — compare hour then minute
    auto parseTime = [](const char* s) -> int {
        int h = (s[0] - '0') * 10 + (s[1] - '0');
        int m = (s[3] - '0') * 10 + (s[4] - '0');
        return h * 60 + m;
        };
    int t1 = parseTime(s1), t2 = parseTime(s2);
    return t1 < t2 ? -1 : (t1 > t2 ? 1 : 0);
}
