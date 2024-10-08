#include "api.h"

// Check if API key is set as environment variable
bool isSetAPIKey(const char *env_var_name, std::string &api_key)
{
    if (const char *env_p = std::getenv(env_var_name))
    {
        api_key = env_p;
        return true;
    }
    return false;
}

// https://tradermade.com/tutorials/how-to-build-your-first-cpp-rest-api-client
// WriteCallback function handles data retrieved from a web request with the help of curl library
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// Uses the libcurl library to get data from a URL
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

// uses the JsonCpp library to try parsing a JSON response provided as a string
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
