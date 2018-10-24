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


void writeConfFile(Config * myConf, string modelname, string mjdref, string debug, string batchsize, string numberofthread,string mhost, string mpwd, string musr, string mdb, string rhost, string rpwd, string rdb, string indexon );

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

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  EXPECT_TRUE( mySqlDBTest->connect(mux) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");


}


TEST(MySqlDBConnector, FailedConnectionWrongPwd) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string r_password = myConf->file["Redis"]["password"].getString();

  string m_password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost","asdasd","rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");



}

TEST(MySqlDBConnector, FailedConnectionWrongUserName) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", m_password,"fkajhuis","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

}

TEST(MySqlDBConnector, FailedConnectionWrongDatabase) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","qwerty","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  EXPECT_FALSE( mySqlDBTest->connect(mux) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

}

TEST(MySqlDBConnector, SelectRowsTestTable) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  mySqlDBTest->connect(mux);

  // EXPECT_TRUE( mySqlDBTest->executeQuery("SELECT * FROM test_table") );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, WriteWrongTable) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  mySqlDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_FALSE( mySqlDBTest->insertData("lest_fable", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

}

TEST(MySqlDBConnector, InsertDataSuccefully) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"test_table","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto mySqlDBTest = make_shared<MySqlDBConnector>(0,"./");

  mySqlDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( mySqlDBTest->insertData("test_table", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

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

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto redisDBTest = make_shared<RedisDBConnector>(0,"./");

  EXPECT_TRUE(redisDBTest->connect(mux));

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

}


TEST(RedisDBConnector, InsertDataSuccIndexonFirst) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:a");

  auto redisDBTest = make_shared<RedisDBConnector>(0,"./");

  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}


TEST(RedisDBConnector, InsertDataSuccIndexonMiddle) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:c");

  auto redisDBTest = make_shared<RedisDBConnector>(0,"./");

  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "5";
  args["b"] = "6";
  args["c"] = "7";
  args["d"] = "8";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(RedisDBConnector, InsertDataSuccIndexonLast) {

  Mutex* mux = Mutex::getIstance();

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:d");

  auto redisDBTest = make_shared<RedisDBConnector>(0,"./");

  redisDBTest->connect(mux);

  map <string, string > args;
  args["a"] = "9";
  args["b"] = "10";
  args["c"] = "11";
  args["d"] = "12";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

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

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("mysql", "./");

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

}

TEST(RTA_DL_DB, waitAndCloseRedisTest) {

  Config * myConf;

  myConf = Config::getIstance("./");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  auto rtaTestDb = make_shared<RTA_DLTEST_DB>("redis-basic", "./");

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}


void writeConfFile( Config * myConf,
                    string modelname,
                    string mjdref,
                    string debug,
                    string batchsize,
                    string numberofthread,
                    string mhost,
                    string mpwd,
                    string musr,
                    string mdb,
                    string rhost,
                    string rpwd,
                    string rdb,
                    string indexon
                  ) {


  Config * writeConf = myConf;

  map < string, string > Gentries;

  map < string, string > Mentries;

  map < string, string > Rentries;

  vector < map <string, string > > GSection;

  vector < map <string, string > > MSection;

  vector < map <string, string > > RSection;

  Gentries["modelname"]= modelname;
  Gentries["mjdref"]= mjdref;
  Gentries["debug"]= debug;
  Gentries["batchsize"]= batchsize;
  Gentries["numberofthreads"]= numberofthread;

  GSection.push_back(Gentries);

  Mentries["host"]= mhost;
  Mentries["password"]= mpwd;
  Mentries["username"]= musr;
  Mentries["dbname"]= mdb;

  MSection.push_back(Mentries);

  Rentries["host"]= rhost;
  Rentries["password"]= rpwd;
  Rentries["dbname"]= rdb;
  Rentries["indexon"]= indexon;

  RSection.push_back(Rentries);

  for(int i=0; i <2; i++) {

    switch(i) {

      case 1 : writeConf->setSection("./rtalibconfig", "General", GSection);

      case 2 : writeConf->setSection("./rtalibconfig", "MySql", MSection);

      case 3 : writeConf->setSection("./rtalibconfig", "Redis", RSection);
    }

  }

  writeConf->setConfFile("./rtalibconfig");

}


int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);

  cout << startString<< endl;

  return RUN_ALL_TESTS();


}
