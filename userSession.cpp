//
// Created by dominic on 11/20/19.
//

#include "userSession.h"

void userSession::tryLogin() {
    if (name.empty() || password.empty())
        return;

    MYSQL_RES *login_res = db->query(
            "SELECT id FROM student WHERE Name = '" + name + "' AND Password = '" + password + "';");
    if (login_res == NULL) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return;
    }

    MYSQL_ROW row;
    int numrows = (int) mysql_num_rows(login_res);
    if (numrows != 0) {
        row = mysql_fetch_row(login_res);
        id = atoi(row[0]);
        login = true;
    }
    mysql_free_result(login_res);
}

bool userSession::getLoginStatus() {
    return login;
}

std::vector<course> userSession::getCurCourses() {
    std::vector<course> res;
    auto timeNow = std::chrono::system_clock::now();
    std::time_t curTime = std::chrono::system_clock::to_time_t(timeNow);
    struct tm *time = localtime(&curTime);
    std::string year = std::to_string(time->tm_year + 1900);
    int month = time->tm_mon;
    std::string quarter;
    if (month >= 0 && month <= 5)
        quarter = "Q1";
    else
        quarter = "Q2";

    MYSQL_RES *courses_res = db->query(
            "SELECT UoSCode, UosName FROM transcript NATURAL JOIN unitofstudy WHERE StudId = " + std::to_string(id) +
            " AND Year = " + year + " AND Semester = '" + quarter + "';");
    if (courses_res == NULL) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(courses_res);
    for(int i=0; i < numsrow; i++){
        row = mysql_fetch_row(courses_res);
        if(row != NULL){
            res.emplace_back(std::make_pair(row[0], row[1]));
        }
    }
    mysql_free_result(courses_res);
    return res;
}

std::unordered_map<std::string, std::string> userSession::getTranscript(){
    std::unordered_map<std::string, std::string> res;

    MYSQL_RES *transcript_res = db->query("SELECT UoSCode, Grade FROM transcript WHERE StudId = " + std::to_string(id) + ";");
    if (transcript_res == NULL) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(transcript_res);
    for(int i=0; i < numsrow; i++){
        row = mysql_fetch_row(transcript_res);
        if(row != NULL){
            std::string code = row[0];
            std::string grade;
            if(row[1])
                grade = row[1];
            else
                grade = "NULL";
            res.emplace(std::make_pair(code, grade));
        }
    }
    mysql_free_result(transcript_res);
    return res;
}