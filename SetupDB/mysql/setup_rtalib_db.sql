CREATE DATABASE IF NOT EXISTS __DBNAME___testing;
CREATE DATABASE IF NOT EXISTS __DBNAME__;


/* User for rtalib tests */
CREATE USER IF NOT EXISTS __TESTUSER__@'__HOST__' IDENTIFIED BY '__TESTUSERPSW__';
GRANT ALL PRIVILEGES ON __DBNAME___testing.* to '__TESTUSER__'@'__HOST__' identified by '__TESTUSERPSW__';

/* User for rtalib */
CREATE USER IF NOT EXISTS __RTALIBUSER__@'__HOST__' IDENTIFIED BY '__RTALIBUSERPSW__';
GRANT ALL PRIVILEGES ON __DBNAME__.* to '__RTALIBUSER__'@'__HOST__' identified by '__RTALIBUSERPSW__';



USE __DBNAME___testing;

/* Table for tests */
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
