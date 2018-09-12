/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "IniFile.hpp"
#include "ini_parser.h"

/*
 * Parser class.
 */
class IniParser {
public:
  /**
   * Loads an INI file at a given path. If this path is empty, an empty class is
   * returned. If this function fails, the error code can be accessed using the
   * getError() method.
   * @param path
   * @param UTF8 mode
   * @param equals character
   * @param comment character
   * @param comment (stored at the beginning of the file when store() is called)
   * @throws a parser_exception and might throw others as well (see std::string
   * constructor)
   * @return the loaded ini file
   */
  static IniFile &load(std::string path,
                       enum ini_utf8_mode utf8_mode = INI_UTF8_MODE_FORBID,
                       char equals = '=', char comment = ';');

  /**
   * Stores an INI file at a given path. If this path is empty, nothing is done.
   * If this function fails, the error code can be accessed using the getError()
   * method.
   * @param path
   * @param UTF8 mode
   * @param equals character
   * @param comment character
   * @param comment (stored at the beginning of the file when store() is called)
   * @throws a parser_exception and might throw others as well (see std::string
   * constructor)
   */
  static void store(IniFile ini, std::string path,
                    enum ini_utf8_mode utf8_mode = INI_UTF8_MODE_FORBID,
                    char equals = '=', char comment = ';');

  /**
   * Getter for the error. This error can be converted to a string using the
   * getErrorString() method.
   * @return error
   */
  static int getError();

  /**
   * Returns an error string for the given error code.
   * @param error code
   * @return error string
   * @throws might throw exceptions (see std::string construtor)
   */
  static std::string getErrorString(int error);

  /**
   * Constants representing the error state.
   */
  const static int NO_ERROR = 0;
  const static int READ_ERROR = -1;
  const static int WRITE_ERROR = -2;
  const static int INVALID_STRING_ERROR = -3;

  /**
   * Class for parser exceptions.
   */
  class ParserException : public std::exception {
    /**
     * Returns exception string.
     * @return exception string
     */
    virtual const char *what() const throw() { return "Parser exception"; }
  };

private:
  static int error;
  static std::string path;
  static ParserException parser_exception;

  /**
   * Callback function for the C interface.
   * @param section name
   * @param key
   * @param value
   * @param data structure
   */
  static int callback(const char *section, const char *name, const char *value,
                      void *data_structure);
};

#endif // INIPARSER_HPP
