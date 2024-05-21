#include <iostream>
#include <Windows.h>
#include <curl/curl.h>
#include <array>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <comdef.h>
#include "Colors.h"
#include "GatherInformation.h"
#include "xorstr.hpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string sendPostRequest(const std::string& url, const std::string& postData) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Only disable this for testing, always verify peer in production!
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Only disable this for testing, always verify host in production!
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            //std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "";
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return readBuffer;
}

bool sendlogin(string username, string password, string siD)
{

    string result = sendPostRequest("http://localhost:8080/login_query", xorstr_("username=") + username + xorstr_("&password=") + password + xorstr_("&hwid=") + siD + xorstr_("&token=") + xorstr_("342IUHRUIWHBNEFUS9FH89")); // XOR THE FUCKING SECRET TOKEN SLEEPY MONKEY

    if (result == xorstr_("success"))
        return true;
    else
        return false;
}


using namespace std;

int main() {

start:

    string username, password;


    cout << " " << endl;

    cout << dye::purple("Scarlett Client") << hue::reset << endl;
    Sleep(2000);
    cout << " " << endl;
    cout << "[Scarlett] Enter your username: ";
    cin >> username;
    cout << "[Scarlett] Enter your password: ";
    cin >> password;
    system("CLS");

    if (!sendlogin(username, password, GetCurrentUserSID()))
        return 0;

    cout << "[Scarlett] Welcome back, " + username << endl;


    system("pause");

    return 0;
}