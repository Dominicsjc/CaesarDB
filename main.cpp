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
        std::vector<course> curCourses = loginedStu->getCurCourses();

        /*
         * Current course list Demo
         */
        std::cout << "UoSCode      UosName" << std::endl;
        for(const course &c : curCourses){
            std::cout << c.first << "     " << c.second << std::endl;
        }
        std::cout << std::endl;

        //Get transcript TODO: rewrite to API
        //Maybe unordered_map is not necessary because the front-edn can get the key of picked course to show its detail
        std::unordered_map<std::string, std::string> transcript = loginedStu->getTranscript();

        /*
         * Transcript Demo
         */
        std::cout << "UoSCode      Grade" << std::endl;
        for(auto &c : transcript){
            std::cout << c.first << "     " << c.second << std::endl;
        }
        std::cout << std::endl;

        //Free and logout
        delete(loginedStu);
    } else{
        //TODO: Error message should be shown in the front-end and maintain on the login screen
        std::cout << "Username or passowrd is incorrect! Please try again." << std::endl;
        std::cout << std::endl;
    }

    //Close DB
    delete(caesarDB);

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

