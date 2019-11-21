//
// Created by dominic on 11/20/19.
//

#ifndef CAESARDB_DBINSTANCE_H
#define CAESARDB_DBINSTANCE_H

#include <mysql.h>
#include <iostream>
#include <string>

class dbInstance {
private:
    MYSQL *conn;

    static void proceduresInitial();

    static void proceduresFree();

public:
    dbInstance() = delete;

    dbInstance(const std::string &host, const std::string &user, const std::string &passwd, const std::string &db);

    ~dbInstance();

    MYSQL_RES *query(const std::string &myQuery);

};


#endif //CAESARDB_DBINSTANCE_H
