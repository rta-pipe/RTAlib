/*
 * Copyright (c) 2017
 *     Leonardo Baroncelli, Giancarlo Zollino,
 *
 * Any information contained in this software
 * is property of the AGILE TEAM and is strictly
 * private and confidential.
*/


#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <sstream>

using std::string;
using std::ofstream;

class FileWriter
{
    public:
      static void write2File(string outputFileName,string input);
      static void write2JSONFile(string outputFileName,string input);
	    static void write2FileAppend(string outputFileName,string input);
      static void write2JSONFileAppend(string outputFileName,string input);
      static string convertToString(int number);
      static string convertToString(float number);
      static string convertToString(double number);
      static string convertToString(time_t number);

    private:
      FileWriter();
};

#endif // FILEWRITER_H
