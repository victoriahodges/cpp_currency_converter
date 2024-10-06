#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool performCurlRequest(const std::string& url, std::string& response);

int main()
{
    // std::string api_url = "https://openexchangerates.org/api/latest.json?app_id=YOUR_APP_ID";
    std::string api_url = "https://www.google.com";
    std::string response;
    
    std::cout << "Currency Converter Initialized\n";

    curl_global_init(CURL_GLOBAL_DEFAULT);

    performCurlRequest(api_url, response);
    std::cout << response;

    curl_global_cleanup();
    return 0;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool performCurlRequest(const std::string& url, std::string& response) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}
