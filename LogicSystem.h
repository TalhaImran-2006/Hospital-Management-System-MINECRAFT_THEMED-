#pragma once
#include "FileHandler.h"
#include "Storage.h"
#include <iostream>

class LogicSystem {
	int id;
	char* password;
public:
	static int tries;
	LogicSystem(int,const char*);
	~LogicSystem();
	bool isExist(const char*,const char*);
	int str_len(const char*);
	bool canCheckAgain();
	bool compareInsensitive(const char*, const char*);
	void failedAttempt();
	char to_lower(char);
	static void sortPrescriptions();
	static void sortAppointments();
	static void sortBills();
	static void sortSecurityLogs();
};