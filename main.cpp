#include "dbInstance.h"
#include "userSession.h"

#include <iostream>
#include <string>

userSession *login(const std::string &username, const std::string &passwd, dbInstance *curDB);

int main(int argc, char *argv[]) {
    //Open DB
    auto *caesarDB = new dbInstance("localhost", "root", "bitdefender123", "project3-nudb");

    //login Demo
    userSession *LoginedStu = login("Linda Smith", "lunch", caesarDB);
    if(LoginedStu){
        std::cout << "Login successfully!" << std::endl;
        delete(LoginedStu);
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

