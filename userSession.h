//
// Created by dominic on 11/20/19.
//

#ifndef CAESARDB_USERSESSION_H
#define CAESARDB_USERSESSION_H

#include <string>
#include <cstring>
#include "dbInstance.h"

class userSession {
private:
    int id = -1;
    bool login = false;
    std::string name;
    std::string password;
    dbInstance *db;


public:
    userSession() = delete;

    userSession(const std::string &username, const std::string &passwd, dbInstance *mydb) {
        db = mydb;
        name = username;
        password = passwd;
    };

    ~userSession() = default;

    void tryLogin();

    bool getLoginStatus();
};


#endif //CAESARDB_USERSESSION_H
