#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "FileHandler.h"
#include <cstdio>
#include <ctime>

// ─── Internal helpers ─────────────────────────────────────────────────────────

// Split a CSV line into fields. Returns field count.
// Modifies line in-place by replacing commas with '\0'.
// Trims trailing \r, \n, spaces, tabs from every field — robust against
// Windows CRLF, BOMs, and editor artifacts.
int FileHandler::splitCSV(char* line, char** fields, int maxFields) {
    int count = 0;
    fields[count++] = line;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ',' && count < maxFields) {
            line[i] = '\0';
            fields[count++] = line + i + 1;
        }
    }
    // Strip trailing whitespace/newline/CR from every field
    for (int f = 0; f < count; ++f) {
        char* s = fields[f];
        int   L = 0;
        while (s[L] != '\0') ++L;
        while (L > 0 && (s[L - 1] == '\n' || s[L - 1] == '\r' ||
            s[L - 1] == ' ' || s[L - 1] == '\t')) {
            s[--L] = '\0';
        }
        // Also trim leading whitespace by shifting
        int start = 0;
        while (s[start] == ' ' || s[start] == '\t') ++start;
        if (start > 0) {
            int j = 0;
            while (s[start + j] != '\0') { s[j] = s[start + j]; ++j; }
            s[j] = '\0';
        }
    }
    return count;
}

// ─── Build CSV line strings ───────────────────────────────────────────────────

void FileHandler::writePatientLine(char* buf, int sz, const Patient& p) {
    char balBuf[32];
    Person::floatToStr(p.getBalance(), balBuf, 32, 2);
    char idBuf[16], ageBuf[16];
    Person::intToStr(p.getId(), idBuf, 16);
    Person::intToStr(p.getAge(), ageBuf, 16);

    // patient_id,name,age,gender,contact,password,balance
    int pos = 0;
    auto app = [&](const char* s) {
        for (int i = 0; s[i] != '\0' && pos < sz - 2; ++i) buf[pos++] = s[i];
        };
    app(idBuf); buf[pos++] = ',';
    app(p.getName()); buf[pos++] = ',';
    app(ageBuf); buf[pos++] = ',';
    app(p.getGender()); buf[pos++] = ',';
    app(p.getContact()); buf[pos++] = ',';
    app(p.getPassword()); buf[pos++] = ',';
    app(balBuf); buf[pos++] = '\n'; buf[pos] = '\0';
}

void FileHandler::writeDoctorLine(char* buf, int sz, const Doctor& d) {
    char feeBuf[32], idBuf[16];
    Person::floatToStr(d.getFee(), feeBuf, 32, 2);
    Person::intToStr(d.getId(), idBuf, 16);

    // doctor_id,name,specialization,contact,password,fee
    int pos = 0;
    auto app = [&](const char* s) {
        for (int i = 0; s[i] != '\0' && pos < sz - 2; ++i) buf[pos++] = s[i];
        };
    app(idBuf); buf[pos++] = ',';
    app(d.getName()); buf[pos++] = ',';
    app(d.getSpecialization()); buf[pos++] = ',';
    app(d.getContact()); buf[pos++] = ',';
    app(d.getPassword()); buf[pos++] = ',';
    app(feeBuf); buf[pos++] = '\n'; buf[pos] = '\0';
}

void FileHandler::writeAppointmentLine(char* buf, int sz, const Appointment& a) {
    char aIdBuf[16], pIdBuf[16], dIdBuf[16];
    Person::intToStr(a.getAppointmentId(), aIdBuf, 16);
    Person::intToStr(a.getPatientId(), pIdBuf, 16);
    Person::intToStr(a.getDoctorId(), dIdBuf, 16);

    // appointment_id,patient_id,doctor_id,date,time_slot,status
    int pos = 0;
    auto app = [&](const char* s) {
        for (int i = 0; s[i] != '\0' && pos < sz - 2; ++i) buf[pos++] = s[i];
        };
    app(aIdBuf); buf[pos++] = ',';
    app(pIdBuf); buf[pos++] = ',';
    app(dIdBuf); buf[pos++] = ',';
    app(a.getDate()); buf[pos++] = ',';
    app(a.getTimeSlot()); buf[pos++] = ',';
    app(a.getStatus()); buf[pos++] = '\n'; buf[pos] = '\0';
}

void FileHandler::writeBillLine(char* buf, int sz, const Bill& b) {
    char bIdBuf[16], pIdBuf[16], aIdBuf[16], amtBuf[32];
    Person::intToStr(b.getBillId(), bIdBuf, 16);
    Person::intToStr(b.getPatientId(), pIdBuf, 16);
    Person::intToStr(b.getAppointmentId(), aIdBuf, 16);
    Person::floatToStr(b.getAmount(), amtBuf, 32, 2);

    // bill_id,patient_id,appointment_id,amount,status,date
    int pos = 0;
    auto app = [&](const char* s) {
        for (int i = 0; s[i] != '\0' && pos < sz - 2; ++i) buf[pos++] = s[i];
        };
    app(bIdBuf); buf[pos++] = ',';
    app(pIdBuf); buf[pos++] = ',';
    app(aIdBuf); buf[pos++] = ',';
    app(amtBuf); buf[pos++] = ',';
    app(b.getStatus()); buf[pos++] = ',';
    app(b.getDate()); buf[pos++] = '\n'; buf[pos] = '\0';
}

void FileHandler::writePrescriptionLine(char* buf, int sz, const Prescription& pr) {
    char prIdBuf[16], apIdBuf[16], pIdBuf[16], dIdBuf[16];
    Person::intToStr(pr.getPrescriptionId(), prIdBuf, 16);
    Person::intToStr(pr.getAppointmentId(), apIdBuf, 16);
    Person::intToStr(pr.getPatientId(), pIdBuf, 16);
    Person::intToStr(pr.getDoctorId(), dIdBuf, 16);

    // prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes
    int pos = 0;
    auto app = [&](const char* s) {
        for (int i = 0; s[i] != '\0' && pos < sz - 2; ++i) buf[pos++] = s[i];
        };
    app(prIdBuf); buf[pos++] = ',';
    app(apIdBuf); buf[pos++] = ',';
    app(pIdBuf);  buf[pos++] = ',';
    app(dIdBuf);  buf[pos++] = ',';
    app(pr.getDate()); buf[pos++] = ',';
    app(pr.getMedicines()); buf[pos++] = ',';
    app(pr.getNotes()); buf[pos++] = '\n'; buf[pos] = '\0';
}

// ─── Date helpers ─────────────────────────────────────────────────────────────

void FileHandler::getTodayDate(char* buf) {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    // format DD-MM-YYYY
    char tmp[11];
    strftime(tmp, 11, "%d-%m-%Y", now);
    Person::copyStr(buf, tmp, 11);
}

int FileHandler::getCurrentYear() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    return now->tm_year + 1900;
}

// ─── Load functions ───────────────────────────────────────────────────────────

void FileHandler::loadPatients(Storage<Patient>& store) {
    FILE* f = fopen("patients.txt", "r");
    if (!f) return; // file may not exist on first run
    char line[512];
    while (fgets(line, 512, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[10];
        char  copy[512];
        Person::copyStr(copy, line, 512);
        int n = splitCSV(copy, fields, 10);
        if (n < 7) continue;
        Patient p;
        p.setId(Person::strToInt(fields[0]));
        p.setName(fields[1]);
        p.setAge(Person::strToInt(fields[2]));
        p.setGender(fields[3]);
        p.setContact(fields[4]);
        p.setPassword(fields[5]);
        p.setBalance(Person::strToFloat(fields[6]));
        store.add(p);
    }
    fclose(f);
}

void FileHandler::loadDoctors(Storage<Doctor>& store) {
    FILE* f = fopen("doctors.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, 256, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[10];
        char  copy[256];
        Person::copyStr(copy, line, 256);
        int n = splitCSV(copy, fields, 10);
        if (n < 6) continue;
        Doctor d;
        d.setId(Person::strToInt(fields[0]));
        d.setName(fields[1]);
        d.setSpecialization(fields[2]);
        d.setContact(fields[3]);
        d.setPassword(fields[4]);
        d.setFee(Person::strToFloat(fields[5]));
        store.add(d);
    }
    fclose(f);
}

void FileHandler::loadAdmin(Admin& admin) {
    FILE* f = fopen("admin.txt", "r");
    if (!f) throw FileNotFoundException("admin.txt");
    char line[256];
    while (fgets(line, 256, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[5];
        char  copy[256];
        Person::copyStr(copy, line, 256);
        int n = splitCSV(copy, fields, 5);
        if (n < 3) continue;
        admin.setId(Person::strToInt(fields[0]));
        admin.setName(fields[1]);
        admin.setPassword(fields[2]);
        break; // only one admin
    }
    fclose(f);
}

void FileHandler::loadAppointments(Storage<Appointment>& store) {
    FILE* f = fopen("appointments.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, 256, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[10];
        char  copy[256];
        Person::copyStr(copy, line, 256);
        int n = splitCSV(copy, fields, 10);
        if (n < 6) continue;
        Appointment a;
        a.setAppointmentId(Person::strToInt(fields[0]));
        a.setPatientId(Person::strToInt(fields[1]));
        a.setDoctorId(Person::strToInt(fields[2]));
        a.setDate(fields[3]);
        a.setTimeSlot(fields[4]);
        a.setStatus(fields[5]);
        store.add(a);
    }
    fclose(f);
}

void FileHandler::loadBills(Storage<Bill>& store) {
    FILE* f = fopen("bills.txt", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, 256, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[10];
        char  copy[256];
        Person::copyStr(copy, line, 256);
        int n = splitCSV(copy, fields, 10);
        if (n < 6) continue;
        Bill b;
        b.setBillId(Person::strToInt(fields[0]));
        b.setPatientId(Person::strToInt(fields[1]));
        b.setAppointmentId(Person::strToInt(fields[2]));
        b.setAmount(Person::strToFloat(fields[3]));
        b.setStatus(fields[4]);
        b.setDate(fields[5]);
        store.add(b);
    }
    fclose(f);
}

void FileHandler::loadPrescriptions(Storage<Prescription>& store) {
    FILE* f = fopen("prescriptions.txt", "r");
    if (!f) return;
    char line[1024];
    while (fgets(line, 1024, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        char* fields[10];
        char  copy[1024];
        Person::copyStr(copy, line, 1024);
        int n = splitCSV(copy, fields, 10);
        if (n < 7) continue;
        Prescription pr;
        pr.setPrescriptionId(Person::strToInt(fields[0]));
        pr.setAppointmentId(Person::strToInt(fields[1]));
        pr.setPatientId(Person::strToInt(fields[2]));
        pr.setDoctorId(Person::strToInt(fields[3]));
        pr.setDate(fields[4]);
        pr.setMedicines(fields[5]);
        pr.setNotes(fields[6]);
        store.add(pr);
    }
    fclose(f);
}

// ─── Append functions ─────────────────────────────────────────────────────────

void FileHandler::appendPatient(const Patient& p) {
    FILE* f = fopen("patients.txt", "a");
    if (!f) throw FileNotFoundException("patients.txt");
    char buf[512]; writePatientLine(buf, 512, p);
    fputs(buf, f);
    fclose(f);
}

void FileHandler::appendDoctor(const Doctor& d) {
    FILE* f = fopen("doctors.txt", "a");
    if (!f) throw FileNotFoundException("doctors.txt");
    char buf[256]; writeDoctorLine(buf, 256, d);
    fputs(buf, f);
    fclose(f);
}

void FileHandler::appendAppointment(const Appointment& a) {
    FILE* f = fopen("appointments.txt", "a");
    if (!f) throw FileNotFoundException("appointments.txt");
    char buf[256]; writeAppointmentLine(buf, 256, a);
    fputs(buf, f);
    fclose(f);
}

void FileHandler::appendBill(const Bill& b) {
    FILE* f = fopen("bills.txt", "a");
    if (!f) throw FileNotFoundException("bills.txt");
    char buf[256]; writeBillLine(buf, 256, b);
    fputs(buf, f);
    fclose(f);
}

void FileHandler::appendPrescription(const Prescription& pr) {
    FILE* f = fopen("prescriptions.txt", "a");
    if (!f) throw FileNotFoundException("prescriptions.txt");
    char buf[1024]; writePrescriptionLine(buf, 1024, pr);
    fputs(buf, f);
    fclose(f);
}

// ─── Update functions (rewrite whole file) ────────────────────────────────────

void FileHandler::updatePatient(const Storage<Patient>& store) {
    FILE* f = fopen("patients.txt", "w");
    if (!f) throw FileNotFoundException("patients.txt");
    char buf[512];
    for (int i = 0; i < store.size(); ++i) {
        writePatientLine(buf, 512, store.get(i));
        fputs(buf, f);
    }
    fclose(f);
}

void FileHandler::updateDoctor(const Storage<Doctor>& store) {
    FILE* f = fopen("doctors.txt", "w");
    if (!f) throw FileNotFoundException("doctors.txt");
    char buf[256];
    for (int i = 0; i < store.size(); ++i) {
        writeDoctorLine(buf, 256, store.get(i));
        fputs(buf, f);
    }
    fclose(f);
}

void FileHandler::updateAppointment(const Storage<Appointment>& store) {
    FILE* f = fopen("appointments.txt", "w");
    if (!f) throw FileNotFoundException("appointments.txt");
    char buf[256];
    for (int i = 0; i < store.size(); ++i) {
        writeAppointmentLine(buf, 256, store.get(i));
        fputs(buf, f);
    }
    fclose(f);
}

void FileHandler::updateBill(const Storage<Bill>& store) {
    FILE* f = fopen("bills.txt", "w");
    if (!f) throw FileNotFoundException("bills.txt");
    char buf[256];
    for (int i = 0; i < store.size(); ++i) {
        writeBillLine(buf, 256, store.get(i));
        fputs(buf, f);
    }
    fclose(f);
}

void FileHandler::updatePrescription(const Storage<Prescription>& store) {
    FILE* f = fopen("prescriptions.txt", "w");
    if (!f) throw FileNotFoundException("prescriptions.txt");
    char buf[1024];
    for (int i = 0; i < store.size(); ++i) {
        writePrescriptionLine(buf, 1024, store.get(i));
        fputs(buf, f);
    }
    fclose(f);
}

// ─── Delete functions ─────────────────────────────────────────────────────────

void FileHandler::deletePatientById(int id, Storage<Patient>& store) {
    store.removeById(id);
    updatePatient(store);
}

void FileHandler::deleteDoctorById(int id, Storage<Doctor>& store) {
    store.removeById(id);
    updateDoctor(store);
}

void FileHandler::deleteAppointmentById(int id, Storage<Appointment>& store) {
    store.removeById(id);
    updateAppointment(store);
}

void FileHandler::deleteBillById(int id, Storage<Bill>& store) {
    store.removeById(id);
    updateBill(store);
}

// ─── Security Log ─────────────────────────────────────────────────────────────

void FileHandler::logSecurityEvent(const char* role, const char* enteredId,
    const char* result) {
    FILE* f = fopen("security_log.txt", "a");
    if (!f) return;
    // Get timestamp
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    char ts[30];
    strftime(ts, 30, "%d-%m-%Y %H:%M:%S", now);
    // timestamp,role,entered_id,result
    fprintf(f, "%s,%s,%s,%s\n", ts, role, enteredId, result);
    fclose(f);
}

void FileHandler::readSecurityLog() {
    FILE* f = fopen("security_log.txt", "r");
    if (!f) { printf("No security events logged.\n"); return; }
    char line[256];
    bool any = false;
    while (fgets(line, 256, f)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        printf("%s", line);
        any = true;
    }
    if (!any) printf("No security events logged.\n");
    fclose(f);
}

// ─── Archive patient (discharge) ─────────────────────────────────────────────

void FileHandler::archivePatient(const Patient& p,
    const Storage<Appointment>& appts,
    const Storage<Bill>& bills,
    const Storage<Prescription>& prescs,
    int patientId) {
    FILE* f = fopen("discharged.txt", "a");
    if (!f) throw FileNotFoundException("discharged.txt");

    // Write patient record
    char buf[512];
    writePatientLine(buf, 512, p);
    fputs(buf, f);

    // Write all appointments for this patient
    char abuf[256];
    for (int i = 0; i < appts.size(); ++i) {
        if (appts.get(i).getPatientId() == patientId) {
            writeAppointmentLine(abuf, 256, appts.get(i));
            fputs(abuf, f);
        }
    }

    // Write all prescriptions for this patient
    char pbuf[1024];
    for (int i = 0; i < prescs.size(); ++i) {
        if (prescs.get(i).getPatientId() == patientId) {
            writePrescriptionLine(pbuf, 1024, prescs.get(i));
            fputs(pbuf, f);
        }
    }

    // Write all bills for this patient
    char bbuf[256];
    for (int i = 0; i < bills.size(); ++i) {
        if (bills.get(i).getPatientId() == patientId) {
            writeBillLine(bbuf, 256, bills.get(i));
            fputs(bbuf, f);
        }
    }

    fclose(f);
}
