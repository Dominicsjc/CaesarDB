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
    if(loginedStu){
        //TODO: OK message should be shown in the front-end and enter Student Menu
        std::cout << "Login successfully!" << std::endl;
        std::cout << std::endl;

        //Get current courses TODO: rewrite to API
        std::vector<std::shared_ptr<course>> curCourses;
        loginedStu->getCurCourses(curCourses);
        /*
         * Current course list Demo
         */
        std::cout << "UoSCode      UosName" << std::endl;
        for(std::shared_ptr<course> &c : curCourses){
            std::cout << c->first << "     " << c->second << std::endl;
        }
        std::cout << std::endl;

        //Get transcript TODO: rewrite to API
        //Maybe unordered_map is not necessary because the front-edn can get the key of picked course to show its detail
        std::unordered_map<std::string, std::string> transcript;
        loginedStu->getTranscript(transcript);

        /*
         * Transcript Demo
         */
        std::cout << "UoSCode      Grade" << std::endl;
        for(auto &c : transcript){
            std::cout << c.first << "     " << c.second << std::endl;
        }
        std::cout << std::endl;

        //Get the detail of a arbitrary course TODO: rewrite to API
        std::vector<std::shared_ptr<std::string>> courseDetail;
        loginedStu->getCourseDetail("INFO1103", courseDetail);

        /*
         * Course detail Demo
         */
        std::cout << "UoSCode   UosName   Year   Semester   Enrollment   MaxEnrollment   Lecturer_Name   Grade" << std::endl;
        for(auto &d: courseDetail){
            std::cout << *d << "  ";
        }
        std::cout << std::endl;

        //Free and logout
        delete(loginedStu);
        loginedStu = nullptr;
    } else{
        //TODO: Error message should be shown in the front-end and maintain on the login screen
        std::cout << "Username or passowrd is incorrect! Please try again." << std::endl;
        std::cout << std::endl;
    }

    //Close DB
    delete(caesarDB);
    caesarDB = nullptr;

    return 0;
}

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB) {
    auto *newUser = new userSession(username, passwd, curDB);
    newUser->tryLogin();
    if (newUser->getLoginStatus()) {
        //std::cout << "Login successfully!" << std::endl;
        return newUser;
    } else {
        //std::cout << "Username or passowrd is incorrect! Please try again." << std::endl;
        delete (newUser);
        return nullptr;
    }
}
