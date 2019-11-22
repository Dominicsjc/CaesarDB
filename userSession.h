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

typedef struct course_offering {
    std::string code;
    std::string name;
    std::string semester;
    int year;
    int enrollment;
    int maxEnrollment;
} course_off;

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

    std::vector<course_pair> getCurCourses();

    std::vector<course_pair> getTranscript();

    std::vector<std::string> getCourseDetail(const std::string &courseCode);

    std::vector<course_off> getCoursesOffering();

    // TODO: Explain means of different status_code
    std::vector<std::string> enrollCourse(const std::string &uoscode_in, const std::string &semester_in, const int &year_in, int &status_code);
};


#endif //CAESARDB_USERSESSION_H
