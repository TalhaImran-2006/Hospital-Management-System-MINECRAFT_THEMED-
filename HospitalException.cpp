#include "HospitalException.h"
#include<iostream>
using namespace std;

HospitalException::HospitalException(const char* msg) {
    int i = 0;
    for (; msg && msg[i] != '\0' && i < 199; i++) message[i] = msg[i];
    message[i] = '\0';
}

const char* HospitalException::what() const {
    return message;
}

FileNotFoundException::FileNotFoundException(const char* file)
    : HospitalException("Error: Required file not found: ") {
    int len = 0;
    while (message[len] != '\0') len++;
    for (int i = 0; file[i] != '\0' && len < 199; i++, len++) message[len] = file[i];
    message[len] = '\0';
}

InsufficientFundsException::InsufficientFundsException()
    : HospitalException("Error: Patient has insufficient funds for this transaction.") {
}

InvalidInputException::InvalidInputException()
    : HospitalException("Error: Input failed validation. Please check your data.") {
}

SlotUnavailableException::SlotUnavailableException()
    : HospitalException("Error: This appointment slot is already booked.") {
}