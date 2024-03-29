#include "dbInstance.h"
#include "userSession.h"

#include <iostream>
#include <string>

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB);

int main(int argc, char *argv[]) {
    //Open DB
    auto *caesarDB = new dbInstance("localhost", "root", "bitdefender123", "project3-nudb");

    //login Demo
    userSession *loginedStu = login("Harry Jenkins", "butterflY", caesarDB);
    if (loginedStu) {
        //TODO: OK message should be shown in the front-end and enter Student Menu
        std::cout << "Login successfully!" << std::endl;
        std::cout << std::endl;

        //Get current courses TODO: rewrite to API
        std::vector<course_pair> curCourses = loginedStu->getCurCourses();
        /*
         * Current course list Demo
         */
        std::cout << "UoSCode      UosName" << std::endl;
        for (course_pair &c : curCourses) {
            std::cout << c.first << "     " << c.second << std::endl;
        }
        std::cout << std::endl;

        //Get transcript TODO: rewrite to API
        std::vector<course_pair> transcript = loginedStu->getTranscript();

        /*
         * Transcript Demo
         */
        std::cout << "UoSCode      Grade" << std::endl;
        for (course_pair &c : transcript) {
            std::cout << c.first << "     " << c.second << std::endl;
        }
        std::cout << std::endl;

        //Get the detail of a arbitrary course TODO: rewrite to API
        std::vector<std::string> courseDetail = loginedStu->getCourseDetail("INFO2110");

        /*
         * Course detail Demo
         */
        std::cout << "UoSCode   UosName   Year   Semester   Enrollment   MaxEnrollment   Lecturer_Name   Grade"
                  << std::endl;
        for (auto &d: courseDetail) {
            std::cout << d << "  ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        //Get specific courses offering TODO: rewrite to API
        std::vector<course_off> coursesOffering = loginedStu->getCoursesOffering();

        /*
         * Courses offering Demo
         */
        std::cout << "UoSCode   UosName   Semester   Year   Enrollment   MaxEnrollment"
                  << std::endl;
        for (auto &c: coursesOffering) {
            std::cout << c.code << "  " << c.name << "  " << c.semester << "  " << c.year << "  " << c.enrollment
                      << "  " << c.maxEnrollment << std::endl;
        }
        std::cout << std::endl;

        /*
         * Enroll course Demo
         */
        int status_code = -1;
        std::vector<std::string> prerequisitesMissing = loginedStu->enrollCourse("COMP5046", "Q1", 2020, status_code);
        switch (status_code) {
            case -1:
                std::cerr << "Not get the status!" << std::endl;
                break;
            case 0:
                std::cout << "Enroll successfully." << std::endl;
                break;
            case 1:
                std::cerr << "SQL error!" << std::endl;
                break;
            case 2:
                std::cerr << "SQL warning." << std::endl;
                break;
            case 3:
                std::cout << "You have enrolled this class of the course!" << std::endl;
                break;
            case 4:
                std::cout << "Please enter correct information of a offering course!" << std::endl;
                break;
            case 5:
                std::cout << "This class of the course have reached the maximum capacity." << std::endl;
                break;
            case 6: {
                std::cout << "Some prerequisites are not be cleared!" << std::endl;
                std::cout << "Prerequisites of the course:" << std::endl;
                for (std::string &s : prerequisitesMissing)
                    std::cout << s << "  ";
                std::cout << std::endl;
                break;
            }
            case 7:
                std::cerr << "Unknown terminate in the procedure!" << std::endl;
                break;
            default:
                std::cerr << "Unknown error!" << std::endl;
        }
        std::cout << std::endl;

        // Get courses in progress TODO: rewrite to API
        std::vector<course_prog> coursesInProg = loginedStu->getCoursesInProgress();

        /*
         * Courses in progress Demo
         */
        std::cout << "UoSCode   Semester   Year"
                  << std::endl;
        for (auto &c: coursesInProg) {
            std::cout << c.code << "  " << c.semester << "         " << c.year << "  " << std::endl;
        }
        std::cout << std::endl;

        /*
         * Withdraw course and low enrollment warning Demo
         */
        status_code = -1;
        bool lowWarning = loginedStu->withdrawCourse("INFO1105", "Q2", 2019, status_code);
        //bool lowWarning = loginedStu->withdrawCourse("INFO3315", "Q2", 2019, status_code);
        switch (status_code) {
            case -1:
                std::cerr << "Not get the status!" << std::endl;
                break;
            case 0: {
                std::cout << "Withdraw successfully." << std::endl;
                if (lowWarning)
                    std::cout << "Warning: The course's enrollment goes below 50% of the MaxEnrollment!" << std::endl;
                break;
            }
            case 1:
                std::cerr << "SQL error!" << std::endl;
                break;
            case 2:
                std::cerr << "SQL warning." << std::endl;
                break;
            case 3:
                std::cout << "Please choose a valid course in progress!" << std::endl;
                break;
            case 4:
                std::cout << "Withdraw successfully but something wrong when checking low enrollment!" << std::endl;
                break;
            default:
                std::cerr << "Unknown error!" << std::endl;
        }
        std::cout << std::endl;

        /*
         * Show personal record Demo
         */
        profile record = loginedStu->getPersonalDetail();
        if (record.id == -1)
            std::cerr << "Not fetch the correct profile!" << std::endl;
        else {
            std::cout << "Id: " << record.id << std::endl;
            std::cout << "Name: " << record.name << std::endl;
            std::cout << "Address: " << record.address << std::endl;
        }
        std::cout << std::endl;

        /*
         * Change password Demo
         */
        status_code = -1;
        loginedStu->changePasswd("butterflY123", status_code);
        switch (status_code) {
            case -1:
                std::cerr << "Not get the status!" << std::endl;
                break;
            case 0:
                std::cout << "Change password successfully." << std::endl;
                break;
            case 1:
                std::cerr << "SQL error!" << std::endl;
                break;
            case 2:
                std::cerr << "SQL warning." << std::endl;
                break;
            case 3:
                std::cout << "Please enter a valid id!" << std::endl;
                break;
            case 4:
                std::cout << "Too long new password! Please use another password and try again." << std::endl;
                break;
            default:
                std::cerr << "Unknown error!" << std::endl;
        }
        std::cout << std::endl;

        /*
         * Change address Demo
         */
        status_code = -1;
        loginedStu->changeAddress("Whitetown123456789101112131415161718192021222324252627282930", status_code);
        switch (status_code) {
            case -1:
                std::cerr << "Not get the status!" << std::endl;
                break;
            case 0:
                std::cout << "Change address successfully." << std::endl;
                break;
            case 1:
                std::cerr << "SQL error!" << std::endl;
                break;
            case 2:
                std::cerr << "SQL warning." << std::endl;
                break;
            case 3:
                std::cout << "Please enter a valid id!" << std::endl;
                break;
            case 4:
                std::cout << "Too long new address! Please use another address and try again." << std::endl;
                break;
            default:
                std::cerr << "Unknown error!" << std::endl;
        }
        std::cout << std::endl;


        //Free and logout
        delete (loginedStu);
        loginedStu = nullptr;
    } else {
        //TODO: Error message should be shown in the front-end and maintain on the login screen
        std::cout << "Username or passowrd is incorrect! Please try again." << std::endl;
        std::cout << std::endl;
    }

    //Close DB
    delete (caesarDB);
    caesarDB = nullptr;

    return 0;
}

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB) {
    auto *newUser = new userSession(username, passwd, curDB);
    //newUser->tryLogin();
    if (!newUser->getLoginStatus()) {
        delete (newUser);
        newUser = nullptr;
    }
    return newUser;
}

