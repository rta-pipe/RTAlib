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
                  char comment, enum ini_utf8_mode utf8_mode) {
  if (!file || is_special_character(equals, '\0', '\0') ||
      is_special_character(comment, '\0', '\0'))
    return 0;

  // open file
  file->file = fopen(path, "w");
  if (!file->file)
    return 0;

  // write byte order mark if needed
  if (utf8_mode == INI_UTF8_MODE_ALLOW_WITH_BOM) {
    const char BOM[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
    if (fwrite(BOM, 1, 3, file->file) != 3)
      return 0;
  }

  // set options
  file->utf8_mode = utf8_mode;
  file->equals = equals;
  file->comment = comment;

  return 1;
}

/**
 * Closes an .INI file. The file pointer must be valid!
 * @param file pointer
 * @return 1 on success; 0 on failure
 */
int ini_close_file(struct ini_file *file) {
  if (!file)
    return 0;

  if (fclose(file->file))
    return 0;

  file->file = NULL;

  return 1;
}

/**
 * Writes a section name to a file. The section and file pointers must be valid!
 * The section name mustn't include whitespace character, other escape
 * characters!
 * THe section name also mustn't be empty!
 * @param file pointer
 * @param section name
 * @return 1 on success; 0 on failure
 */
int ini_write_section(const struct ini_file *file, const char *section) {
  if (!file || !section)
    return 0;

  if (contains_escape_characters(section, file->comment, file->equals) ||
      contains_utf8(section))
    return 0;

  size_t len = strlen(section);

  if (!len)
    return 0;

  if (fwrite("\n[", 1, 2, file->file) != 2)
    return 0;
  if (fwrite(section, 1, len, file->file) != len)
    return 0;
  if (fwrite("]\n\n", 1, 3, file->file) != 3)
    return 0;

  return 1;
}

/**
 * Verifies that a given string consists only of valid UTF-8 sequences or
 * doesn't contain any UTF-8 sequence.
 * @param string
 * @param flag whether to allow UTF-8 or not
 * @return 1 if the string is valid; 0 otherwise
 */
int ini_verify_utf8(const char *str, int allow_utf8) {
  if (!str)
    return 0;

  size_t pos = 0;
  size_t len = strlen(str);
  while (pos < len) {
    if ((str[pos] & 0x7F) == str[pos]) {
      ++pos;
      continue;
    } else if (allow_utf8) {
      size_t utf8_len = get_utf8_len(str[pos]);
      if (!utf8_len || utf8_len + pos > len)
        // invalid sequence
        return 0;

      utf8_len += pos - 1;

      for (++pos; pos <= utf8_len; ++pos) {
        if ((str[pos] ^ 0x80) & 0xC0)
          return 0;
      }
    } else {
      // UTF-8 sequences not allowed
      return 0;
    }
  }

  return 1;
}

/**
 * Writes a name - value pair to a file. The name and file pointers mustn't be
 * NULL!
 * @param file pointer
 * @param name
 * @param value
 * @return 1 on success; 0 on failure
 */
int ini_write_name_value(const struct ini_file *file, const char *name,
                         const char *value) {
  if (!file || !name)
    return 0;

  if (contains_escape_characters(name, file->comment, file->equals) ||
      contains_utf8(name))
    return 0;

  size_t len = strlen(name);

  if (!len)
    return 0;

  // write name
  if (fwrite(name, 1, len, file->file) != len)
    return 0;

  if (value) {
    char equals_str[2] = " \"";
    equals_str[0] = file->equals;

    // if value exists, write equals character and write value
    if (fwrite(equals_str, 1, 2, file->file) != 2)
      return 0;

    if (!write_value(file, value))
      return 0;

    // end quotes
    if (fwrite("\"", 1, 1, file->file) != 1)
      return 0;
  }

  // write newline
  if (fwrite("\n", 1, 1, file->file) != 1)
    return 0;

  return 1;
}

/**
 * Writes a comment into an .INI file. The comment string and file pointers
 * mustn't be NULL!
 * @param file
 * @param comment string
 * @return 1 on success; 0 on failure
 */
int ini_write_comment(const struct ini_file *file, const char *comment) {
  if (!file || !comment)
    return 0;

  size_t len = strlen(comment);
  size_t pos = 0;
  size_t current_length = 0;
  size_t last_pos = 0;

  // write comment character
  if (fwrite(&file->comment, 1, 1, file->file) != 1)
    return 0;

  for (; pos < len; ++pos) {
    ++current_length;
    if (is_newline(comment[pos])) {
      // write comment
      if (fwrite(comment + last_pos, 1, current_length, file->file) !=
          current_length)
        return 0;

      // write comment character
      if (fwrite(&file->comment, 1, 1, file->file) != 1)
        return 0;

      current_length = 0;
      last_pos = pos + 1;
    } else if ((file->utf8_mode == INI_UTF8_MODE_ESCAPE ||
                file->utf8_mode == INI_UTF8_MODE_FORBID) &&
               is_utf8_part(comment[pos])) {
      // abort if an UTF-8 sequence is detected but not allowed
      return 0;
    }
  }

  if (current_length)
    // write comment
    if (fwrite(comment + last_pos, 1, current_length, file->file) !=
        current_length)
      return 0;

  // write newline
  if (fwrite("\n", 1, 1, file->file) != 1)
    return 0;

  return 1;
}
