#include "dbInstance.h"
#include "userSession.h"

#include <iostream>
#include <string>
#include <cstdlib>

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB);

void printCurCourses(userSession *stu);

void printTranscript(userSession *stu);

bool printCourseDetail(userSession *stu, const std::string &code);

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
                                }
                                    break;
                                case 2: {
                                    back = true;
                                }
                                    break;
                                default:;
                            }
                        }
                        break;
                    }
                    case 2: {
                        break;
                    }
                    case 3: {
                        break;
                    }
                    case 4: {
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
