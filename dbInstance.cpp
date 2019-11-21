//
// Created by dominic on 11/20/19.
//

#include "dbInstance.h"

dbInstance::dbInstance(const std::string &host, const std::string &user, const std::string &passwd,
                       const std::string &db) {
    conn = mysql_init(NULL);
    if (mysql_real_connect(
            conn,
            host.c_str(),
            user.c_str(),
            passwd.c_str(),
            db.c_str(),
            0,
            NULL,
            CLIENT_MULTI_RESULTS) == NULL) {
        std::cerr << "Connect to MySQL failed!" << std::endl;
    }
    proceduresInitial();
}

dbInstance::~dbInstance() {
    proceduresFree();
    mysql_close(conn);
    conn = nullptr;
}

void dbInstance::proceduresInitial() {

}

void dbInstance::proceduresFree() {

}

MYSQL_RES *dbInstance::query(const std::string &myQuery) {
    MYSQL_RES *res_set;
    const char *tmpQ = myQuery.c_str();
    mysql_query(conn, tmpQ);
    res_set = mysql_store_result(conn);
    return res_set;
}