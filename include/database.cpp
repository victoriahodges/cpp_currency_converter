#include "database.h"

// https://medium.com/@crud-boy/how-to-use-sqlite-sdk-in-c-c-62fb47766d20

// Create the database tables
bool createCurrencyDatabase(const std::vector<std::string> &currencies)
{
    sqlite3 *db = openDB("../data/currency.db");
    if (db != nullptr)
    {
        std::string sql =
            "CREATE TABLE IF NOT EXISTS currency_data ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "datetime DATETIME NOT NULL, "
            "base_currency TEXT NOT NULL, ";
        // 2. Create table columns for each currency name
        for (std::string curr : currencies)
        {
            sql += curr + " REAL NOT NULL, ";
        }
        // remove last comma and space
        sql.erase(sql.length() - 2, 2);
        sql += ");";
        
        if (!createTable(db, sql))
        {
            std::cerr << "Failed to create table." << std::endl;
            return false;
        }
        closeDB(db);
    }
    return true;
}

// Update the database
bool updateCurrencyDatabase() {
    return true;
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
    std::cout << "Table created successfully." << std::endl;
    return true;
}

// Insert data into the database
bool insertData(sqlite3 *db, const std::string& sql) {
    sqlite3_stmt *preparedStatement = nullptr;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &preparedStatement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(preparedStatement) == SQLITE_DONE) {
            sqlite3_finalize(preparedStatement);
            return true;
        }
    }
    sqlite3_finalize(preparedStatement);
    return false;
}
