#ifndef API_H
#define API_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool isSetAPIKey(const char *env_var_name, std::string &api_key);
bool performCurlRequest(const std::string &url, std::string &response);
bool parseJsonResponse(const std::string &jsonResponse, Json::Value &parsedRoot);

#endif
