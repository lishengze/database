#ifndef _BASIC_FUNC_H_
#define _BASIC_FUNC_H_
#include <string>
using std::string;

void testAdo();

string getConnectStr(string userid, string pwd, string databaseName);

void testGetConnectStr();

void outputDatabaseDrivers();

void testODBC();
#endif
