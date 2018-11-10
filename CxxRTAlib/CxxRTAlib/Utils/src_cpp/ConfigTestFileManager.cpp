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

#include "ConfigTestFileManager.hpp"
IniFile ini;

ConfigTestFileManager::ConfigTestFileManager(){

}

void ConfigTestFileManager::writeConfigFile(map < string, vector < map < string, string > > > input){

  Config * myConf;

  myConf = Config::getIstance("../../Configs/rtalibconfig_testing");

  map < string, string > Gentries;

  map < string, string > DTRentries;

  map < string, string > Mentries;

  map < string, string > Rentries;

  map < string, string > MPDentries;

  vector < map <string, string > > GSection;

  vector < map <string, string > > DTRSection;

  vector < map <string, string > > MSection;

  vector < map <string, string > > RSection;

  vector < map <string, string > > MPDSection;

  vector < map < string, vector < map < string, string > > > > inifileModel;

  map < string, vector < map <string, string > > > Section;

  Gentries["modelname"]= myConf->file["General"]["modelname"].getString();
  Gentries["mjdref"]= myConf->file["General"]["mjdref"].getString();
  Gentries["debug"]= myConf->file["General"]["debug"].getString();
  Gentries["batchsize"]= myConf->file["General"]["batchsize"].getString();
  Gentries["numberofthreads"]= myConf->file["General"]["numberofthreads"].getString();
  GSection.push_back(Gentries);

  DTRentries["active"] = myConf->file["Dtr"]["active"].getString();
  DTRentries["debug"] = myConf->file["Dtr"]["debug"].getString();
  DTRentries["inputchannel"] = myConf->file["Dtr"]["inputchannel"].getString();
  DTRentries["outputchannel"] = myConf->file["Dtr"]["outputchannel"].getString();
  DTRSection.push_back(DTRentries);

  Mentries["host"]= myConf->file["MySql"]["host"].getString();
  Mentries["password"]= myConf->file["MySql"]["password"].getString();
  Mentries["username"]= myConf->file["MySql"]["username"].getString();
  Mentries["dbname"]= myConf->file["MySql"]["dbname"].getString();
  MSection.push_back(Mentries);

  Rentries["host"]= myConf->file["Redis"]["host"].getString();
  Rentries["password"]= myConf->file["Redis"]["password"].getString();
  Rentries["dbname"]= myConf->file["Redis"]["dbname"].getString();
  Rentries["indexon"]= myConf->file["Redis"]["indexon"].getString();
  RSection.push_back(Rentries);

  MPDentries["active"] = myConf->file["MySqlPipelineDatabase"]["active"].getString();
  MPDentries["debug"] = myConf->file["MySqlPipelineDatabase"]["debug"].getString();
  MPDentries["host"] = myConf->file["MySqlPipelineDatabase"]["host"].getString();
  MPDentries["username"] = myConf->file["MySqlPipelineDatabase"]["username"].getString();
  MPDentries["password"] = myConf->file["MySqlPipelineDatabase"]["password"].getString();
  MPDentries["dbname"] = myConf->file["MySqlPipelineDatabase"]["dbname"].getString();
  MPDSection.push_back(MPDentries);

  Section["General"] = GSection;
  Section["Dtr"] = DTRSection;
  Section["MySql"] = MSection;
  Section["Redis"] = RSection;
  Section["MySqlPipelineDatabase"] = MPDSection;

  inifileModel.push_back(Section);
  bool check = false;
  // cout << "CHECK INI: " << check << endl;

  if(input.size()!=0) {
  // cout << "INPUT DIVERSO DA ZERO!" << endl;

  for(vector < map < string, vector < map < string, string > > > >::iterator vet_it=inifileModel.begin(); vet_it!=inifileModel.end(); ++vet_it ) {

    for(map < string, vector < map < string, string > > >::iterator map_it=Section.begin(); map_it!=Section.end(); ++map_it ) {

      // cout << "[" << map_it->first << "]" << endl;
      string sectionName = map_it->first;
      IniSection sect(sectionName);

      // if(input.size()!=0) {

        map < string, vector < map < string, string > > >::iterator gg = input.begin();
        string modifySection = gg->first;
        if(sectionName.compare(modifySection)==0){
          // cout << modifySection << " = " << sectionName << endl;
          // cout << "CHECK COMAPRE: " << check << endl;
          check=true;
        }

      vector < map < string, string > > gl =gg->second;

      vector < map <string, string> > vect = map_it->second;

      vector < map <string, string> >::iterator it, en;
      for( it = vect.begin(), en=gl.begin(); it!= vect.end(); ++it, ++en ) {

          map < string, string> currentEntry = *it;

          map < string, string >::iterator map_it;

          if(check){

            map < string, string> modifyEntry = *en;

            map < string, string >::iterator input_it;

            for(map_it=currentEntry.begin(), input_it=modifyEntry.begin(); map_it!=currentEntry.end(); ++map_it, ++input_it ) {

              // cout  << input_it->first <<  " = " << input_it->second << endl;
              string key = input_it->first;
              string value = input_it->second;
              IniEntry entry(key, value);
              sect.insert(entry);

            }
            check = false;
            // cout << "Check value: " << check << endl;

            }else{
            for(map_it=currentEntry.begin(); map_it!=currentEntry.end(); ++map_it) {

              // cout  << map_it->first <<  " = " << map_it->second << endl;
              string key = map_it->first;
              string value = map_it->second;
              IniEntry entry(key, value);
              sect.insert(entry);

            }
          }

        }

      ini.insert(sect);

      check = false;

    }

    IniParser::store(ini, "./rtalibconfig", INI_UTF8_MODE_ALLOW, '=', ';');
    ini.clear();
  }

  // myConf->deleteInstance();

  }else{

    // cout << "INPUT NULLO" << endl;

    for(vector < map < string, vector < map < string, string > > > >::iterator vet_it=inifileModel.begin(); vet_it!=inifileModel.end(); ++vet_it ) {

      for(map < string, vector < map < string, string > > >::iterator map_it=Section.begin(); map_it!=Section.end(); ++map_it ) {

        // cout << "[" << map_it->first << "]" << endl;
        string sectionName = map_it->first;
        IniSection sect(sectionName);



          vector < map <string, string> > vect = map_it->second;

          vector < map <string, string> >::iterator it;
          for( it = vect.begin(); it!= vect.end(); ++it ) {

              map < string, string> currentEntry = *it;

              map < string, string >::iterator map_it;

                for(map_it=currentEntry.begin(); map_it!=currentEntry.end(); ++map_it) {

                  // cout  << map_it->first <<  " = " << map_it->second << endl;
                  string key = map_it->first;
                  string value = map_it->second;
                  IniEntry entry(key, value);
                  sect.insert(entry);

                }
              }

              ini.insert(sect);

              check = false;

            }

            IniParser::store(ini, "./rtalibconfig", INI_UTF8_MODE_ALLOW, '=', ';');
            ini.clear();


      }
      // myConf->deleteInstance();


    }
}

// void ConfigTestFileManager :: clearConfFile(string filepath) {
//
//   ini.clear();
//
// }
