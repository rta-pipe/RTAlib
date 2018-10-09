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

void writeConfFile(Config * myConf, string modelname, string mjdref, string debug, string batchsize, string numberofthread,string mhost, string mpwd, string musr, string mdb );

TEST(MySqlDBConnector, CorrectConnection) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",password,"rta_test","evt_test");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_TRUE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, FailedConnectionWrongPwd) {

  Config * myConf;


  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost","asdasd","rta_test","evt_test");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, FailedConnectionWrongUserName) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", password,"fkajhuis","evt_test");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, FailedConnectionWrongDatabase) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost",password,"rta_test","qwerty");

  DBConnector * mySqlDBTest = new MySqlDBConnector(".");

  EXPECT_FALSE( mySqlDBTest->connect() );

  myConf->deleteInstance();

  myConf->clearConfFile("./rtalibconfig");

}

TEST(MySqlDBConnector, WriteWrongTable) {

  Config * myConf;

  myConf = Config::getIstance("../../PyRTAlib");

  string password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"evt3_memory","51544.5","yes","1","1","localhost", password,"rta_test","evt_test");

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

  string password = myConf->file["MySql"]["password"].getString();

  myConf->deleteInstance();

  writeConfFile(myConf,"test_table","51544.5","yes","1","1","localhost", password,"rta_test","evt_test");

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


void writeConfFile(Config * myConf, string modelname, string mjdref, string debug, string batchsize, string numberofthread,string mhost, string mpwd, string musr, string mdb ){

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

  for(int i=0; i <2; i++) {

    switch(i) {

      case 1 : writeConf->setSection("./rtalibconfig", "General", GSection);

      case 2 : writeConf->setSection("./rtalibconfig", "MySql", MSection);
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
