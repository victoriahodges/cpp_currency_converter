#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>


bool createCurrencyDatabase(const std::vector<std::string> &currencies);
bool updateCurrencyDatabase();
sqlite3 *openDB(const std::string &dbName);
bool closeDB(sqlite3 *db);
bool createTable(sqlite3 *db, const std::string &sql);
bool insertData(sqlite3 *db, const std::string& sql);

#endif
