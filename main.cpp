#include "dbInstance.h"
#include "userSession.h"

#include <iostream>
#include <string>

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB);

int main(int argc, char *argv[]) {
    //Open DB
    auto *caesarDB = new dbInstance("localhost", "root", "bitdefender123", "project3-nudb");

    //login Demo
    userSession *loginedStu = login("Linda Smith", "lunch", caesarDB);
    if(loginedStu){
        std::cout << "Login successfully!" << std::endl;
        //Get current courses TODO: rewrite to API
        std::vector<course> curCourses = loginedStu->getCurCourses();

        //Free and logout
        delete(loginedStu);
    } else{
        std::cout << "Username or passowrd is incorrect! Please try again." << std::endl;
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

