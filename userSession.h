//
// Created by dominic on 11/20/19.
//

#ifndef CAESARDB_USERSESSION_H
#define CAESARDB_USERSESSION_H

#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include "dbInstance.h"

typedef std::pair<std::string, std::string> course_pair;

class userSession {
private:
    dbInstance *db;

    int id = -1;
    std::string name;
    std::string password;
    bool login = false;

public:
    userSession() = delete;

    userSession(const std::string &username, const std::string &passwd, dbInstance *mydb) {
        db = mydb;
        name = username;
        password = passwd;
    };

    ~userSession() { db = nullptr; };

    void tryLogin();

    bool getLoginStatus();

    bool getCurCourses(std::vector<std::shared_ptr<course_pair>> &container);

    bool getTranscript(std::vector<std::shared_ptr<course_pair>> &container);

    bool getCourseDetail(const std::string &courseCode, std::vector<std::shared_ptr<std::string>> &container);
};


#endif //CAESARDB_USERSESSION_H
