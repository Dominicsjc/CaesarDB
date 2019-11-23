//
// Created by dominic on 11/20/19.
//

#include "dbInstance.h"

dbInstance::dbInstance(const std::string &host, const std::string &user, const std::string &passwd,
                       const std::string &db) {
    conn = mysql_init(nullptr);
    if (mysql_real_connect(
            conn,
            host.c_str(),
            user.c_str(),
            passwd.c_str(),
            db.c_str(),
            0,
            NULL,
            CLIENT_MULTI_RESULTS) == nullptr) {
        std::cerr << "Connect to MySQL failed!" << std::endl;
    }
    proceduresInitial();
}

dbInstance::~dbInstance() {
    mysql_close(conn);
    conn = nullptr;
}

MYSQL_RES *dbInstance::retrievalQuery(const std::string &myQuery) {
    MYSQL_RES *res_set;
    const char *tmpQ = myQuery.c_str();
    if(mysql_query(conn, tmpQ) != 0)
        return nullptr;
    else
        res_set = mysql_store_result(conn);
    return res_set;
}

bool dbInstance::alterQuery(const std::string &myQuery){
    const char *tmpQ = myQuery.c_str();
    return mysql_query(conn, tmpQ) == 0;
}

void dbInstance::proceduresInitial() {
    enrollInitial();
    withdrawInitial();
}

void dbInstance::enrollInitial() {
    if(!this->alterQuery("DROP PROCEDURE IF EXISTS enrollProcedure;")){
        std::cerr << "Sometime wrong in the enrollProcedure drop." << std::endl;
        return;
    }

    std::string procedureQuery = "CREATE PROCEDURE enrollProcedure (IN id int(11), IN c char(20), IN s char(2), IN y int(11), IN d date, OUT status_code tinyint(1))\n"
                                 "BEGIN\n"
                                 "\tDECLARE pre_c char(8);\n"
                                 "    DECLARE pre_req tinyint(1) DEFAULT 1;\n"
                                 "    DECLARE done tinyint(1) DEFAULT 0;\n"
                                 "    DECLARE c CURSOR FOR (SELECT PrereqUoSCode FROM requires WHERE UoSCode = c AND d > EnforcedSince);\n"
                                 "    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;\n"
                                 "    DECLARE EXIT HANDLER FOR SQLEXCEPTION\n"
                                 "    BEGIN\n"
                                 "        -- error\n"
                                 "        SET status_code = 1;\n"
                                 "        ROLLBACK;\n"
                                 "    END;\n"
                                 "\n"
                                 "    DECLARE EXIT HANDLER FOR SQLWARNING\n"
                                 "    BEGIN\n"
                                 "        -- warning\n"
                                 "        SET status_code = 2;\n"
                                 "        ROLLBACK;\n"
                                 "    END;\n"
                                 "    START TRANSACTION;\n"
                                 "\t\t-- Unknow terminate\n"
                                 "\t\tSET status_code = 7;\n"
                                 "        IF EXISTS ( SELECT * FROM transcript WHERE StudId = id AND UoSCode = c AND Semester = s AND Year = y) THEN\n"
                                 "\t\t\t-- Already enrolled\n"
                                 "            SET status_code = 3;\n"
                                 "\t\tELSEIF NOT EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND Year = y ) THEN\n"
                                 "            -- invalid course\n"
                                 "            SET status_code = 4;\n"
                                 "        ELSEIF EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND year = y ) AND NOT EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND year = y AND Enrollment < MaxEnrollment ) THEN\n"
                                 "            -- Already full\n"
                                 "            SET status_code = 5;\n"
                                 "        ELSE\n"
                                 "            OPEN c;\n"
                                 "            requireLoop: LOOP\n"
                                 "\t\t\t\tFETCH c INTO pre_c;\n"
                                 "                IF done = 1 THEN LEAVE requireLoop; END IF;\n"
                                 "                IF NOT EXISTS (SELECT * FROM transcript WHERE StudId = id AND UoSCode = pre_c AND Grade is NOT NULL AND Grade IN ('D', 'P', 'CR')) THEN\n"
                                 "                    -- Prerequistes not be cleared\n"
                                 "\t\t\t\t\tSET status_code = 6;\n"
                                 "                    SET pre_req = 0;\n"
                                 "\t\t\t\tEND IF;\n"
                                 "            END LOOP;\n"
                                 "            CLOSE c;\n"
                                 "            IF pre_req = 0 THEN\n"
                                 "                DROP TEMPORARY TABLE IF EXISTS tmp;\n"
                                 "\t\t\t\tCREATE TEMPORARY TABLE tmp SELECT PrereqUoSCode FROM requires WHERE UoSCode = c AND d > EnforcedSince;\n"
                                 "\t\t\tELSE\n"
                                 "                INSERT INTO transcript VALUES (id, c, s, y, NULL);\n"
                                 "                UPDATE uosoffering SET Enrollment = Enrollment + 1 WHERE UoSCode = c AND Semester = s AND Year = y;\n"
                                 "                SET status_code = 0;\n"
                                 "\t\t\tEND IF;\n"
                                 "        END IF;\n"
                                 "    COMMIT;\n"
                                 "\n"
                                 "END";
    if(!this->alterQuery(procedureQuery)) {
        std::cerr << "Sometime wrong in the enrollProcedure create." << std::endl;
        return;
    }
}

void dbInstance::withdrawInitial() {
    if(!this->alterQuery("DROP PROCEDURE IF EXISTS withdrawProcedure;")){
        std::cerr << "Sometime wrong in the withdrawProcedure drop." << std::endl;
        return;
    }
    std::string procedureQuery = "???";
    if(!this->alterQuery(procedureQuery)) {
        std::cerr << "Sometime wrong in the withdrawProcedure create." << std::endl;
        return;
    }
}