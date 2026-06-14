# MediCore Hospital Management System
Course:Object Oriented Programming (OOP)  
Section: BCS-2A 
Semester: Spring 2026  
Roll Number: 24L-0990 
Name: Farwa Naseem

GitHub Repository:

---

## Project Description

A complete Hospital Management System for the fictional MediCore Hospital,
built in C++ using Object-Oriented Programming principles and the SFML graphics library for the GUI. 
The system supports three user roles — Patient, Doctor, and Admin — and manages appointments,
prescriptions, billing, and medical records. All data persists across program restarts via `.txt` files.

---

## How to Compile and Run

### Requirements
- Visual Studio 2022 
- SFML 2.6.x library installed and linked
## Default Login Credentials

| Role     | ID | Password   |
|----------|----|-----------|
| Patient  | 1  | pass123    |
| Doctor   | 1  | doc456     |
| Admin    | 1  | admin123   |
## File Structure
| File                  |                          Purpose                                |
|         ------        |---------|
| `Person.h / .cpp` | Abstract base class with shared data and string utility functions |
| `Patient.h / .cpp` | Patient entity (overloads `+=`, `-=`, `==`, `<<`) |
| `Doctor.h / .cpp` | Doctor entity (overloads `==`, `<<`) |
| `Admin.h / .cpp` | Admin entity loaded from `admin.txt` |
| `Appointment.h / .cpp` | Appointment entity (overloads `==` for conflict detection, `<<`) |
| `Bill.h / .cpp` | Bill entity |
| `Prescription.h / .cpp` | Prescription entity |
| `Storage.h` | Generic template container (no `std::vector` used) |
| `FileHandler.h / .cpp` | The only class permitted to perform file I/O |
| `Validator.h / .cpp` | The only class permitted to validate input |
| `HospitalException.h / .cpp` | Exception hierarchy |
| `GUI.h / GUI_Core.cpp / GUI_Screens.cpp` | SFML graphical user interface |
| `main.cpp` | Entry point — only function calls |

---

# Data Files which are auto-created on first run
- `patients.txt` — patient records
- `doctors.txt` — doctor records
- `admin.txt` — admin credentials
- `appointments.txt` — all appointments
- `bills.txt` — all bills
- `prescriptions.txt` — all prescriptions
- `security_log.txt` — failed login attempts
- `discharged.txt` — archived discharged patients
---
# OOP Concepts Used
- Encapsulation
- Inheritance
- Abstraction
- Polymorphism
- Operator Overloading
- Templates
- Exception Handling
- Composition
- Friend Functions
- Static Members
- Const Correctness
- Virtual Destructors

# Patient
- Book / cancel appointments
- View own appointments and medical records
- View bills, pay unpaid bills
- Top up account balance

# Doctor
- View today's appointments
- Mark appointments as completed or no-show
- Write prescriptions for completed appointments
- View patient medical history

# Admin
- Add / remove doctors
- View all patients, doctors, and appointments
- View unpaid bills (with overdue marker)
- Discharge patients (archive their data)
- View security log of failed login attempts
- Generate daily reports

