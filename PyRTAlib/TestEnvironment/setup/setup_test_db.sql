CREATE DATABASE IF NOT EXISTS rtalib_test_db;

USE rtalib_test_db;

CREATE TABLE IF NOT EXISTS rtalib_test_table (
                                          id INT AUTO_INCREMENT PRIMARY KEY,
                                          a INTEGER,
                                          b INTEGER,
                                          c INTEGER,
                                          d INTEGER
                                        );

CREATE TABLE IF NOT EXISTS rtalib_dl_test_table (
                                        evtid INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
                                        eventidfits INTEGER,
                                        observationid INTEGER UNSIGNED,
                                        datarepositoryid INTEGER UNSIGNED,
                                        ra_deg DOUBLE,
                                        dec_deg DOUBLE,
                                        energy DOUBLE,
                                        detx DOUBLE,
                                        dety DOUBLE,
                                        mcid INTEGER,
                                        status INTEGER,
                                        timerealtt DOUBLE,
                                        insert_time INTEGER UNSIGNED
                                      );

SET GLOBAL validate_password_length = 12;
SET GLOBAL validate_password_number_count = 0;

CREATE USER IF NOT EXISTS rtalib_test_user@'localhost' IDENTIFIED BY '#Rtalibtest@2018';

GRANT ALL PRIVILEGES ON rtalib_test_db.* to 'rtalib_test_user'@'localhost' identified by '#Rtalibtest@2018'
