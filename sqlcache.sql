DROP PROCEDURE IF EXISTS enrollProcedure;
DELIMITER $$
CREATE PROCEDURE enrollProcedure (IN id int(11), IN c char(20), IN s char(2), IN y int(11), IN d date, OUT status_code tinyint(1))
BEGIN
	DECLARE pre_c char(8);
    DECLARE pre_req tinyint(1) DEFAULT 1;
    DECLARE done tinyint(1) DEFAULT 0;
    DECLARE c CURSOR FOR (SELECT PrereqUoSCode FROM requires WHERE UoSCode = c AND d > EnforcedSince);
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        -- error
        SET status_code = 1;
        ROLLBACK;
    END;

    DECLARE EXIT HANDLER FOR SQLWARNING
    BEGIN
        -- warning
        SET status_code = 2;
        ROLLBACK;
    END;
    START TRANSACTION;
		-- Unknow terminate
		SET status_code = 7;
        IF EXISTS ( SELECT * FROM transcript WHERE StudId = id AND UoSCode = c AND Semester = s AND Year = y) THEN
			-- Already enrolled
            SET status_code = 3;
		ELSEIF NOT EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND Year = y ) THEN
            -- invalid course
            SET status_code = 4;
        ELSEIF EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND year = y ) AND NOT EXISTS ( SELECT * FROM uosoffering WHERE UoSCode = c AND Semester = s AND year = y AND Enrollment < MaxEnrollment ) THEN
            -- Already full
            SET status_code = 5;
        ELSE
            OPEN c;
            requireLoop: LOOP
				FETCH c INTO pre_c;
                IF done = 1 THEN LEAVE requireLoop; END IF;
                IF NOT EXISTS (SELECT * FROM transcript WHERE StudId = id AND UoSCode = pre_c AND Grade is NOT NULL AND Grade IN ('D', 'P', 'CR')) THEN
                    -- Prerequistes not be cleared
					SET status_code = 6;
                    SET pre_req = 0;
				END IF;
            END LOOP;
            CLOSE c;
            IF pre_req = 0 THEN
                DROP TEMPORARY TABLE IF EXISTS tmp;
				CREATE TEMPORARY TABLE tmp SELECT PrereqUoSCode FROM requires WHERE UoSCode = c AND d > EnforcedSince;
			ELSE
                INSERT INTO transcript VALUES (id, c, s, y, NULL);
                UPDATE uosoffering SET Enrollment = Enrollment + 1 WHERE UoSCode = c AND Semester = s AND Year = y;
                SET status_code = 0;
			END IF;
        END IF;
    COMMIT;

END $$
DELIMITER ;


DROP PROCEDURE IF EXISTS withdrawProcedure;
DELIMITER $$
CREATE PROCEDURE withdrawProcedure(IN id int(11), IN c char(20), IN s char(2), IN y int(11), OUT status_code tinyint(1))
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        -- error
        SET status_code = 1;
        ROLLBACK;
    END;

    DECLARE EXIT HANDLER FOR SQLWARNING
    BEGIN
        -- warning
        SET status_code = 2;
        ROLLBACK;
    END;
    START TRANSACTION;
		IF NOT EXISTS ( SELECT * FROM transcript WHERE StudId = id AND UoSCode = c AND Semester = s AND Year = y AND Grade IS NULL ) THEN
			-- invalid course
            SET status_code = 3;
		ELSE
			DELETE FROM transcript WHERE StudId = id AND UoSCode = c AND Semester = s AND Year = y;
            UPDATE uosoffering SET Enrollment = Enrollment -1 WHERE UoSCode = c AND Semester = s AND Year = y;
            -- success
            SET status_code = 0;
        END IF;
    COMMIT;
END $$
DELIMITER ;

DROP TRIGGER IF EXISTS lowEnrollmentTrigger;
DELIMITER $$
CREATE TRIGGER lowEnrollmentTrigger AFTER UPDATE ON uosoffering
	FOR EACH ROW
    BEGIN
		SET @low = 0;
        IF NEW.Enrollment < NEW.MaxEnrollment / 2 THEN
			SET @low = 1;
		END IF;
    END $$
DELIMITER ;