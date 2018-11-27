/* Table for tests */
CREATE TABLE IF NOT EXISTS rtalib_test_table (
                                          id INT AUTO_INCREMENT PRIMARY KEY,
                                          a INTEGER,
                                          b INTEGER,
                                          c INTEGER,
                                          d INTEGER
                                        );

CREATE TABLE IF NOT EXISTS rtalib_dl_test_table (
                                        evtid BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
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
