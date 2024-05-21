#include "httplib.h"
#include <iostream>
#include "Colors.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <winsqlite/winsqlite3.h>
#include <SHA256.h>
#define SECRET_TOKEN "342IUHRUIWHBNEFUS9FH89"
#define __SHA256_H_

std::string GenerateSalt()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    std::stringstream ss;

    for (int i = 0; i < 8; ++i) { // Generate 16 bytes of salt
        int randNum = dist(gen);
        ss << std::hex << std::setw(2) << std::setfill('0') << randNum;
    }
    return ss.str();
}

std::string Hash(std::string input, std::string salt)
{
    std::string salted_input = input + salt;
    const uint8_t* msg = reinterpret_cast<const uint8_t*>(salted_input.c_str());
    uint32_t* digest256 = SHA256Hash(const_cast<uint8_t*>(msg), salted_input.length());
    std::string hashStr;
    char buf[9];
    for (int i = 0; i < SHA256_ARRAY_LEN; ++i) {
        snprintf(buf, sizeof(buf), "%08" PRIx32, digest256[i]);
        hashStr += buf;
    }
    free(digest256);
    return hashStr;
}

std::string rotateString(const std::string& input, int offset)
{
    std::string rotated = input;
    std::rotate(rotated.begin(), rotated.begin() + offset % rotated.size(), rotated.end());
    return rotated;
}

std::string LookInDatabase(std::string input_username, std::string input_password, std::string input_hwid)
{
    bool final_result = false;

    sqlite3* db;
    if (sqlite3_open("C:\\USERDATABASE.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return "false";
    }

    const char* sql = "SELECT Username, HashedPassword, SaltKey, HardwareId FROM users;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "false";
    }

    //std::cout << "Users list:\n";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string hashedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string saltKey = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string hardwareId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        /*
        std::cout << "Username: " << username << ", "
            << "HashedPassword: " << hashedPassword << ", "
            << "SaltKey: " << saltKey << ", "
            << "HardwareId: " << hardwareId << std::endl;

        std::cout << "Compare -> " << input_username << " - " << input_password << " - " << input_hwid << std::endl;
        std::cout << "Password -> " << Hash(input_password, saltKey) << std::endl;
        */
        // check username match then go forward else not
        if (input_username != username)
            return "false";

        // hash the password given with all the salt keys in the database then move forward
        std::string hashedinputpassword = Hash(input_password, saltKey);
        if (hashedinputpassword != hashedPassword)
            return "false";

        // check for hwid match
        if (input_hwid != hardwareId)
            return "false";

        final_result = true;
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    return final_result ? "true" : "false";
}

int main()
{
    httplib::Server svr;

    svr.set_mount_point("/", "");

    std::cout << dye::purple("[ Scarlett Server ] Listening...") << hue::reset << std::endl;

    svr.Post("/login_query", [](const httplib::Request& req, httplib::Response& res) {
        // Extract strings from the posted form data.
        std::string username_req = req.get_param_value("username");
        std::string password_req = req.get_param_value("password");
        std::string hwid_req = req.get_param_value("hwid");
        std::string secretToken_req = req.get_param_value("token");

        std::cout << " " << endl;
        std::cout << dye::light_blue("[ Scarlett Server ] Houston, we have a problem, - Request Received!") << hue::reset << std::endl;

        if (secretToken_req != SECRET_TOKEN)
        {
            res.set_content("Invalid Token", "text/plain");
            return;
        }

        // Check if the Username,HashedPassword,HWID is right
        std::string result_login = LookInDatabase(username_req, password_req, hwid_req);

        string content;

        if (result_login == "true")
            content = "success";
        else
            content = "denied";

        // Send back the result.
        res.set_content(content, "text/plain");
        });

    svr.listen("localhost", 8080);

}