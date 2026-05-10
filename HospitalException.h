#pragma once
#include <iostream>

class HospitalException {
protected:
    char message[200];
public:
    HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException() {}
};

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* file);
};

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException();
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException();
};

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException();
};