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

CREATE USER IF NOT EXISTS rtalib_test_user@'127.0.0.1' IDENTIFIED BY 'x1x2x3';

GRANT ALL PRIVILEGES ON rtalib_test_db.* to 'rtalib_test_user'@'127.0.0.1' identified by 'x1x2x3'
