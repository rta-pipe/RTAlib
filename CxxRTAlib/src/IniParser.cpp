/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#include "../include/IniParser.hpp"

std::string IniParser::path;
int IniParser::error;
IniParser::ParserException IniParser::parser_exception;

/**
 * Loads an INI file at a given path. If this path is empty, an empty class is
 * returned. If this function fails, the error code can be accessed using the
 * getError() method.
 * @param path
 * @param UTF8 mode
 * @param equals character
 * @param comment character
 * @param comment (stored at the beginning of the file when store() is called)
 * @throws a parser_exception, an std::bad_alloc exception as well as others
 * (see std::string constructor)
 * @return the loaded ini file
 */
IniFile &IniParser::load(std::string path, enum ini_utf8_mode utf8_mode,
                         char equals, char comment) {

  IniFile *ini = new IniFile();

  if (path == "")
    return *ini;

  IniParser::path = path;

  error = ini_read_file(path.c_str(), IniParser::callback, ini, comment, equals,
                        utf8_mode);
  if (error < 0)
    error = READ_ERROR;

  if (error != 0)
    throw parser_exception;

  return *ini;
}

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
void IniParser::store(IniFile ini, std::string path,
                      enum ini_utf8_mode utf8_mode, char equals, char comment) {
  if (path == "")
    return;

  IniParser::path = path;

  struct ini_file file;

  if (!ini_open_file(&file, path.c_str(), equals, comment, utf8_mode)) {
    error = WRITE_ERROR;
    throw parser_exception;
  }

  if (ini.getComment() != "") {
    if (!ini_write_comment(&file, ini.getComment().c_str())) {
      error = WRITE_ERROR;
      throw parser_exception;
    }
  }

  for (IniSection section : ini.getAllSections()) {
    if (section.getComment() != "") {
      if (!ini_write_comment(&file, section.getComment().c_str())) {
        error = WRITE_ERROR;
        throw parser_exception;
      }
    }
    if (section.getName() != "") {
      if (!ini_write_section(&file, section.getName().c_str())) {
        error = WRITE_ERROR;
        throw parser_exception;
      }
    }

    for (IniEntry entry : section.getAllEntries()) {
      if (entry.getComment() != "") {
        if (!ini_write_comment(&file, entry.getComment().c_str())) {
          error = WRITE_ERROR;
          throw parser_exception;
        }
      }
      if (!ini_write_name_value(&file, entry.getKey().c_str(),
                                entry.getString().c_str())) {
        error = WRITE_ERROR;
        throw parser_exception;
      }
    }
  }

  if (!ini_close_file(&file)) {
    error = WRITE_ERROR;
    throw parser_exception;
  }
}

/**
 * Getter for the error. This error can be converted to a string using the
 * getErrorString() method.
 * @return error
 */
int IniParser::getError() { return error; }

/**
 * Returns an error string for the given error code.
 * @param error code
 * @return error string
 * @throws might throw exceptions (see std::string construtor)
 */
std::string IniParser::getErrorString(int error) {
  if (error == WRITE_ERROR)
    return std::string("Error while writing file: ") + path + std::string("!");
  else if (error == READ_ERROR)
    return std::string("Error while reading file: ") + path + std::string("!");
  else if (error == NO_ERROR)
    return std::string("No errors detected.");
  else if (error == INVALID_STRING_ERROR)
    return std::string("Invalid string found!");
  else
    return std::string("Error in file ") + path + std::string(" on line ") +
           std::to_string(error) + std::string("!");
}

/**
 * Callback function for the C interface.
 * @param section name
 * @param key
 * @param value
 * @param data structure
 */
int IniParser::callback(const char *section, const char *name,
                        const char *value, void *data_structure) {
  if (!data_structure)
    return 0;
  IniFile *file = (IniFile *)data_structure;

  std::string section_str = std::string((section ? section : ""));
  std::string name_str = std::string((name ? name : ""));
  std::string value_str = std::string((value ? value : ""));

  file->at(section).at(name) = value;

  return 1;
}
