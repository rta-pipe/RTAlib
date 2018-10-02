/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INI_PARSER_H
#define INI_PARSER_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ini_config.h"

typedef int (*ini_event)(const char *section, const char *name,
                         const char *value, void *data_structure);

// read methods

/**
 * Parses a string with a given file size and calls a handler function when a
 * key value pair is read.
 * The handler should return 0 on failure and a non 0 value on success.
 * @param string
 * @param file size
 * @param handler function
 * @param pointer to user data structure
 * @param comment char
 * @param equals char
 * @param allow UTF-8 flag
 * @return 0 on success; -1 on memory error and the currently parsed line number
 * on a parsing error
 */
int ini_read(const char *str, size_t file_size, ini_event handler,
             void *data_structure, char comment, char equals, int allow_utf8);

/**
 * Reads an .ini file and parses it using the user defined handler function and
 * the data structure.
 * The handler should return 0 on failure and a non 0 value on success.
 * @param string
 * @param file size
 * @param handler function
 * @param pointer to user data structure
 * @param comment char
 * @param equals char
 * @param allow UTF-8 flag
 * @return 0 on success; -1 on memory error and the currently parsed line number
 * on a parsing error
 */
int ini_read_file(const char *path, ini_event handler, void *data_structure,
                  char comment, char equals, enum ini_utf8_mode allow_utf8);

// write methods

/**
 * Opens an .INI file for write and writes the byte order mark if needed.
 * The file pointer mustn't be NULL!
 * @param file pointer
 * @param path
 * @param equals character
 * @param comment character
 * @param UTF-8 mode (INI_UTF8_MODE_FORBID, INI_UTF8_MODE_ALLOW,
 * INI_UTF8_MODE_ALLOW_WITH_BOM, INI_UTF8_MODE_ESCAPE)
 * @return 1 on success; 0 on failure
 */
int ini_open_file(struct ini_file *file, const char *path, char equals,
                  char comment, enum ini_utf8_mode utf8_mode);

/**
 * Closes an .INI file. The file pointer must be valid!
 * @param file pointer
 * @return 1 on success; 0 on failure
 */
int ini_close_file(struct ini_file *file);

/**
 * Writes a section name to a file. The section and file pointers must be valid!
 * The section name mustn't include whitespace character, other escape
 * characters!
 * The section name also mustn't be empty!
 * @param file pointer
 * @param section name
 * @return 1 on success; 0 on failure
 */
int ini_write_section(const struct ini_file *file, const char *section);

/**
 * Verifies that a given string consists only of valid UTF-8 sequences or
 * doesn't contain any UTF-8 sequence.
 * @param string
 * @param flag whether to allow UTF-8 or not
 * @return 1 if the string is valid; 0 otherwise
 */
int ini_verify_utf8(const char *str, int allow_utf8);

/**
 * Writes a name - value pair to a file. The name and file pointers mustn't be
 * NULL!
 * @param file pointer
 * @param name
 * @param value
 * @return 1 on success; 0 on failure
 */
int ini_write_name_value(const struct ini_file *file, const char *name,
                         const char *value);

/**
 * Writes a comment into an .INI file. The comment string and file pointers
 * mustn't be NULL!
 * @param file
 * @param comment string
 * @return 1 on success; 0 on failure
 */
int ini_write_comment(const struct ini_file *file, const char *comment);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_PARSER_H
