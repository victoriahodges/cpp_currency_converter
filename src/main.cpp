#include <iostream>
#include <fstream>
#include <string>

#include "../include/api.h"
#include "../include/database.h"

double convertCurrency(double &base_amount, double &rate);
void error_message(std::string err_msg);

int main()
{
    bool is_test_mode = true;

    std::string api_key;
    std::string response;
    double base_amount;
    double rate;
    double converted_amount;

    const std::string BASE_CURRENCY = "GBP";
    const std::vector<std::string> CURRENCY_LIST = {"EUR", "CHF", "USD"};
    // const std::vector<std::string> CURRENCY_LIST = {};

    // ----- Check API Key is set -----
    if (!isSetAPIKey("CC_API_KEY", api_key))
    {
        error_message("API key not set. Run script: source ./set_api_key.sh");
        return 0;
    }

    // ----- Initialise program -----
    std::cout << "Currency Converter Initialized" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Enter the amount to convert (" << BASE_CURRENCY << "):" << std::endl;
    std::cin >> base_amount;
    std::cout << "==============================" << std::endl;

    // ----- Call API -----
    if (is_test_mode) // to save on API calls
    {
        // get data from test.json
        std::cout << "Getting data from test file..." << std::endl;
        std::ifstream jsonFile("../data/test.json");
        std::ostringstream tmp;
        tmp << jsonFile.rdbuf();
        response = tmp.str();
        jsonFile.close();
    }
    else
    {
        // get data from API
        std::string api_url = "https://api.currencyapi.com/v3/latest?apikey=" + api_key + "&base_currency=" + BASE_CURRENCY;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        if (!performCurlRequest(api_url, response))
        {
            return 0;
        }
        curl_global_cleanup();
    }

    // ----- Parse JSON data -----
    Json::Value root;
    if (parseJsonResponse(response, root))
    {
        const Json::Value rates = root["data"];
        const Json::Value last_update = root["meta"]["last_updated_at"];
        std::vector<std::string> currencies;
        if (CURRENCY_LIST.size() > 0)
        {
            currencies = CURRENCY_LIST;
        }
        else
        {
            // Fetch all currencies
            currencies = rates.getMemberNames();
        }

        // 1. Set up database and tables, if none exists
        std::string sql =
            "CREATE TABLE IF NOT EXISTS currency_data ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "datetime DATETIME NOT NULL, ";
        "base_currency TEXT NOT NULL, ";
        // 2. Create table columns for each currency name
        for (std::string curr : currencies)
        {
            sql += curr + " REAL NOT NULL, ";
        }
        // remove last comma and space
        sql.erase(sql.length() - 2, 2);
        sql += ");";

        createDatabase(sql);
        // TODO:
        // 3. Populate rows for each timestamp since last updated, if time > 24hrs since last fetch

        // ----- Print converted currency data -----
        std::cout << "Last updated: " << last_update << std::endl;
        std::cout << base_amount << ' ' << BASE_CURRENCY << " returns: " << std::endl;
        for (std::string curr : currencies)
        {
            rate = rates[curr]["value"].asDouble();
            converted_amount = convertCurrency(base_amount, rate);
            std::cout << "  " << converted_amount << ' ' << curr << std::endl;
        }
    }

    return 0;
}

double convertCurrency(double &base_amount, double &rate)
{
    double converted_amount;

    converted_amount = base_amount * rate;

    return converted_amount;
}

void error_message(std::string err_msg)
{
    std::cerr << "------------- ERROR! -------------" << std::endl;
    std::cerr << err_msg << std::endl;
    std::cerr << "----------------------------------" << std::endl;
}
