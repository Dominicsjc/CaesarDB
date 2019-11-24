//
// Created by dominic on 11/20/19.
//

#include "userSession.h"

void userSession::tryLogin(const std::string &username, const std::string &passwd) {
    if (username.empty() || passwd.empty())
        return;

    MYSQL_RES *login_res = db->retrievalQuery(
            "SELECT id FROM student WHERE BINARY Name = '" + username + "' AND BINARY Password = '" + passwd + "';");
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
    std::string curYear = std::to_string(time->tm_year + 1900);
    std::string curMon = std::to_string(time->tm_mon);
    std::string curDay = std::to_string(time->tm_mday);
    std::string curDate = "'" + curYear + "-" + curMon + "-" + curDay + "'";

    if (time->tm_mon < 6) {
        if (year_in != time->tm_year + 1900) {
            status_code = 4;
            return res;
        }
    } else {
        if (semester_in == "Q2") {
            if (year_in != time->tm_year + 1900) {
                status_code = 4;
                return res;
            }
        } else {
            if (year_in != time->tm_year + 1900 + 1) {
                status_code = 4;
                return res;
            }
        }
    }

    bool tryEnroll = db->alterQuery(
            "CALL enrollProcedure(" + std::to_string(id) + ", '" + uoscode_in + "', '" + semester_in + "', " +
            std::to_string(year_in) + ", " +
            curDate + ", @stat);");
    if (!tryEnroll) {
        std::cerr << "Sometime wrong in the call enroll procedure." << std::endl;
        status_code = -1;
        return res;
    }

    MYSQL_RES *enroll_res = db->retrievalQuery("SELECT @stat;");
    if (enroll_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        status_code = -1;
        return res;
    }
    MYSQL_ROW status_row;
    status_row = mysql_fetch_row(enroll_res);
    status_code = atoi(status_row[0]);
    mysql_free_result(enroll_res);
    enroll_res = nullptr;

    if (status_code == 6) {
        MYSQL_RES *pre_res = db->retrievalQuery("SELECT * FROM tmp;");
        if (pre_res == nullptr) {
            std::cerr << "Sometime wrong int the query construction." << std::endl;
            status_code = -1;
            return res;
        }
        MYSQL_ROW pre_row;
        int prenums = (int) mysql_num_rows(pre_res);
        if (prenums) {
            for (int i = 0; i < prenums; i++) {
                pre_row = mysql_fetch_row(pre_res);
                if (pre_row != nullptr)
                    res.emplace_back(pre_row[0]);
            }
        }
        mysql_free_result(pre_res);
    }
    return res;
}

std::vector<course_prog> userSession::getCoursesInProgress() {
    std::vector<course_prog> res;

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

    MYSQL_RES *prog_res = db->retrievalQuery(
            "SELECT UoSCode, Semester, Year FROM transcript WHERE StudId = " + std::to_string(id) +
            " AND Grade IS NULL AND ((Year = " + year + " AND Semester = '" + quarter + "') OR (Year > " + year + "));");
    if (prog_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return {};
    }

    MYSQL_ROW row;
    int numsrow = (int) mysql_num_rows(prog_res);
    if (numsrow) {
        for (int i = 0; i < numsrow; i++) {
            row = mysql_fetch_row(prog_res);
            if (row != nullptr) {
                course_prog c;
                c.code = row[0];
                c.semester = row[1];
                c.year = atoi(row[2]);
                res.emplace_back(c);
            }
        }
    }
    mysql_free_result(prog_res);
    return res;
}

bool userSession::withdrawCourse(const std::string &uoscode_in, const std::string &semester_in, const int &year_in,
                                 int &status_code) {
    bool res = false;

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

    bool tryWithdraw = db->alterQuery(
            "CALL withdrawProcedure(" + std::to_string(id) + ", '" + uoscode_in + "', '" + semester_in + "', " +
            std::to_string(year_in) + ", '" + quarter + "', " + year + ", @stat);");
    if (!tryWithdraw) {
        std::cerr << "Sometime wrong in the call withdraw procedure." << std::endl;
        status_code = -1;
        return res;
    }

    MYSQL_RES *withdraw_res = db->retrievalQuery("SELECT @stat;");
    if (withdraw_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        status_code = -1;
        return res;
    }

    MYSQL_ROW status_row;
    status_row = mysql_fetch_row(withdraw_res);
    status_code = atoi(status_row[0]);
    if (status_code == 0) {
        MYSQL_RES *warning_res = db->retrievalQuery("SELECT @low;");
        if (warning_res == nullptr) {
            std::cerr << "Sometime wrong in the low enrollment check." << std::endl;
            status_code = 4;
            return res;
        }
        MYSQL_ROW warning_row;
        warning_row = mysql_fetch_row(warning_res);
        if (atoi(warning_row[0]) == 1)
            res = true;
    }
    mysql_free_result(withdraw_res);
    return res;
}

profile userSession::getPersonalDetail() {
    profile p;
    p.id = -1;
    p.name = "";
    p.address = "";

    MYSQL_RES *profile_res = db->retrievalQuery(
            "SELECT Name, Address FROM student WHERE Id = " + std::to_string(id) + ";");
    if (profile_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return p;
    }

    MYSQL_ROW profile_row;
    profile_row = mysql_fetch_row(profile_res);
    p.id = id;
    p.name = profile_row[0];
    p.address = profile_row[1];

    return p;
}

void userSession::changePasswd(const std::string &passwd, int &status_code) {
    if (passwd.length() > 10) {
        status_code = 4;
        return;
    }

    bool tryChange = db->alterQuery(
            "CALL changePasswdProcedure(" + std::to_string(id) + ", '" + passwd + "', @stat);");
    if (!tryChange) {
        std::cerr << "Sometime wrong in the call change password procedure." << std::endl;
        status_code = -1;
        return;
    }

    MYSQL_RES *change_res = db->retrievalQuery("SELECT @stat;");
    if (change_res == nullptr) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        status_code = -1;
        return;
    }

    MYSQL_ROW status_row;
    status_row = mysql_fetch_row(change_res);
    status_code = atoi(status_row[0]);

    mysql_free_result(change_res);
}
