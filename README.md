# Hospital-Management-System-MINECRAFT_THEMED-

This documentation covers the end-to-end development of the Medicore Hospital Management System, a C++ project that evolved from a core logic-based console application to a fully functional Graphical User Interface (GUI) using the SFML 2.6.1 library.

## 1. Project Overview
The system is designed to manage hospital operations across three primary user roles: **Admin**, **Doctor**, and **Patient**. It utilizes Object-Oriented Programming (OOP) principles, manual memory management, and file-handling logic.

### Key Evolution
- **Phase 1 (Core Logic):** Developed 14 header files establishing the storage systems, validators, and entity classes.
- **Phase 2 (GUI Implementation):** Migrated from console I/O to SFML, implementing a state-based navigation system and custom rendering engines all in single file named **GraphicsEngine.cpp**.

---

## 2. System Architecture (The 14 Core Headers)

The backbone of the project consists of 14 modular header files, each handling a specific domain of the hospital ecosystem.

### A. Entity Classes
1. **Person.h**: The base class using inheritance for all users (Name, Age, Gender, Contact).
2. **Admin.h**: Derived from Person; manages system-wide overrides and doctor credentials.
3. **Doctor.h**: Derived from Person; includes Specialty, Fee, and Appointment tracking.
4. **Patient.h**: Derived from Person; tracks Balance and medical history links.
5. **Appointment.h**: Manages the link between Doctors and Patients, including Date, Time, and Status.
6. **Bill.h**: Handles financial transactions, linking Appointment IDs to specific costs and payment statuses.
7. **Prescription.h**: Stores medical advice, medicines, and notes from doctors to patients.

### B. System & Utility Headers
8. **Storage.h**: A custom template-based dynamic array (Vector alternative) used for manual memory management of all entities.
9. **FileHandler.h**: Manages Data Persistence. Reads/Writes all entities to `.txt` files to ensure data is saved between sessions.
10. **LogicSystem.h**: The "Brain" of the app. Handles authentication logic, sorting algorithms for reports, and session management.
11. **Validator.h**: Uses RegEx and string parsing to validate Dates (DD-MM-YYYY), Time Slots, and Numeric inputs.
12. **SecurityLog.h**: Logs every login attempt and critical system change to a dedicated `security_log.txt`.
13. **HospitalException.h**: Custom exception classes (e.g., `InsufficientFundsException`, `InvalidInputException`) for robust error handling.
14. **GraphicsEngine.h**: Declares the main rendering loop and SFML window configurations.

---

## 3. The Logic & Working

### The Template Storage System
I implemented a manual `Storage` template. It uses pointers and deep copying (Rule of Five) to manage arrays of Doctors, Patients, and Appointments. This was critical for learning manual memory management in C++.

### Authentication & Security
- **Tri-Level Login:** Separate authentication logic for Admin, Doctors, and Patients.
- **Lockout Mechanism:** Implemented in `LogicSystem`, the system triggers an 8-second lockout after 3 failed attempts, preventing brute-force attacks.

### Data Synchronization
The `FileHandler` ensures that when a Patient pays a bill or a Doctor writes a prescription, the text files are updated immediately. The GUI then calls "Refresh" functions to reload this data from the files into the UI Text objects.

---

## 4. UI Implementation (SFML)

Moving to SFML involved transforming the console logic into a **State Machine**.

### State-Based Navigation
The project uses an integer `currentState` to determine what the user sees:
- `STATE_HOME`
- `STATE_PATIENT_DASHBOARD`
- `STATE_DOCTOR_VIEW_HISTORY`, etc.

### Isolated UI Modules
I implemented "Isolated Buttons" to ensure that clicking a button on the Dashboard doesn't accidentally trigger a button on the screen underneath it. Each screen (like "View Bills") has its own pagination logic:
1. **Collect matches** for the logged-in ID.
2. **Sort** them by Date.
3. **Display** only 10 items at a time.
4. **Next/Prev** buttons adjust the `currentPage` index and re-read the file.

---

## 5. Development Milestones
1. **Console Draft:** Initial implementation of `Person` hierarchy and file saving.
2. **SFML Migration:** Designing the textures and sprites for the Medicore branded portal. bugs.

---

## 6. Technical Specifications
- **Language:** C++
- **Graphics:** SFML 2.6.1
- **Compiler:** GCC
- **OS:** Windows
