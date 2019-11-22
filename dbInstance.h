//
// Created by dominic on 11/20/19.
//

#ifndef CAESARDB_DBINSTANCE_H
#define CAESARDB_DBINSTANCE_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <memory>

class dbInstance {
private:
    MYSQL *conn;

    void constraintCheckInitial();

public:
    dbInstance() = delete;

    dbInstance(const std::string &host, const std::string &user, const std::string &passwd, const std::string &db);

    ~dbInstance();

    MYSQL_RES *query(const std::string &myQuery);

};


#endif //CAESARDB_DBINSTANCE_H
