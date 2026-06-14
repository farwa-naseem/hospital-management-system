#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "HospitalException.h"

// ─── FileHandler ──────────────────────────────────────────────────────────────
// The ONLY class permitted to perform file I/O in this system.
// Every read/write goes through here.
class FileHandler {
public:
    // ── Load all entities from files into Storage objects ──────────────────
    static void loadPatients(Storage<Patient>& store);
    static void loadDoctors(Storage<Doctor>& store);
    static void loadAdmin(Admin& admin);
    static void loadAppointments(Storage<Appointment>& store);
    static void loadBills(Storage<Bill>& store);
    static void loadPrescriptions(Storage<Prescription>& store);

    // ── Append a new record to the end of a file ───────────────────────────
    static void appendPatient(const Patient& p);
    static void appendDoctor(const Doctor& d);
    static void appendAppointment(const Appointment& a);
    static void appendBill(const Bill& b);
    static void appendPrescription(const Prescription& pr);

    // ── Update a record by ID (rewrites the whole file) ────────────────────
    static void updatePatient(const Storage<Patient>& store);
    static void updateDoctor(const Storage<Doctor>& store);
    static void updateAppointment(const Storage<Appointment>& store);
    static void updateBill(const Storage<Bill>& store);
    static void updatePrescription(const Storage<Prescription>& store);

    // ── Delete a record by ID (rewrites the whole file minus that record) ──
    static void deletePatientById(int id, Storage<Patient>& store);
    static void deleteDoctorById(int id, Storage<Doctor>& store);
    static void deleteAppointmentById(int id, Storage<Appointment>& store);
    static void deleteBillById(int id, Storage<Bill>& store);

    // ── Security log ──────────────────────────────────────────────────────
    static void logSecurityEvent(const char* role, const char* enteredId,
        const char* result);
    static void readSecurityLog();

    // ── Discharge: archive + purge patient from all files ─────────────────
    static void archivePatient(const Patient& p,
        const Storage<Appointment>& appts,
        const Storage<Bill>& bills,
        const Storage<Prescription>& prescs,
        int patientId);

    // ── Helper to get today's date string in DD-MM-YYYY ───────────────────
    static void getTodayDate(char* buf);   // buf must be at least 11 chars
    static int  getCurrentYear();

private:
    // Internal line-parsing helpers
    static int  splitCSV(char* line, char** fields, int maxFields);
    static void writePatientLine(char* buf, int sz, const Patient& p);
    static void writeDoctorLine(char* buf, int sz, const Doctor& d);
    static void writeAppointmentLine(char* buf, int sz, const Appointment& a);
    static void writeBillLine(char* buf, int sz, const Bill& b);
    static void writePrescriptionLine(char* buf, int sz, const Prescription& pr);
};

#endif

