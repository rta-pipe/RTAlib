/*
 ==========================================================================

 Copyright (C) 2018 Giancarlo Zollino
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ==========================================================================
*/

#include <iostream>
#include <vector>
#include <memory>
#include "gtest/gtest.h"

#include "Config.hpp"
#include "ConfigTestFileManager.hpp"
#include "DBConnector.hpp"
#include "MySqlDBConnector.hpp"
#include "RedisDBConnector.hpp"
#include "RTA_DLTEST_DB.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

const char* startString = {
"################################################################\n"
"###               - RTAlib/CXXRTAlib/UnitTest -              ###"
};

string confTestFilePath = "";
string localConfFileTestPath = "";
vector < map <string, string> > randomEventsGenerator(int size);

/*
 ==================================
*                                 *
*         MYSQL TEST CASE         *
*                                 *
 ==================================
*/

TEST(MySqlDBConnector, CorrectConnection) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  EXPECT_TRUE( mySqlDBTest->connect(mux) );

  mux->deleteInstance();
  mySqlDBTest->disconnect();
  myConf->deleteInstance();
  cout << "local test path: " << localConfFileTestPath << endl;
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}


TEST(MySqlDBConnector, FailedConnectionWrongPwd) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["host"]= myConf->file["MySql"]["host"].getString();
  kv["port"]= myConf->file["MySql"]["port"].getString();
  kv["password"]= "djkfhbsal";
  kv["username"]= myConf->file["MySql"]["username"].getString();
  kv["dbname"]= myConf->file["MySql"]["dbname"].getString();
  entry.push_back(kv);
  section["MySql"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath,section);
  myConf->deleteInstance();

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(MySqlDBConnector, FailedConnectionWrongUserName) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["host"]= myConf->file["MySql"]["host"].getString();
  kv["port"]= myConf->file["MySql"]["port"].getString();
  kv["password"]= myConf->file["MySql"]["password"].getString();
  kv["username"]= "sdklfhasjh";
  kv["dbname"]= myConf->file["MySql"]["dbname"].getString();
  entry.push_back(kv);
  section["MySql"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(MySqlDBConnector, FailedConnectionWrongDatabase) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["host"]= myConf->file["MySql"]["host"].getString();
  kv["port"]= myConf->file["MySql"]["port"].getString();
  kv["password"]= myConf->file["MySql"]["password"].getString();
  kv["username"]= myConf->file["MySql"]["username"].getString();
  kv["dbname"]= "dsfljhn";
  entry.push_back(kv);
  section["MySql"] = entry;


  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);
  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(MySqlDBConnector, SelectRowsTestTable) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["host"]= myConf->file["MySql"]["host"].getString();
  kv["port"]= myConf->file["MySql"]["port"].getString();
  kv["password"]= myConf->file["MySql"]["password"].getString();
  kv["username"]= myConf->file["MySql"]["username"].getString();
  kv["dbname"]= myConf->file["MySql"]["dbname"].getString();
  entry.push_back(kv);
  section["MySql"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);
  mySqlDBTest->connect(mux);
  EXPECT_TRUE( mySqlDBTest->executeQuery("SELECT * FROM rtalib_dl_test_table") );

  mySqlDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(MySqlDBConnector, WriteWrongTable) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  mySqlDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_FALSE( mySqlDBTest->insertData("lest_fable", args) );
  mySqlDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(MySqlDBConnector, InsertDataSuccefully) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  mySqlDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( mySqlDBTest->insertData("rtalib_test_table", args) );
  mySqlDBTest->disconnect();
  mux->deleteInstance();

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}


/*
 ==================================
*                                 *
*         REDIS TEST CASE         *
*                                 *
 ==================================
*/


TEST(RedisDBConnector, CorrectConnection) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto redisDBTest = make_shared<RedisDBConnector>(0,localConfFileTestPath);

  EXPECT_TRUE(redisDBTest->connect(mux));

  redisDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}


TEST(RedisDBConnector, InsertDataSuccIndexonFirst) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto redisDBTest = make_shared<RedisDBConnector>(0,localConfFileTestPath);
  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  redisDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}


TEST(RedisDBConnector, InsertDataSuccIndexonMiddle) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto redisDBTest = make_shared<RedisDBConnector>(0,localConfFileTestPath);

  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "5";
  args["b"] = "6";
  args["c"] = "7";
  args["d"] = "8";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  redisDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(RedisDBConnector, InsertDataSuccIndexonLast) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto redisDBTest = make_shared<RedisDBConnector>(0,localConfFileTestPath);

  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "9";
  args["b"] = "10";
  args["c"] = "11";
  args["d"] = "12";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  redisDBTest->disconnect();
  mux->deleteInstance();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);
}

/*
 ==================================
*                                 *
*       RTA_DL_DB TEST CASE       *
*                                 *
 ==================================
*/

TEST(RTA_DL_DB, waitAndCloseMysqlTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("mysql", localConfFileTestPath);

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);
}

TEST(RTA_DL_DB, waitAndCloseRedisTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("redis-basic", localConfFileTestPath);

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}


TEST(RTA_DL_DB, streamingSingleThreadRedisTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "1";
  kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("redis-basic", localConfFileTestPath);

  vector < map < string, string > > events = randomEventsGenerator(1);

  for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, string > currentEvent = *it;

    EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
                                          currentEvent["timerealtt"],
                                          currentEvent["ra_deg"],
                                          currentEvent["dec_deg"],
                                          currentEvent["energy"],
                                          currentEvent["detx"],
                                          currentEvent["dety"],
                                          currentEvent["observationid"],
                                          currentEvent["datarepositoryid"],
                                          currentEvent["mcid"],
                                          currentEvent["insert_time"],
                                          currentEvent["status"] ) );
  }

  rtaTestDb->waitAndClose();

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(RTA_DL_DB, streamingSingleThreadMysqlTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "1";
  kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("mysql", localConfFileTestPath);

  vector < map < string, string > > events = randomEventsGenerator(1);

  for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, string > currentEvent = *it;

    EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
                                          currentEvent["timerealtt"],
                                          currentEvent["ra_deg"],
                                          currentEvent["dec_deg"],
                                          currentEvent["energy"],
                                          currentEvent["detx"],
                                          currentEvent["dety"],
                                          currentEvent["observationid"],
                                          currentEvent["datarepositoryid"],
                                          currentEvent["mcid"],
                                          currentEvent["insert_time"],
                                          currentEvent["status"] ) );
  }

  rtaTestDb->waitAndClose();

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(RTA_DL_DB, streamingMultiThreadMysqlTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "1";
  kv["numberofthreads"]= "2";
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("mysql", localConfFileTestPath);

  vector < map < string, string > > events = randomEventsGenerator(4);

  for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, string > currentEvent = *it;

    EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
                                          currentEvent["timerealtt"],
                                          currentEvent["ra_deg"],
                                          currentEvent["dec_deg"],
                                          currentEvent["energy"],
                                          currentEvent["detx"],
                                          currentEvent["dety"],
                                          currentEvent["observationid"],
                                          currentEvent["datarepositoryid"],
                                          currentEvent["mcid"],
                                          currentEvent["insert_time"],
                                          currentEvent["status"] ) );
  }

  rtaTestDb->waitAndClose();

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(RTA_DL_DB, batchSingleThreadMysqlTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "2";
  kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("redis-basic", localConfFileTestPath);

  vector < map < string, string > > events = randomEventsGenerator(4);

  for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, string > currentEvent = *it;

    EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
                                          currentEvent["timerealtt"],
                                          currentEvent["ra_deg"],
                                          currentEvent["dec_deg"],
                                          currentEvent["energy"],
                                          currentEvent["detx"],
                                          currentEvent["dety"],
                                          currentEvent["observationid"],
                                          currentEvent["datarepositoryid"],
                                          currentEvent["mcid"],
                                          currentEvent["insert_time"],
                                          currentEvent["status"] ) );
  }

  rtaTestDb->waitAndClose();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

TEST(RTA_DL_DB, selectSingleThreadMysqlTest) {

  Mutex* mux = Mutex::getIstance();
  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "2";
  kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,localConfFileTestPath);

  mySqlDBTest->connect(mux);
  string query = "SELECT * FROM " + kv["modelname"];
  cout << query << endl;
  mySqlDBTest->executeQuery(query);

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

// TEST(RTA_DL_DB, batchErrorSingleThreadMysqlTest) {
//
//   Mutex* mux = Mutex::getIstance();
//   Config * myConf;
//   myConf = Config::getIstance(localConfFileTestPath);
//
//   map < string, vector < map < string, string > > > section;
//   vector < map < string, string > > entry;
//   map < string, string>  kv;
//
//   kv["modelname"]= myConf->file["General"]["modelname"].getString();
//   kv["mjdref"]= myConf->file["General"]["modelname"].getString();
//   kv["debug"]= myConf->file["General"]["debug"].getString();
//   kv["batchsize"]= "2";
//   kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
//   entry.push_back(kv);
//   section["General"] = entry;
//
//   myConf->deleteInstance();
//   ConfigTestFileManager::writeConfigFile(section);
//   myConf->deleteInstance();
//
//   auto rtaTestDb = make_shared<RTA_DLTEST_DB>("mysql", localConfFileTestPath);
//
//   vector < map < string, string > > events = randomEventsGenerator(5);
//
//   for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {
//
//     map < string, string > currentEvent = *it;
//
//     EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
//                                           currentEvent["timerealtt"],
//                                           currentEvent["ra_deg"],
//                                           currentEvent["dec_deg"],
//                                           currentEvent["energy"],
//                                           currentEvent["detx"],
//                                           currentEvent["dety"],
//                                           currentEvent["observationid"],
//                                           currentEvent["datarepositoryid"],
//                                           currentEvent["mcid"],
//                                           currentEvent["insert_time"],
//                                           currentEvent["status"] ) );
//
//     if( it == next(next(events.end())) ) {
//       rtaTestDb->waitAndClose();
//     }
//   }
//
//   rtaTestDb->waitAndClose();
//   myConf->deleteInstance();
//   ConfigTestFileManager::writeConfigFile();
//
// }

TEST(RTA_DL_DB, batchSingleThreadRedisTest) {

  Config * myConf;
  myConf = Config::getIstance(localConfFileTestPath);

  map < string, vector < map < string, string > > > section;
  vector < map < string, string > > entry;
  map < string, string>  kv;

  kv["modelname"]= myConf->file["General"]["modelname"].getString();
  kv["mjdref"]= myConf->file["General"]["modelname"].getString();
  kv["debug"]= myConf->file["General"]["debug"].getString();
  kv["batchsize"]= "2";
  kv["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  entry.push_back(kv);
  section["General"] = entry;

  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath, section);
  myConf->deleteInstance();

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("redis-basic", localConfFileTestPath);

  vector < map < string, string > > events = randomEventsGenerator(4);

  for(vector < map < string, string > >::iterator it = events.begin(); it!=events.end(); ++it ) {

    map < string, string > currentEvent = *it;

    EXPECT_TRUE( rtaTestDb->insertEvent(  currentEvent["eventidfits"],
                                          currentEvent["timerealtt"],
                                          currentEvent["ra_deg"],
                                          currentEvent["dec_deg"],
                                          currentEvent["energy"],
                                          currentEvent["detx"],
                                          currentEvent["dety"],
                                          currentEvent["observationid"],
                                          currentEvent["datarepositoryid"],
                                          currentEvent["mcid"],
                                          currentEvent["insert_time"],
                                          currentEvent["status"] ) );
  }

  rtaTestDb->waitAndClose();
  myConf->deleteInstance();
  ConfigTestFileManager::writeConfigFile(localConfFileTestPath);

}

vector < map <string, string> > randomEventsGenerator(int size){

  map <string, string> args;

  vector < map <string, string> > events;

  /*  CREAZIONE EVENTI RANDOM  */
  for(int i=0; i<size; i++){
    args["eventidfits"] =to_string(rand());
    args["timerealtt"] =to_string(rand());
    args["detx"] =to_string(rand());
    args["ra_deg"] =to_string(rand());
    args["dec_deg"] =to_string(rand());
    args["energy"] =to_string(rand());
    args["dety"] =to_string(rand());
    args["observationid"] =to_string(rand());
    args["datarepositoryid"] =to_string(rand());
    args["status"] =to_string(1);
    args["mcid"] =to_string(1);
    args["insert_time"] =to_string(rand());

    events.push_back(args);

  }

  return events;

}



int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);

  cout << startString<< endl;

  char * val = getenv( "RTALIBDIR" );
  string envVar(val);

  confTestFilePath = envVar + "/Configs/rtalibconfig_testing ";
  localConfFileTestPath = envVar + "/CxxRTAlib/TestEnvironment/rtalibconfigUTest";

  string cmd = "cp " + confTestFilePath + localConfFileTestPath ;


  if ( std::system(cmd.c_str()) != 0) {
    exit(EXIT_FAILURE);
  }


  return RUN_ALL_TESTS();


}
