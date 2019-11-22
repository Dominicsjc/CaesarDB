//
// Created by dominic on 11/20/19.
//

#include "userSession.h"

void userSession::tryLogin() {
    if (name.empty() || password.empty())
        return;

    MYSQL_RES *login_res = db->retrievalQuery(
            "SELECT id FROM student WHERE BINARY Name = '" + name + "' AND BINARY Password = '" + password + "';");
    if (login_res == nullptr) {
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

std::vector<course_pair> userSession::getCurCourses() {
    std::vector<course_pair> res;

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

    MYSQL_RES *courses_res = db->retrievalQuery(
            "SELECT UoSCode, UosName FROM transcript NATURAL JOIN unitofstudy WHERE StudId = " + std::to_string(id) +
            " AND Year = " + year + " AND Semester = '" + quarter + "';");
    if (courses_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(courses_res);
    for (int i = 0; i < numsrow; i++) {
        row = mysql_fetch_row(courses_res);
        if (row != nullptr) {
            res.emplace_back(std::make_pair(row[0], row[1]));
        }
    }
    mysql_free_result(courses_res);
    return res;
}

std::vector<course_pair> userSession::getTranscript() {
    std::vector<course_pair> res;

    MYSQL_RES *transcript_res = db->retrievalQuery(
            "SELECT UoSCode, Grade FROM transcript WHERE StudId = " + std::to_string(id) + ";");
    if (transcript_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(transcript_res);
    for (int i = 0; i < numsrow; i++) {
        row = mysql_fetch_row(transcript_res);
        if (row != nullptr) {
            std::string code = row[0];
            std::string grade;
            if (row[1])
                grade = row[1];
            else
                grade = "NULL";
            res.emplace_back(std::make_pair(row[0], grade));
        }
    }
    mysql_free_result(transcript_res);
    return res;
}

std::vector<std::string> userSession::getCourseDetail(const std::string &courseCode) {
    std::vector<std::string> res;

    MYSQL_RES *detail_res = db->retrievalQuery(
            "SELECT UoSCode, UoSName, Year, Semester, Enrollment, MaxEnrollment, Name, Grade FROM transcript NATURAL JOIN unitofstudy NATURAL JOIN uosoffering u INNER JOIN faculty f on (u.InstructorId = f.Id) WHERE UoSCode = '" +
            courseCode + "' AND StudId = " + std::to_string(id) + ";");
    if (detail_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(detail_res);
    if (numsrow != 0) {
        row = mysql_fetch_row(detail_res);
        if (row != nullptr) {
            for (int i = 0; i < 8; i++) {
                std::string str;
                if (i == 7 && !row[7])
                    str = "NULL";
                else
                    str = row[i];
                res.emplace_back(str);
            }
        } else
            return {};
    } else {
        std::cout << "Please enter a valid course code." << std::endl;
        return {};
    }
    mysql_free_result(detail_res);
    return res;
}

std::vector<course_off> userSession::getCoursesOffering() {
    std::vector<course_off> res;

    auto timeNow = std::chrono::system_clock::now();
    std::time_t curTime = std::chrono::system_clock::to_time_t(timeNow);
    struct tm *time = localtime(&curTime);
    std::string year = std::to_string(time->tm_year + 1900);
    std::string nextYear;
    int month = time->tm_mon;
    std::string quarter;
    std::string nextQuarter;
    if (month >= 0 && month <= 5) {
        quarter = "Q1";
        nextQuarter = "Q2";
        nextYear = std::to_string(time->tm_year + 1900);
    } else {
        quarter = "Q2";
        nextQuarter = "Q1";
        nextYear = std::to_string(time->tm_year + 1901);
    }

    MYSQL_RES *offering_res = db->retrievalQuery(
            "SELECT UoSCode, UoSName, Semester, Year, Enrollment, MaxEnrollment FROM unitofstudy NATURAL JOIN uosoffering WHERE (Year = " +
            year + " AND Semester = '" + quarter + "') OR (Year = " + nextYear + " AND Semester = '" + nextQuarter +
            "');");
    if (offering_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(offering_res);
    for (int i = 0; i < numsrow; i++) {
        row = mysql_fetch_row(offering_res);
        if (row != nullptr) {
            course_off c;
            c.code = row[0];
            c.name = row[1];
            c.semester = row[2];
            c.year = atoi(row[3]);
            c.enrollment = atoi(row[4]);
            c.maxEnrollment = atoi(row[5]);
            res.emplace_back(c);
        }
    }
    mysql_free_result(offering_res);
    return res;
}

std::vector<std::string>
userSession::enrollCourse(const std::string &uoscode_in, const std::string &semester_in, const int &year_in,
                          int &status_code) {
    std::vector<std::string> res = {};
    auto timeNow = std::chrono::system_clock::now();
    std::time_t curTime = std::chrono::system_clock::to_time_t(timeNow);
    struct tm *time = localtime(&curTime);
    std::string curYear = std::to_string(time->tm_year + 1900);;
    std::string curMon = std::to_string(time->tm_mon);
    std::string curDay = std::to_string(time->tm_mday);
    std::string curDate = "'" + curYear + "-" + curMon + "-" + curDay + "'";

    MYSQL_RES *enroll_res = db->retrievalQuery(
            "CALL enrollProcedure(" + std::to_string(id) + ", '" + uoscode_in + "', '" + semester_in + "', " +
            std::to_string(year_in) + ", " +
            curDate + ", @stat);");
    if (enroll_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        status_code = -1;
        return res;
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(enroll_res);
    if (numsrow) {
        for (int i = 0; i < numsrow; i++) {
            row = mysql_fetch_row(enroll_res);
            if (row != nullptr)
                res.emplace_back(row[0]);
        }
    }

    MYSQL_RES *status_res = db->retrievalQuery("SELECT @stat;");
    MYSQL_ROW status_row = mysql_fetch_row(status_res);
    mysql_free_result(enroll_res);
    mysql_free_result(status_res);
    return res;
}