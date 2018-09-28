/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

//#include "../include/ini_helper.h"
#include "../include_c/ini_helper.h"

/**
 * Converts a ASCII hex digit (0 - f or 0 - F) to a binary value.
 * @param ASCII character
 * @return converted charecter; -1 on failure
 */
char hex_digit_to_binary(char c) {
  // convert ASCII to hex and return -1 on failure
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return -1;
}

/**
 * Converts unicode characters to UTF-8 characters.
 * Note: This uses the UTF-8 standard since 2003.
 * @param unicode character
 * @return UTF-8 character; -1 on failure
 */
struct four_bytes uctoutf8(struct four_bytes code) {
  // NOTE: this uses the UTF-8 standard since 2003
  struct four_bytes c = {{0xFF, 0xFF, 0xFF, 0xFF}};
  if (code.bytes[3] == 0 && code.bytes[2] == 0 && code.bytes[1] == 0 &&
      code.bytes[0] <= 0x7F) {
    c.bytes[0] = (0x7f & code.bytes[0]);
    c.bytes[1] = 0;
    c.bytes[2] = 0;
    c.bytes[3] = 0;
  } else if (code.bytes[3] == 0 && code.bytes[2] == 0 &&
             code.bytes[1] <= 0x07) {
    c.bytes[0] = (0x80 | (0x3F & code.bytes[0]));
    c.bytes[1] =
        (0xC0 | (0x1F & ((code.bytes[0] >> 6) | (code.bytes[1] << 2))));
    c.bytes[2] = 0;
    c.bytes[3] = 0;
  } else if (code.bytes[3] == 0 && code.bytes[2] == 0) {
    c.bytes[0] = (0x80 | (0x3F & code.bytes[0]));
    c.bytes[1] =
        (0x80 | (0x3F & ((code.bytes[0] >> 6) | (code.bytes[1] << 2))));
    c.bytes[2] =
        (0xE0 | (0x0F & ((code.bytes[1] >> 4) | (code.bytes[2] << 4))));
    c.bytes[3] = 0;
  } else if (code.bytes[3] == 0 && code.bytes[2] <= 0x10) {
    c.bytes[0] = (0x80 | (0x3F & code.bytes[0]));
    c.bytes[1] =
        (0x80 | (0x3F & ((code.bytes[0] >> 6) | (code.bytes[1] << 2))));
    c.bytes[2] =
        (0x80 | (0x3F & ((code.bytes[1] >> 4) | (code.bytes[2] << 4))));
    c.bytes[3] =
        (0xF0 | (0x07 & ((code.bytes[2] >> 2) | (code.bytes[3] << 6))));
  }

  return c;
}

/**
 * Checks if a character is a spacing character (tab or space).
 * @param character
 * @return 1, if the character is a spacing character (0 otherwise)
 */
int is_spacing(char c) {
  if (c == '\t' || c == ' ')
    return 1;
  return 0;
}

/**
 * Checks if a character is a newline character (\r or \n).
 * @param character
 * @return 1, if the character is a newline character (0 otherwise)
 */
int is_newline(char c) {
  if (c == '\r' || c == '\n')
    return 1;
  return 0;
}

/**
 * Checks if a character is a whitespace character (spacing or newline).
 * @param character
 * @return 1, if the character is a whitespace character (0 otherwise)
 */
int is_whitespace(char c) {
  if (is_spacing(c) || is_newline(c))
    return 1;
  return 0;
}

/**
 * Checks if a character is a special character.
 * @param character
 * @param comment character
 * @param equals character
 * @return 1, if the character is a special character (0 otherwise)
 */
int is_special_character(char c, char comment, char equals) {
  if (c == '[' || c == ']' || c == '\\' || c == '\'' || c == '"' ||
      c == comment || c == equals)
    return 1;
  return 0;
}

/**
 * Checks if a character is part of an UTF-8 sequence.
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_utf8_part(char c) {
  if (c & 0x80)
    return 1;
  return 0;
}

/**
 * Checks if a character a printable character.
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_printable_char(char c) {
  if (c > 32 || is_utf8_part(c) || is_whitespace(c))
    return 1;
  return 0;
}

/**
 * Returns the number of whitespaces on the left side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t lskipws(const char *str) {
  size_t i = 0;
  // skip whitespaces
  while (is_whitespace(str[i++]))
    ;
  // return index of the last whitespace character at the front
  return --i;
}

/**
 * Returns the number of whitespaces on the right side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t rskipws(const char *str) {
  size_t len = strlen(str);
  if (len == 0)
    return 0;
  int i = (int)len;
  // skip whitespaces from back to front
  while ((is_whitespace(str[i]) || str[i] == '\0') && i >= 0)
    --i;
  // return index of first whitespace character at the end
  return ++i;
}

/**
 * Reads an unicode character from a string and converts it to an UTF-8
 * character.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to an offset
 * @return UTF-8 character; -1 on failure
 */
struct four_bytes read_unicode_from_string(const char *str, size_t *pos) {
  size_t local_pos = 0;
  if (pos)
    local_pos = *pos;

  struct four_bytes ret = {{0, 0, 0, 0}};
  size_t len = strlen(str);
  unsigned int hex_length;

  if (local_pos + 6 > len)
    // string too short
    return (struct four_bytes){{0xFF, 0xFF, 0xFF, 0xFF}};

  for (hex_length = 0; hex_length < 6 && hex_length < len; ++hex_length) {
    char current_char = str[local_pos + hex_length];
    char val = hex_digit_to_binary(current_char);
    if (val == -1)
      // invalid hex character and therfore invalid hex code
      return (struct four_bytes){{0xFF, 0xFF, 0xFF, 0xFF}};

    ret.bytes[(5 - hex_length) / 2] |=
        ((unsigned char)val << (((5 - hex_length) % 2) * 4));
  }

  // add number of read digits
  local_pos += hex_length;

  ret = uctoutf8(ret);
  if (ret.bytes[0] == 0xFF && ret.bytes[1] == 0xFF && ret.bytes[2] == 0xFF &&
      ret.bytes[3] == 0xFF) {
    // invalid code
    return (struct four_bytes){{0xFF, 0xFF, 0xFF, 0xFF}};
  }

  if (pos)
    *pos = local_pos;

  return ret;
}

/**
 * Checks if a string has a whitespace character before a newline.
 * The optional offset points to either the first non-whitespace character or
 * the newline.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to offset
 * @return 1, if the string contains whitespaces before a newline (0 otherwise)
 */
int has_non_whitespace_before_newline(const char *str, size_t *pos) {
  size_t local_pos = 0;
  if (pos)
    local_pos = *pos;

  while (!is_newline(str[local_pos]) && str[local_pos] != '\0') {
    if (!is_whitespace(str[local_pos++])) {
      --local_pos;
      if (pos)
        *pos = local_pos;
      return 1;
    }
  }

  if (pos)
    *pos = local_pos;
  return 0;
}

/**
 * Replaces an escaped character with its counter part.
 * The comment and equals character are intended for user defined escaped
 * characters
 * and return themselves.
 * Possible escaped characters: 'a', 'b', 'f', '[', ']', 'n', 'r', 't', 'v',
 * ''', '"', '\', ' ', '\t', '\n', '\r', the comment character and the equals
 * character
 * Notes:
 *  - a whitespace after an '\' creates a newline
 *  - 'x' returns x (which is used to enter Unicode characters)
 * @param character
 * @param comment character
 * @param equals character
 * @return the unescaped counterpart or -1 on failure
 */
char unescape_char(char c, char comment, char equals) {
  char ret = -1;
  if (c == comment)
    return comment;
  else if (c == equals)
    return equals;
  switch (c) {
  case 'a':
    ret = '\a';
    break;
  case 'b':
    ret = '\b';
    break;
  case 'f':
    ret = '\f';
    break;
  case '[':
    ret = '[';
    break;
  case ']':
    ret = ']';
    break;
  case 'n':
    ret = '\n';
    break;
  case 'r':
    ret = '\r';
    break;
  case 't':
    ret = '\t';
    break;
  case 'v':
    ret = '\v';
    break;
  case '\'':
    ret = '\'';
    break;
  case '"':
    ret = '\"';
    break;
  case '\\':
    ret = '\\';
    break;
  case 'x':
    ret = 'x';
    break;
  case 'X':
    ret = 'x';
    break;
  case ' ':
  case '\t':
  case '\n':
  case '\r':
    ret = '\n';
    break;
  };
  return ret;
}

/**
 * Removes the whitespaces from the front and the back of a string.
 * The input string must not be NULL!
 * @param string
 * @return string without whitespaces at the front or the back.
 */
char *stripws(char *str) {
  size_t offset_front = lskipws(str);
  size_t offset_back = rskipws(str);
  size_t len = offset_back - offset_front;
  if (offset_front >= offset_back)
    len = 0;
  memmove(str, str + offset_front, len);
  str[len] = '\0';
  return str;
}

/**
 * Converts a string with all its escaped sequences to an UTF-8 compatible
 * string.
 * The string must not be NULL and the number of bytes to be copied is expected
 * to be in range!
 * @param string
 * @param number of bytes to copy (has to include the zero-terminator!)
 * @param comment charcter
 * @param equals character
 * @return 1 on success; 0 on failure or if reserved characters are encountered
 */
int unescape_string(char *str, size_t len, char comment, char equals) {
  size_t src_index, dst_index = 0;

  // iterate through all characters
  for (src_index = 0; src_index < len; ++src_index) {
    char c = str[src_index];

    // check for escaped characters
    if (c == '\\') {
      if (++src_index >= len)
        // escaped sequence out of range
        return 0;

      // skip the '\'
      c = str[src_index];

      // convert to UTF-8
      char unesc = unescape_char(c, comment, equals);
      if (unesc == -1) {
        // illegal escaped character
        return 0;
      } else if (unesc == 'x') {
        // convert Unicode
        // skip 'x'
        ++src_index;

        struct four_bytes hex_val = read_unicode_from_string(str, &src_index);

        // check if the src_index is still in range and if the character is
        // valid
        if (src_index > len ||
            (hex_val.bytes[0] == 0xFF && hex_val.bytes[1] == 0xFF &&
             hex_val.bytes[2] == 0xFF && hex_val.bytes[3] == 0xFF))
          return 0;

        // copy only non-null bytes
        for (int k = 3; k >= 0; --k) {
          char current_byte = (0xFF & (hex_val.bytes[k]));
          if (current_byte != 0)
            str[dst_index++] = current_byte;
        }
      } else if (unesc == '\n' && c != 'n') {
#ifndef INI_FORBID_MULTILINE
        if (has_non_whitespace_before_newline(str, &src_index))
          //"\\ "has to be followed by a newline
          return 0;

        // add newline to the string
        str[dst_index++] = '\n';
#else
        // newlines not allowed
        return 0;
#endif // INI_FORBID_MULTILINE
      } else {
        // copy converted character
        str[dst_index++] = unesc;
      }
    } else if (is_special_character(str[src_index], comment, equals)) {
      // invalid character
      return 0;
#ifndef INI_FORBID_MULTILINE
    } else if (is_newline(str[src_index])) {
      // newlines need to be escaped
      return 0;
#endif // INI_FORBID_MULTILINE
    } else {
      // copy normal string
      str[dst_index++] = str[src_index];
    }
  }

  // zero-terminate the string
  str[dst_index] = '\0';

  return 1;
}

/**
 * Returns the size of a file in bytes.
 * The stream must not be NULL!
 * @param file
 * @param optional pointe to error variable
 * @return file size
 */
size_t fsize(FILE *stream, int *error) {
  size_t ret = 0;

  // iterate through file and count the size
  // NOTE: this method was chosen to assure compatibillity
  while (fgetc(stream) != EOF)
    ++ret;
  if (error && ferror(stream) != 0) {
    *error = ferror(stream);
    return 0;
  }

  // reset file
  if (error)
    *error = fseek(stream, 0, SEEK_SET);
  else
    fseek(stream, 0, SEEK_SET);

  return ret;
}

/**
 * Takes three pointers and frees them if they are not NULL.
 * @param first pointer
 * @param second pointer
 * @param third pointer
 */
void free3(void *data0, void *data1, void *data2) {
#ifndef INI_STATIC_MEMORY_SIZE
  if (data0)
    free(data0);
  if (data1)
    free(data1);
  if (data2)
    free(data2);
#else
  (void)data0;
  (void)data1;
  (void)data2;
#endif
}

/**
 * Counts the number of newlines at the given position in a string.
 * '\n' and '\r' are seperately counted as a single newline but the
 * combinations '\n\r' and '\r\n' are also counted as a single newline
 * to assure compatibillity.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to position offset
 * @return number of newlines
 */
unsigned int count_newlines(const char *str, size_t *pos) {
  size_t local_pos = 0;
  if (pos)
    local_pos = *pos;

  unsigned int ret = 0;
  char prev_newline = 0;
  while (is_newline(str[local_pos])) {
    if (prev_newline == str[local_pos]) {
      ++ret;
      prev_newline = str[local_pos];
    } else if (prev_newline == 0) {
      ++ret;
      prev_newline = str[local_pos];
    } else {
      prev_newline = 0;
    }

    ++local_pos;
  }

  if (pos)
    *pos = local_pos;

  return ret;
}

/**
 * Skips the whitespaces at the given position at a string.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to offset
 * @param optional pointer to line number counter
 * @return number of skipped whitespaces
 */
size_t skip_whitespaces(const char *str, size_t *pos,
                        unsigned int *line_number) {
  size_t local_pos = 0;
  if (pos)
    local_pos = *pos;

  // save the starting position
  size_t start_pos = local_pos;

  while (is_whitespace(str[local_pos])) {
    // count spacing characters
    while (is_spacing(str[local_pos]))
      ++local_pos;

    // count newlines
    if (line_number)
      *line_number += count_newlines(str, &local_pos);
    else
      count_newlines(str, &local_pos);
  }

  if (pos)
    *pos = local_pos;

  // return the number of skipped characters
  return local_pos - start_pos;
}

/**
 * Skips all characters until a newline is reached.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to offset
 * @return number of skipped characters
 */
size_t skip_comment(const char *str, size_t *pos) {
  size_t local_pos = 0;
  if (pos)
    local_pos = *pos;

  size_t start_pos = local_pos;

  while (!is_newline(str[local_pos]) && str[local_pos] != '\0')
    ++local_pos;

  if (pos)
    *pos = local_pos;

  return local_pos - start_pos;
}

/**
 * Checks if a string contains UTF-8 characters.
 * The input string musn't be NULL!
 * @param string
 * @return 1 if the string contains UTF-8 characters; 0 otherwise
 */
int contains_utf8(const char *str) {
  size_t len = strlen(str);
  size_t i;

  for (i = 0; i < len; ++i) {
    if (is_utf8_part(str[i]))
      return 1;
  }

  return 0;
}

/**
 * Checks if a string contains characters which is not allowed in a name or a
 * section.
 * The input string musn't be NULL!
 * @param string
 * @param comment character
 * @param equals character
 * @return 1 if the string contains an illegal character; 0 otherwise
 */
int contains_escape_characters(const char *str, char comment, char equals) {
  size_t len = strlen(str);
  size_t i;
  char c;

  for (i = 0; i < len; ++i) {
    c = str[i];
    if (is_special_character(c, comment, equals) || is_whitespace(c))
      return 1;
  }

  return 0;
}

/**
 * Reads either a key name or a section name from an input string.
 * The input string and the output string musn't be NULL and the start and end
 * position
 * are expected to be in range!
 * @param name
 * @param input string
 * @param start position
 * @param end position
 * @param comment character
 * @param equals character
 * @param allow UTF-8 flag
 * @return 1 on success; 0 otherwise
 */
int read_name(char *name, const char *str, size_t start, size_t end,
              char comment, char equals, int allow_utf8) {
  size_t len = end - start;

  // copy new name
  memcpy(name, str + start, len);
  name[len] = '\0';

  // strip whitespaces form the front and the back of the string
  name = stripws(name);

  // check against illegal characters
  if (contains_escape_characters(name, comment, equals))
    return 0;

  // chech if UTF-8 characters are allowed
  if (!allow_utf8) {
    if (contains_utf8(name))
      return 0;
  }

  return 1;
}

/**
 * Removes quotes directly from the front and the back of the string.
 * The input string musn't be NULL!
 * @param string
 * @return 1 on success, 0 if the quotes don't match
 */
int remove_quotes(char *str) {
  size_t len = strlen(str);

  if (len) {
    char first = str[0];
    char last = str[len - 1];
    if (first == '"' || first == '\'') {
      if (len < 2)
        // string too short for quotes
        return 0;

      if (first != last)
        // quotes not closed correctly
        return 0;

      // remove quotes
      len -= 2;
      memcpy(str, str + 1, len);
      str[len] = '\0';
    }
  }

  return 1;
}

/**
 * Reads value from an input string.
 * The input string and the output string musn't be NULL and the start and end
 * position
 * are expected to be in range!
 * @param value
 * @param input string
 * @param start position
 * @param end position
 * @param comment character
 * @param equals character
 * @param optional pointer to line number counter
 * @param allow UTF-8 flag
 * @return 1 on success; 0 otherwise
 */
int read_value(char *value, const char *str, size_t start, size_t end,
               char comment, char equals, unsigned int *line_number,
               int allow_utf8) {
  size_t len = end - start;

  // copy new value
  memcpy(value, str + start, len);
  value[len] = '\0';

  // count newlines
  if (line_number) {
    size_t i;
    for (i = 0; i < len; ++i) {
      while (value[i] && !is_newline(value[i]))
        ++i;
      *line_number += count_newlines(value, &i);
    }
  }

  // strip whitespaces form the front and the back of the string
  value = stripws(value);

  if (!remove_quotes(value))
    return 0;

  // check against illegal characters
  if (!unescape_string(value, strlen(value), comment, equals))
    return 0;

  // chech if UTF-8 characters are allowed
  if (!allow_utf8) {
    if (contains_utf8(value))
      return 0;
  }

  return 1;
}

/**
 * Converts a binary value to two ASCII hex digits.
 * @param binary value
 * @return two ASCII digits
 */
struct two_bytes binary_to_hex_digits(char c) {
  struct two_bytes ret;
  int i;

  ret.bytes[0] = (c & 0x0F);
  ret.bytes[1] = ((c & 0xF0) >> 4);

  for (i = 0; i < 2; ++i) {
    if (ret.bytes[i] <= 9)
      ret.bytes[i] += '0';
    else
      ret.bytes[i] += 'A' - 10;
  }

  return ret;
}

/**
 * Converts UTF-8 characters into an unicode character.
 * //NOTE: this uses the UTF-8 standard since 2003.
 * @param UTF-8 code
 * @return unicode character; -1 on failure
 */
struct four_bytes utf8touc(struct four_bytes code) {
  // NOTE: this uses the UTF-8 standard since 2003
  struct four_bytes c = {{0xFF, 0xFF, 0xFF, 0xFF}};

  if (!(code.bytes[3] || code.bytes[2] || code.bytes[1] ||
        code.bytes[0] & 0x80)) {
    c.bytes[0] = (0x7F & code.bytes[0]);
    c.bytes[1] = 0;
    c.bytes[2] = 0;
    c.bytes[3] = 0;
  } else if (!(code.bytes[3] || code.bytes[2] ||
               (code.bytes[1] ^ 0xDF) & 0xE0 ||
               (code.bytes[0] ^ 0xBF) & 0xC0)) {
    c.bytes[0] = ((0x03 & code.bytes[1]) << 6) | (0x3F & code.bytes[0]);
    c.bytes[1] = ((0x1F & code.bytes[1]) >> 2);
    c.bytes[2] = 0;
    c.bytes[3] = 0;
  } else if (!(code.bytes[3] || (code.bytes[2] ^ 0xEF) & 0xF0 ||
               (code.bytes[1] ^ 0xBF) & 0xC0 ||
               (code.bytes[0] ^ 0xBF) & 0xC0)) {
    c.bytes[0] = ((0x03 & code.bytes[1]) << 6) | (0x3F & code.bytes[0]);
    c.bytes[1] = ((0x0F & code.bytes[2]) << 4) | ((0x3F & code.bytes[1]) >> 2);
    c.bytes[2] = 0;
    c.bytes[3] = 0;
  } else if (!((code.bytes[3] ^ 0xF7) & 0xF8 || (code.bytes[2] ^ 0xBF) & 0xC0 ||
               (code.bytes[1] ^ 0xBF) & 0xC0 ||
               (code.bytes[0] ^ 0xBF) & 0xC0)) {
    c.bytes[0] = ((0x03 & code.bytes[1]) << 6) | (0x3F & code.bytes[0]);
    c.bytes[1] = ((0x0F & code.bytes[2]) << 4) | ((0x3F & code.bytes[1]) >> 2);
    c.bytes[2] = ((0x07 & code.bytes[3]) << 2) | ((0x3F & code.bytes[2]) >> 4);
    c.bytes[3] = 0;
  }

  return c;
}

/**
 * Returns the length of an UTF-8 sequence according to the first byte.
 * @param first byte
 * @return length (0 if the first byte is invalid)
 */
size_t get_utf8_len(char c) {
  if (!(c & 0x80))
    // 2 byte UTF-8 sequence
    return 1;
  else if (!((c ^ 0xDF) & 0xE0))
    // 2 byte UTF-8 sequence
    return 2;
  else if (!((c ^ 0xEF) & 0xF0))
    // 3 byte UTF-8 sequence
    return 3;
  else if (!((c ^ 0xF7) & 0xF8))
    // 3 byte UTF-8 sequence
    return 4;

  // invalid UTF-8 sequence
  return 0;
}

/**
 * Checks if a given file has a byte order marker and skips it.
 * The file stream mustn't be invalid!
 * @param file stream
 * @return 1 if it has a BOM; 0 if this is not the case; -1 on failure
 */
int check_for_BOM(FILE *stream) {
  // look for byte order mark
  char BOM[3] = {0, 0, 0};

  if (fread(BOM, 1, 3, stream) == 3) {
    if (BOM[0] != (char)0xEF || BOM[1] != (char)0xBB || BOM[2] != (char)0xBF)
      rewind(stream);
    else
      return 1;

  } else if (ferror(stream)) {
    return -1;
  } else {
    rewind(stream);
  }

  return 0;
}

/**
 * Writes a value string to a file. Please note that the output file will
 * contain junk data if this method fails.
 * @param file struct (mustn't be NULL)
 * @param value string
 * @return 1 if the string is valid; 0 otherwise
 */
int write_value(const struct ini_file *file, const char *value) {
  if (!file || !value)
    return 0;

  size_t len = strlen(value);
  size_t i;

  for (i = 0; i < len; ++i) {
    char c = value[i];

    if (is_special_character(c, file->comment, file->equals) || is_newline(c) ||
        c == '\a' || c == '\b' || c == '\f' || c == '\v') {
      // write escaped characters
      char escaped_str[2] = "\\ ";
      escaped_str[1] = c;

      if (fwrite(escaped_str, 1, 2, file->file) != 2)
        return 0;

    } else if (is_whitespace(c) || c >= 32) {
#ifdef INI_FORBID_MULTILINE
      if (is_newline(c))
        return 0;
#endif
      // normal character
      if (fwrite(&c, 1, 1, file->file) != 1)
        return 0;

    } else if (file->utf8_mode) {

      if (file->utf8_mode == INI_UTF8_MODE_ESCAPE) {
        // escape UTF-8
        struct four_bytes bin_str = {{0, 0, 0, 0}};
        size_t j;
        size_t bin_len = get_utf8_len(c);

        if (!bin_len || bin_len + i > len)
          // invalid length
          return 0;

        // read UTF-8 character
        for (j = 0; j < bin_len; ++j)
          bin_str.bytes[bin_len - j - 1] = value[i + j];
        i += bin_len - 1;

        // convert character
        struct four_bytes hex = utf8touc(bin_str);
        // abort if conversion failed
        if (hex.bytes[0] == 0xFF && hex.bytes[1] == 0xFF &&
            hex.bytes[2] == 0xFF && hex.bytes[3] == 0xFF)
          return 0;

        char escaped_str[8] = "\\x000000";
        char *current;

        // convert binary to hex digits
        for (j = 0; j < 3; ++j) {
          struct two_bytes hex_digits = binary_to_hex_digits(hex.bytes[j]);
          current = (char *)&hex_digits;

          // store converted characters
          escaped_str[7 - j * 2] = current[0];
          escaped_str[7 - j * 2 - 1] = current[1];
        }

        // write everything into a file
        if (fwrite(escaped_str, 1, 8, file->file) != 8)
          return 0;
      } else {
        // INI_UTF8_MODE_ALLOW & INI_UTF8_MODE_ALLOW_WITH_BOM
        // UTF-8 sequences allowed; no need to escape anything
        size_t bin_len = get_utf8_len(c);

        if (!bin_len || bin_len + i > len)
          // invalid length
          return 0;

        if (fwrite(value + i, 1, bin_len, file->file) != bin_len)
          return 0;

        // skip already read characters
        i += bin_len - 1;
        continue;
      }
    } else {
      // invalid character
      return 0;
    }
  }
  return 1;
}
