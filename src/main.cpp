#include <iostream>
#include <string>
#include <map>

#include <curl/curl.h>
#include <json/json.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool isSetAPIKey(const char *env_var_name, std::string &api_key);
bool performCurlRequest(const std::string &url, std::string &response);
bool parseJsonResponse(const std::string &jsonResponse, Json::Value &parsedRoot);
double convertCurrency(double &base_amount, double &rate);

int main()
{
    std::string api_key;
    std::string response;
    double base_amount;
    double rate;
    double converted_amount;

    const std::string BASE_CURRENCY = "GBP";
    const std::vector<std::string> CURRENCY_LIST = {"EUR", "CHF", "USD"};

    /* TEST DATA & API URL */
    /*
    const std::map<std::string, double> test_data = {
            {"EUR", 1.19616},
            {"CHF", 1.12681},
            {"USD", 1.31246}
        };
    */
    // std::string api_url = "http://date.jsontest.com/";


    if (isSetAPIKey("CC_API_KEY", api_key)) {
        std::cout << "Currency Converter Initialized" << std::endl;
        std::cout << "==============================" << std::endl;

        std::cout << "Enter the amount to convert (" << BASE_CURRENCY << "):" << std::endl;
        std::cin >> base_amount;

        std::cout << "==============================" << std::endl;
 
        std::string api_url = "https://api.currencyapi.com/v3/latest?apikey=" + api_key + "&base_currency=" + BASE_CURRENCY;
        curl_global_init(CURL_GLOBAL_DEFAULT);

        if (performCurlRequest(api_url, response))
        {
            Json::Value root;
            if (parseJsonResponse(response, root))
            {
                // TEST: std::cout << root << std::endl;
                const Json::Value rates = root["data"];
                const Json::Value last_update = root["meta"]["last_updated_at"];
                std::cout << "Last updated: " << last_update << std::endl;
                std::cout << base_amount << ' ' << BASE_CURRENCY << " returns: " << std::endl;
                for (std::string curr : CURRENCY_LIST)
                {
                    rate = rates[curr]["value"].asDouble();
                    //TEST: rate = test_data.at(curr);

                    converted_amount = convertCurrency(base_amount, rate);
                    std::cout << "  " << converted_amount << ' ' << curr << std::endl;
                }
            }
        }

        curl_global_cleanup();
    }
    else {
        std::cout << "------------- ERROR! -------------" << std::endl;
        std::cout << "Currency Converter API key not set" << std::endl;
        std::cout << "----------------------------------" << std::endl;
    }
    return 0;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

bool isSetAPIKey(const char *env_var_name, std::string &api_key){
    if (const char *env_p = std::getenv(env_var_name)) {
        api_key = env_p;
        return true;
    }
    return false;
}

bool performCurlRequest(const std::string &url, std::string &response)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

bool parseJsonResponse(const std::string &jsonResponse, Json::Value &parsedRoot)
{
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    std::string errs;

    bool parsingSuccessful = reader->parse(jsonResponse.c_str(), jsonResponse.c_str() + jsonResponse.size(), &parsedRoot, &errs);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cerr << "Failed to parse JSON: " << errs << std::endl;
        return false;
    }

    return true;
}

double convertCurrency(double &base_amount, double &rate)
{
    double converted_amount;

    converted_amount = base_amount * rate;

    return converted_amount;
}
