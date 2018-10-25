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
#include "gtest/gtest.h"

#include "Config.hpp"
#include "DBConnector.hpp"
#include "MySqlDBConnector.hpp"
#include "RedisDBConnector.hpp"
#include "RTA_DLTEST_DB.hpp"

using std::cout;
using std::endl;
using std::vector;

const char* startString = {
"################################################################\n"
"###               - RTAlib/CXXRTAlib/UnitTest -              ###"
};

const char* endString = {
"### test RTAlib/CXXRTAlib exiting .......................... ###\n"
"################################################################"
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

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_TRUE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}


TEST(MySqlDBConnector, FailedConnectionWrongPwd) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost","asdasd","rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, FailedConnectionWrongUserName) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", m_password,"fkajhuis","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, FailedConnectionWrongDatabase) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","qwerty","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, SelectRowsTestTable) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  mySqlDBTest->connect();

  EXPECT_TRUE( mySqlDBTest->executeQuery("SELECT * FROM test_table") );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, WriteWrongTable) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  mySqlDBTest->connect();

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_FALSE( mySqlDBTest->insertData("lest_fable", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, InsertDataSuccefully) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"test_table","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  mySqlDBTest->connect();

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( mySqlDBTest->insertData("test_table", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}


/*
 ==================================
*                                 *
*         REDIS TEST CASE         *
*                                 *
 ==================================
*/


TEST(RedisDBConnector, CorrectConnection) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["Redis"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  DBConnector * redisDBTest = new RedisDBConnector(".");

  EXPECT_TRUE(redisDBTest->connect());

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(RedisDBConnector, FailedConnectionWrongPwd) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost","asdasd","rta_test","evt_test","localhost","asdasd","1","evt3:timerealtt");

  DBConnector * redisDBTest = new RedisDBConnector(".");

  EXPECT_FALSE( redisDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(RedisDBConnector, InsertDataSuccIndexonFirst) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:a");

  DBConnector * redisDBTest = new RedisDBConnector(".");

  redisDBTest->connect();

  map <string, string > args;
  args["a"] = "1";
  args["b"] = "2";
  args["c"] = "3";
  args["d"] = "4";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}


TEST(RedisDBConnector, InsertDataSuccIndexonMiddle) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:c");

  DBConnector * redisDBTest = new RedisDBConnector(".");

  redisDBTest->connect();

  map <string, string > args;
  args["a"] = "5";
  args["b"] = "6";
  args["c"] = "7";
  args["d"] = "8";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(RedisDBConnector, InsertDataSuccIndexonLast) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"testmodel","51544.5","yes","1","1","localhost", m_password,"rta_test","evt_test","localhost",r_password,"1","testmodel:d");

  DBConnector * redisDBTest = new RedisDBConnector(".");

  redisDBTest->connect();

  map <string, string > args;
  args["a"] = "9";
  args["b"] = "10";
  args["c"] = "11";
  args["d"] = "12";

  EXPECT_TRUE( redisDBTest->insertData("testmodel", args) );

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

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB("mysql", ".");

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(RTA_DL_DB, waitAndCloseRedisTest) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string m_password = myConf->file["MySql"]["password"].getString();

  string r_password = myConf->file["Redis"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",m_password,"rta_test","evt_test","localhost",r_password,"1","evt3:timerealtt");

  RTA_DLTEST_DB * rtaTestDb = new RTA_DLTEST_DB("redis-basic", ".");

  EXPECT_TRUE( rtaTestDb->waitAndClose() );

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
  Gentries["numberofthread"]= numberofthread;

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

  cout << endString<< endl;

}
