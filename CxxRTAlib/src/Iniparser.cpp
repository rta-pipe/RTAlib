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

#include "Iniparser.h"

Iniparser::Iniparser(){

}

/*string Iniparser::readFromFile(string filename){

  vector<string> STRING;
  ifstream in( filename.c_str() );

  while(!in.eof()) // To get you all the lines.
  {
    getline(in,STRING); // Saves the line in STRING.
    return STRING;
    //cout << STRING << endl; // Prints our STRING.
  }
  in.close();

}*/

void Iniparser::get(string filename, string section, string key, string value, string& valueReadParse ) { //string& sectioReadParse, string& keyReadParse,

  int secStrPos;
  int secEndPos;
  int keyPos;
  int delimiterPos;

  string startSectionSimbol = "[";
  string endSectionSimbol = "]";
  string delimiter = "=";

  string STRING;
  string sectionParse;
  string keyReadParse;
  //string valueReadParse;

  ifstream in( filename.c_str() );

  cout << "File aperto!" << endl;

    while(!in.eof()) // To get you all the lines.
    {
      getline(in,STRING); // Saves the line in STRING.

      //cout << STRING << endl;

      secStrPos = STRING.find(startSectionSimbol,0);

      if(secStrPos != -1) {
        secEndPos = STRING.find(endSectionSimbol,0);
        sectionParse = STRING.substr(secStrPos+1,secEndPos-1);

        if( sectionParse.compare(section) )

          cout << "Section parse: " << sectionParse << endl;

      }

      delimiterPos = STRING.find(delimiter,0);
      if(delimiterPos != -1){
        keyReadParse = STRING.substr(0, delimiterPos-1);

        if(keyReadParse.compare(key))

          valueReadParse = STRING.substr(delimiterPos+1);
          cout << "Key parse: " << keyReadParse << endl;
          cout << "Value read parse: " << valueReadParse << endl;

      }

    }


  in.close();

}
