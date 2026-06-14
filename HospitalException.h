#ifndef HOSPITAL_EXCEPTION_H
#define HOSPITAL_EXCEPTION_H

#include "Person.h"

// ─── Base Exception ───────────────────────────────────────────────────────────
class HospitalException {
protected:
    char message[200];
public:
    HospitalException();
    explicit HospitalException(const char* msg);
    virtual ~HospitalException();
    virtual const char* what() const;
};

// ─── File Not Found ───────────────────────────────────────────────────────────
class FileNotFoundException : public HospitalException {
public:
    explicit FileNotFoundException(const char* filename);
};

// ─── Insufficient Funds ───────────────────────────────────────────────────────
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException();
    explicit InsufficientFundsException(float required, float available);
};

// ─── Invalid Input ────────────────────────────────────────────────────────────
class InvalidInputException : public HospitalException {
public:
    explicit InvalidInputException(const char* msg);
};

// ─── Slot Unavailable ─────────────────────────────────────────────────────────
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException();
    explicit SlotUnavailableException(const char* slot);
};

#endif

