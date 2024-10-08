#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <sqlite3.h>

bool createDatabase(const std::string &sql);
sqlite3 *openDB(const std::string &dbName);
bool closeDB(sqlite3 *db);
bool createTable(sqlite3 *db, const std::string &sql);

#endif