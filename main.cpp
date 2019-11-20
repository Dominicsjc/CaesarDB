#include "mysql.h"

#include <iostream>

using namespace std;

MYSQL* conn;

int main(int argc, char* argv[]){
    conn = mysql_init(NULL);

    // open connection
    mysql_real_connect(
            conn,
            "localhost",
            "root",
            "bitdefender123",
            "project3-nudb",
            0,
            NULL,
            CLIENT_MULTI_RESULTS);

    MYSQL_RES* res_set;
    MYSQL_ROW row;
    mysql_query(conn, "SELECT * FROM student;");
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);

    for(int i=0;i<numrows;i++){
        row = mysql_fetch_row(res_set);
        if(row != NULL){
            cout << "ID: " << row[0] << endl;
            cout << "Name: " << row[1] << endl;
        }
    }
    mysql_free_result(res_set);
    // close connection
    mysql_close(conn);
    return 0;
}

