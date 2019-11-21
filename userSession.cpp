//
// Created by dominic on 11/20/19.
//

#include "userSession.h"

void userSession::tryLogin() {
    if (name.empty() || password.empty())
        return;
    MYSQL_RES *login_res = db->query(
            "SELECT id FROM student WHERE name = '" + name + "' AND password = '" + password + "';");
    MYSQL_ROW row;
    if (login_res == NULL) {
        std::cerr << "Sometime wrong in the query construction." << std::endl;
        return;
    }
    int numrows = (int) mysql_num_rows(login_res);
    if (numrows != 0) {
        row = mysql_fetch_row(login_res);
        id = atoi(row[0]);
        login = true;
    }
    mysql_free_result(login_res);
}

bool userSession::getLoginStatus() {
    return login;
}