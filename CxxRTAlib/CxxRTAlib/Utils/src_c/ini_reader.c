/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */
 
#include "../include_c/ini_helper.h"
#include "../include_c/ini_parser.h"

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
             void *data_structure, char comment, char equals, int allow_utf8) {

  size_t pos = 0;
  unsigned int line_number = 1;

#ifndef INI_STATIC_MEMORY_SIZE
  char *section = (char *)calloc(file_size, sizeof(char));
  char *name = (char *)calloc(file_size, sizeof(char));
  char *value = (char *)calloc(file_size, sizeof(char));

  if (!str || !file_size || !section || !name || !value) {
    free3(section, name, value);
    return -1;
  }
#else
  char section[INI_STATIC_MEMORY_SIZE];
  char name[INI_STATIC_MEMORY_SIZE];
  char value[INI_STATIC_MEMORY_SIZE];

  if (!str || !file_size)
    return -1;
#endif

  if (is_special_character(comment, '\0', '\0') ||
      is_special_character(equals, '\0', '\0')) {
    // invalid equals char or invalid comment char
    free3(section, name, value);
    return -1;
  }

  while (pos < file_size) {
    skip_whitespaces(str, &pos, &line_number);

    if (str[pos] == '[') {
      // skip '['
      ++pos;
      unsigned int start = pos;

      // search for the first special character
      while (!is_special_character(str[pos], comment, equals) &&
             !is_newline(str[pos]) && str[pos])
        ++pos;

      if (!read_name(section, str, start, pos, comment, equals, allow_utf8)) {
        free3(section, name, value);
        return line_number;
      }

      if (str[pos++] != ']') {
        // section never ended
        free3(section, name, value);
        return line_number;
      } else if (has_non_whitespace_before_newline(str, &pos)) {
        if (str[pos] != comment) {
          // section is not the only thing on this line (besides whitespaces and
          // comments)
          free3(section, name, value);
          return line_number;
        } else {
          // skip comment
          skip_comment(str, &pos);
        }
      }
    } else if (str[pos] == comment) {
      // skip the comment char
      ++pos;
      skip_comment(str, &pos);

    } else if (is_special_character(str[pos], comment, equals)) {
      // illegal character
      free3(section, name, value);
      return line_number;

    } else if (str[pos] == '\0') {
      // reached end of string
      break;

    } else {
      unsigned int start = pos;

      // search for the first special character
      while (!is_special_character(str[pos], comment, equals) &&
             !is_newline(str[pos]) && str[pos])
        ++pos;

      if (!read_name(name, str, start, pos, comment, equals, allow_utf8)) {
        free3(section, name, value);
        return line_number;
      }

      if (str[pos] == equals) {
        // read value
        start = ++pos;

        // search for newline
        int escaped = 0;
        while ((!is_newline(str[pos]) || escaped) && str[pos]) {
          if (str[pos] == '\\' && !escaped)
            escaped = 1;
          else if (!is_whitespace(str[pos]))
            escaped = 0;

          // check for comment
          if (str[pos] == comment && str[pos - 1] != '\\')
            break;

          ++pos;
        }

        // read value
        if (!read_value(value, str, start, pos, comment, equals, &line_number,
                        allow_utf8)) {
          free3(section, name, value);
          return line_number;
        }

        // skip comments
        if (str[pos] == comment)
          skip_comment(str, &pos);

      } else if (str[pos] == comment) {
        // skip comment
        skip_comment(str, &pos);
        value[0] = '\0';
      } else if (is_newline(str[pos]) || !str[pos]) {
        // handle empty value
        value[0] = '\0';
      } else {
        // invalid character in name
        free3(section, name, value);
        return line_number;
      }

      // call user
      if (!handler(section, name, value, data_structure)) {
        free3(section, name, value);
        return line_number;
      }
    }
  }

  // done parsing
  free3(section, name, value);
  return 0;
}

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
                  char comment, char equals, enum ini_utf8_mode allow_utf8) {
  int ret = 0;
  int error = 0;
  FILE *file = fopen(path, "rb");

  if (!file)
    return -1;

  // get file size
  size_t file_size = fsize(file, &error);
  if (error || !file_size) {
    fclose(file);
    return -1;
  }

#ifdef INI_STATIC_MEMORY_SIZE
  if (file_size >= INI_STATIC_MEMORY_SIZE)
    return -1;
#endif

  // check for byte order mark
  int parse_utf8 = check_for_BOM(file);
  if (parse_utf8 == -1) {
    fclose(file);
    return -1;
  } else if (parse_utf8) {
    file_size -= 3;
  }

  // override BOM if UTF-8 is completely allowed
  if (allow_utf8 == INI_UTF8_MODE_ALLOW)
    parse_utf8 = 1;
  else if (!allow_utf8)
    parse_utf8 = 0;

#ifndef INI_STATIC_MEMORY_SIZE
  // allocate memory
  char *file_content = (char *)malloc(file_size + 1);
  if (!file_content) {
    fclose(file);
    return -1;
  }

#else
  char file_content[INI_STATIC_MEMORY_SIZE];
#endif

  // read content
  size_t read_size = fread(file_content, 1, file_size, file);
  if (read_size != file_size) {
#ifndef INI_STATIC_MEMORY_SIZE
    free(file_content);
#endif
    return -1;
  }

  file_content[file_size] = '\0';

  // close file
  if (fclose(file) == EOF) {
#ifndef INI_STATIC_MEMORY_SIZE
    free(file_content);
#endif
    return -1;
  }

  ret = ini_read(file_content, file_size + 1, handler, data_structure, comment,
                 equals, parse_utf8);

#ifndef INI_STATIC_MEMORY_SIZE
  free(file_content);
#endif

  return ret;
}
