#include "database.h"

// https://medium.com/@crud-boy/how-to-use-sqlite-sdk-in-c-c-62fb47766d20

// Create the database tables
bool createDatabase(const std::string &sql)
{
    bool creation_successful = false;
    sqlite3 *db = openDB("../data/test.db");
    if (db != nullptr)
    {
        if (!createTable(db, sql))
        {
            std::cerr << "Failed to create table." << std::endl;
            creation_successful = false;
        }
        closeDB(db);
    }
    return creation_successful;
}

// Function to open a database and return its handle
sqlite3 *openDB(const std::string &dbName)
{
    sqlite3 *db = nullptr;
    int status = sqlite3_open(dbName.c_str(), &db);
    if (status != SQLITE_OK)
    {
        std::cerr << "Failed to initialize DB" << std::endl;
        sqlite3_close(db);
        return nullptr;
    }
    return db;
}

// Function to close the database
bool closeDB(sqlite3 *db)
{
    return sqlite3_close(db) == SQLITE_OK;
}

// Create a new database table
bool createTable(sqlite3 *db, const std::string &sql)
{
    char *errorMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errorMsg) != SQLITE_OK)
    {
        std::cerr << "SQLite error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return false;
    }
    // std::cout << "Table created successfully." << std::endl;
    return true;
}
