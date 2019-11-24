#include "dbInstance.h"
#include "userSession.h"

#include <iostream>
#include <string>
#include <cstdlib>

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB);

void printCurCourses(userSession *stu);

void printTranscript(userSession *stu);

bool printCourseDetail(userSession *stu, const std::string &code);

std::vector<course_off> printOffering(userSession *stu);

void tryEnroll(userSession *stu, const std::string &uoscode_in, const std::string &semester_in, const int &year_in);

std::vector<course_prog> printInprog(userSession *stu);

void tryWithdraw(userSession *stu, const std::string &uoscode_in, const std::string &semester_in, const int &year_in);

void printRecord(userSession *stu);

void tryChangePasswd(userSession *stu, const std::string &newp);

void tryChangeAddress(userSession *stu, const std::string &newa);

int main(int argc, char *argv[]) {
    auto *caesarDB = new dbInstance("localhost", argv[1], argv[2], "project3-nudb");
    std::cout << "Welcome to CaeserDB system! Please first login." << std::endl;
    while (1) {
        system("clear");
        std::cout << "------------------- LOGIN -------------------" << std::endl;
        std::cout << "Enter your username: ";
        std::string username;
        getline(std::cin, username);
        std::cout << "Enter your password: ";
        std::string password;
        getline(std::cin, password);
        std::cout << "Validating..." << std::endl;
        userSession *loginedStu = login(username, password, caesarDB);
        if (loginedStu) {
            while (loginedStu) {
                system("clear");
                std::cout << "------------------- STUDENT MENU -------------------" << std::endl;

                std::cout << "Your current courses list:" << std::endl;
                printCurCourses(loginedStu);

                std::cout << "You can enter the corresponding NUMBER to choose a following option:" << std::endl;
                std::cout << "[1] Transcript" << std::endl;
                std::cout << "[2] Enroll" << std::endl;
                std::cout << "[3] Withdraw" << std::endl;
                std::cout << "[4] Personal Detail" << std::endl;
                std::cout << "[5] Logout" << std::endl;
                int choice = -1;
                while (choice == -1) {
                    std::cout << "Enter your option NUMBER: ";
                    std::cin >> choice;
                    if (choice < 1 || choice > 5) {
                        choice = -1;
                        std::cout << "Please enter a valid option NUMBER!" << std::endl;
                    }
                }
                switch (choice) {
                    case 1: {
                        bool back = false;
                        while (!back) {
                            system("clear");
                            std::cout << "------------------- TRANSCRIPTS -------------------" << std::endl;
                            printTranscript(loginedStu);
                            std::cout << "You can enter the corresponding NUMBER to choose a following option:"
                                      << std::endl;
                            std::cout << "[1] See course details" << std::endl;
                            std::cout << "[2] Return" << std::endl;
                            int tchoice = -1;
                            while (tchoice == -1) {
                                std::cout << "Enter your option NUMBER: ";
                                std::cin >> tchoice;
                                if (tchoice < 1 || tchoice > 2) {
                                    tchoice = -1;
                                    std::cout << "Please enter a valid option NUMBER!" << std::endl;
                                }
                            }
                            switch (tchoice) {
                                case 1: {
                                    while (1) {
                                        std::cout << "Enter the course code you want to check: ";
                                        std::string uoscode;
                                        std::cin >> uoscode;
                                        if (printCourseDetail(loginedStu, uoscode)) {
                                            std::cin.ignore();
                                            std::cout << "Press Enter to return." << std::endl;
                                            std::string tmp;
                                            getline(std::cin, tmp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case 2: {
                                    back = true;
                                    break;
                                }
                                default:;
                            }
                        }
                        break;
                    }
                    case 2: {
                        bool back = false;
                        while (!back) {
                            system("clear");
                            std::cout << "------------------- ENROLL -------------------" << std::endl;
                            std::cout << "Here are courses currently offering:" << std::endl;
                            std::vector<course_off> info = printOffering(loginedStu);
                            std::cout << "You can enter the corresponding NUMBER to choose a following option:"
                                      << std::endl;
                            std::cout << "[1] Enroll a course" << std::endl;
                            std::cout << "[2] Return" << std::endl;
                            int echoice = -1;
                            while (echoice == -1) {
                                std::cout << "Enter your option NUMBER: ";
                                std::cin >> echoice;
                                if (echoice < 1 || echoice > 2) {
                                    echoice = -1;
                                    std::cout << "Please enter a valid option NUMBER!" << std::endl;
                                }
                            }
                            switch (echoice) {
                                case 1: {
                                    while (1) {
                                        std::cout << "Enter the course NO. shown above you want to enroll: ";
                                        int no;
                                        std::cin >> no;
                                        if (no < 1 || no > info.size()) {
                                            std::cout << "Please enter a valid NO.!" << std::endl;
                                        } else {
                                            tryEnroll(loginedStu, info[no - 1].code, info[no - 1].semester,
                                                      info[no - 1].year);
                                            std::cin.ignore();
                                            std::cout << "Press Enter to return." << std::endl;
                                            std::string tmp;
                                            getline(std::cin, tmp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case 2: {
                                    back = true;
                                    break;
                                }
                                default:;
                            }
                        }
                        break;
                    }
                    case 3: {
                        bool back = false;
                        while (!back) {
                            system("clear");
                            std::cout << "------------------- WITHDRAW -------------------" << std::endl;
                            std::cout << "Here are yours courses in progress can be withdrawn:" << std::endl;
                            std::vector<course_prog> info = printInprog(loginedStu);
                            std::cout << "You can enter the corresponding NUMBER to choose a following option:"
                                      << std::endl;
                            std::cout << "[1] Withdraw a course" << std::endl;
                            std::cout << "[2] Return" << std::endl;
                            int wchoice = -1;
                            while (wchoice == -1) {
                                std::cout << "Enter your option NUMBER: ";
                                std::cin >> wchoice;
                                if (wchoice < 1 || wchoice > 2) {
                                    wchoice = -1;
                                    std::cout << "Please enter a valid option NUMBER!" << std::endl;
                                }
                            }
                            switch (wchoice) {
                                case 1: {
                                    while (1) {
                                        std::cout << "Enter the course NO. shown above you want to withdraw: ";
                                        int no;
                                        std::cin >> no;
                                        if (no < 1 || no > info.size()) {
                                            std::cout << "Please enter a valid NO.!" << std::endl;
                                        } else {
                                            tryWithdraw(loginedStu, info[no - 1].code, info[no - 1].semester,
                                                      info[no - 1].year);
                                            std::cin.ignore();
                                            std::cout << "Press Enter to return." << std::endl;
                                            std::string tmp;
                                            getline(std::cin, tmp);
                                            break;
                                        }
                                    }
                                    break;
                                }
                                case 2: {
                                    back = true;
                                    break;
                                }
                                default:;
                            }
                        }
                        break;
                    }
                    case 4: {
                        bool back = false;
                        while (!back) {
                            system("clear");
                            std::cout << "------------------- PERSONAL DETAIL -------------------" << std::endl;
                            std::cout << "Here are your personal record" << std::endl;
                            printRecord(loginedStu);
                            std::cout << "You can enter the corresponding NUMBER to choose a following option:"
                                      << std::endl;
                            std::cout << "[1] Change your password" << std::endl;
                            std::cout << "[2] Change your address" << std::endl;
                            std::cout << "[3] Return" << std::endl;
                            int pchoice = -1;
                            while (pchoice == -1) {
                                std::cout << "Enter your option NUMBER: ";
                                std::cin >> pchoice;
                                if (pchoice < 1 || pchoice > 3) {
                                    pchoice = -1;
                                    std::cout << "Please enter a valid option NUMBER!" << std::endl;
                                }
                            }
                            switch (pchoice) {
                                case 1: {
                                    std::cout << "Enter your new password: ";
                                    std::string passwd;
                                    std::cin.ignore();
                                    getline(std::cin, passwd);
                                    tryChangePasswd(loginedStu, passwd);
                                    std::cout << "Press Enter to return." << std::endl;
                                    std::string tmp;
                                    getline(std::cin, tmp);
                                    break;
                                }
                                case 2: {
                                    std::cout << "Enter your new address: ";
                                    std::string add;
                                    std::cin.ignore();
                                    getline(std::cin, add);
                                    tryChangeAddress(loginedStu, add);
                                    std::cout << "Press Enter to return." << std::endl;
                                    std::string tmp;
                                    getline(std::cin, tmp);
                                    break;
                                }
                                case 3: {
                                    back = true;
                                    break;
                                }
                                default:;
                            }
                        }
                        break;
                    }
                    case 5: {
                        delete (loginedStu);
                        loginedStu = nullptr;
                        std::cin.ignore();
                        break;
                    }
                    default:;
                }
            }
        } else {
            std::cout << "Username or passowrd is incorrect! Please press enter then return to LOGIN." << std::endl;
            std::string tmp;
            getline(std::cin, tmp);
        }
    }

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

void printCurCourses(userSession *stu) {
    std::vector<course_pair> curCourses = stu->getCurCourses();
    std::cout << "UoSCode      UosName" << std::endl;
    for (course_pair &c : curCourses) {
        std::cout << c.first << "     " << c.second << std::endl;
    }
    std::cout << std::endl;
}

void printTranscript(userSession *stu) {
    std::vector<course_pair> transcript = stu->getTranscript();

    std::cout << "UoSCode      Grade" << std::endl;
    for (course_pair &c : transcript) {
        std::cout << c.first << "     " << c.second << std::endl;
    }
    std::cout << std::endl;
}

bool printCourseDetail(userSession *stu, const std::string &code) {
    std::vector<std::string> courseDetail = stu->getCourseDetail(code);
    if (courseDetail.empty()) {
        return false;
    }

    std::cout << std::endl;
    std::cout << "UoSCode   UosName             Year   Semester   Enrollment   MaxEnrollment   Lecturer_Name   Grade"
              << std::endl;
    for (auto &d: courseDetail) {
        std::cout << d << "  ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    return true;
}

std::vector<course_off> printOffering(userSession *stu) {
    std::vector<course_off> coursesOffering = stu->getCoursesOffering();

    std::cout << "    UoSCode   UosName          Semester   Year   Enrollment   MaxEnrollment"
              << std::endl;
    for (int i = 0; i < (int) coursesOffering.size(); i++) {
        course_off &c = coursesOffering[i];
        std::cout << i + 1 << ".  " << c.code << "  " << c.name << "  " << c.semester << "  " << c.year << "  "
                  << c.enrollment
                  << "  " << c.maxEnrollment << std::endl;
    }
    std::cout << std::endl;

    return coursesOffering;
}

void tryEnroll(userSession *stu, const std::string &uoscode_in, const std::string &semester_in, const int &year_in) {
    std::cout << std::endl;
    int status_code = -1;
    std::vector<std::string> prerequisitesMissing = stu->enrollCourse(uoscode_in, semester_in, year_in, status_code);
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
}

std::vector<course_prog> printInprog(userSession *stu){
    std::vector<course_prog> coursesInProg = stu->getCoursesInProgress();

    std::cout << "    UoSCode   Semester   Year"
              << std::endl;
    for (int i = 0; i < (int) coursesInProg.size(); i++) {
        course_prog &c = coursesInProg[i];
        std::cout << i + 1 << ".  " << c.code << "  " << c.semester << "         " << c.year << "  " << std::endl;
    }
    std::cout << std::endl;

    return coursesInProg;
}

void tryWithdraw(userSession *stu, const std::string &uoscode_in, const std::string &semester_in, const int &year_in){
    std::cout << std::endl;
    int status_code = -1;
    bool lowWarning = stu->withdrawCourse(uoscode_in, semester_in, year_in, status_code);
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
}

void printRecord(userSession *stu){
    profile record = stu->getPersonalDetail();
    if (record.id == -1)
        std::cerr << "Not fetch the correct profile!" << std::endl;
    else {
        std::cout << "Id: " << record.id << std::endl;
        std::cout << "Name: " << record.name << std::endl;
        std::cout << "Address: " << record.address << std::endl;
    }
    std::cout << std::endl;
}

void tryChangePasswd(userSession *stu, const std::string &newp){
    int status_code = -1;
    stu->changePasswd(newp, status_code);
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
}

void tryChangeAddress(userSession *stu, const std::string &newa){
    int status_code = -1;
    stu->changeAddress(newa, status_code);
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
}