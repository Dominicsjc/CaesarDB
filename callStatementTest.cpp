#include <iostream>
#include <string>
#include <cstring>
#include "mysql.h"

using namespace std;

int main() {
    MYSQL *conn;
    conn = mysql_init(nullptr);
    int status;
    if (mysql_real_connect(
            conn,
            "localhost",
            "root",
            "bitdefender123",
            "project3-nudb",
            0,
            NULL,
            CLIENT_MULTI_RESULTS) == nullptr) {
        std::cerr << "Connect to MySQL failed!" << std::endl;
    }

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    status = mysql_stmt_prepare(stmt, "CALL enrollProcedure(?, ?, ?, ?, ?, ?)", 38);

    MYSQL_BIND ps_params[6];

    int id = 3213;
    char code[9];
    char semester[3];
    int year = 2019;
    MYSQL_TIME ts;
    int stat;

    memset(code, '\0', sizeof(code));
    string code_in = "INFO3402";
    strncpy(code, code_in.c_str(), 8);

    memset(semester, '\0', sizeof(semester));
    string semester_in = "Q2";
    strncpy(semester, semester_in.c_str(), 2);

    ps_params[0].buffer_type = MYSQL_TYPE_LONG;
    ps_params[0].buffer = (void *) &id;
    ps_params[0].buffer_length = sizeof(id);
    ps_params[0].is_null = 0;

    ps_params[1].buffer_type = MYSQL_TYPE_STRING;
    ps_params[1].buffer = (void *) code;
    ps_params[1].buffer_length = 9;
    ps_params[1].is_null = 0;

    ps_params[2].buffer_type = MYSQL_TYPE_STRING;
    ps_params[2].buffer = (void *) semester;
    ps_params[2].buffer_length = 3;
    ps_params[2].is_null = 0;

    ps_params[3].buffer_type = MYSQL_TYPE_LONG;
    ps_params[3].buffer = (void *) &year;
    ps_params[3].buffer_length = sizeof(year);
    ps_params[3].is_null = 0;

    ps_params[4].buffer_type= MYSQL_TYPE_DATE;
    ps_params[4].buffer= (void *)&ts;
    ps_params[4].is_null= 0;
    ps_params[4].length= 0;

    ps_params[5].buffer_type = MYSQL_TYPE_LONG;
    ps_params[5].buffer = (void *) &stat;
    ps_params[5].length = 0;
    ps_params[5].is_null = 0;

    status = mysql_stmt_bind_param(stmt, ps_params);

    ts.year= 2019;
    ts.month= 11;
    ts.day= 22;

    stat = 10;

    status = mysql_stmt_execute(stmt);

    cout << status << endl;

    /* process results until there are no more */
    do {
        int i;
        int num_fields;       /* number of columns in result */
        MYSQL_FIELD *fields;  /* for result set metadata */
        MYSQL_BIND *rs_bind;  /* for output buffers */

        /* the column count is > 0 if there is a result set */
        /* 0 if the result is only the final status packet */
        num_fields = mysql_stmt_field_count(stmt);

        if (num_fields > 0)
        {
            /* there is a result set to fetch */
            printf("Number of columns in result: %d\n", (int) num_fields);

            /* what kind of result set is this? */
            printf("Data: ");
            if(conn->server_status & SERVER_PS_OUT_PARAMS)
                printf("this result set contains OUT/INOUT parameters\n");
            else
                printf("this result set is produced by the procedure\n");

            MYSQL_RES *rs_metadata = mysql_stmt_result_metadata(stmt);

            fields = mysql_fetch_fields(rs_metadata);

            rs_bind = (MYSQL_BIND *) malloc(sizeof (MYSQL_BIND) * num_fields);
            if (!rs_bind)
            {
                printf("Cannot allocate output buffers\n");
                exit(1);
            }
            memset(rs_bind, 0, sizeof (MYSQL_BIND) * num_fields);

            /* set up and bind result set output buffers */

            status = mysql_stmt_bind_result(stmt, rs_bind);

            /* fetch and display result set rows */
            while (1)
            {
                status = mysql_stmt_fetch(stmt);

                if (status == 1 || status == MYSQL_NO_DATA)
                    break;

                for (i = 0; i < num_fields; ++i)
                {
                    cout << rs_bind[i].buffer << endl;
                }
                printf("\n");
            }

            mysql_free_result(rs_metadata); /* free metadata */
            free(rs_bind);                  /* free output buffers */
        }
        else
        {
            /* no columns = final status packet */
            printf("End of procedure output\n");
        }

        /* more results? -1 = no, >0 = error, 0 = yes (keep looking) */
        status = mysql_stmt_next_result(stmt);
    } while (status == 0);

    //mysql_query(conn, "call enrollProcedure(3213, 'INFO3404', 'Q2', 2019, '2019-11-22', @stat);");
    cout << status << endl;
    return 0;
}