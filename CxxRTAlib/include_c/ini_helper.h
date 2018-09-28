/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INI_HELPER_H
#define INI_HELPER_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ini_config.h"

/**
 * Two byte data structure;
 */
struct two_bytes {
  unsigned char bytes[2];
};

/**
 * Four byte data structure;
 */
struct four_bytes {
  unsigned char bytes[4];
};

// helper functions

/**
 * Converts a ASCII hex digit (0 - f or 0 - F) to a binary value.
 * @param ASCII character
 * @return converted charecter; -1 on failure
 */
struct two_bytes binary_to_hex_digits(char c);

/**
 * Converts unicode characters to UTF-8 characters.
 * Note: This uses the UTF-8 standard since 2003.
 * @param unicode character
 * @return UTF-8 character; -1 on failure
 */
struct four_bytes uctoutf8(struct four_bytes code);

/**
 * Checks if a character is a spacing character (tab or space).
 * @param character
 * @return 1, if the character is a spacing character (0 otherwise)
 */
int is_spacing(char c);

/**
 * Checks if a character is a newline character (\r or \n).
 * @param character
 * @return 1, if the character is a newline character (0 otherwise)
 */
int is_newline(char c);

/**
 * Checks if a character is a whitespace character (spacing or newline).
 * @param character
 * @return 1, if the character is a whitespace character (0 otherwise)
 */
int is_whitespace(char c);

/**
 * Checks if a character is a special character.
 * @param character
 * @param comment character
 * @param equals character
 * @return 1, if the character is a special character (0 otherwise)
 */
int is_special_character(char c, char comment, char equals);

/**
 * Checks if a character is part of an UTF-8 sequence.
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_utf8_part(char c);

/**
 * Checks if a character a printable character.
 * @param character
 * @return 1, if this is the case (0 otherwise)
 */
int is_printable_char(char c);

/**
 * Returns the number of whitespaces on the left side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t lskipws(const char *str);

/**
 * Returns the number of whitespaces on the right side of a string.
 * The input string must not be NULL!
 * @param string
 * @return number of whitespaces
 */
size_t rskipws(const char *str);

/**
 * Reads an unicode character from a string and converts it to an UTF-8
 * character.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to an offset
 * @return UTF-8 character; -1 on failure
 */
struct four_bytes read_unicode_from_string(const char *str, size_t *pos);

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
int has_non_whitespace_before_newline(const char *str, size_t *pos);

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
char unescape_char(char c, char comment, char equals);

/**
 * Removes the whitespaces from the front and the back of a string.
 * The input string must not be NULL!
 * @param string
 * @return string without whitespaces at the front or the back.
 */
char *stripws(char *str);

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
int unescape_string(char *str, size_t len, char comment, char equals);

/**
 * Returns the size of a file in bytes.
 * The stream must not be NULL!
 * @param file
 * @param optional pointe to error variable
 * @return file size
 */
size_t fsize(FILE *stream, int *error);

/**
 * Takes three pointers and frees them if they are not NULL.
 * @param first pointer
 * @param second pointer
 * @param third pointer
 */
void free3(void *data0, void *data1, void *data2);

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
unsigned int count_newlines(const char *str, size_t *pos);

/**
 * Skips the whitespaces at the given position at a string.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to offset
 * @param optional pointer to line number counter
 * @return number of skipped whitespaces
 */
size_t skip_whitespaces(const char *data, size_t *pos,
                        unsigned int *line_number);

/**
 * Skips all characters until a newline is reached.
 * The input string must not be NULL and the position offset is expected to be
 * in range!
 * @param string
 * @param optional pointer to offset
 * @return number of skipped characters
 */
size_t skip_comment(const char *data, size_t *pos);

/**
 * Checks if a string contains UTF-8 characters.
 * The input string musn't be NULL!
 * @param string
 * @return 1 if the string contains UTF-8 characters; 0 otherwise
 */
int contains_utf8(const char *str);

/**
 * Checks if a string contains characters which is not allowed in a name or a
 * section.
 * The input string musn't be NULL!
 * @param string
 * @param comment character
 * @param equals character
 * @return 1 if the string contains an illegal character; 0 otherwise
 */
int contains_escape_characters(const char *str, char comment, char equals);

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
              char comment, char equals, int allow_utf8);

/**
 * Removes quotes directly from the front and the back of the string.
 * The input string musn't be NULL!
 * @param string
 * @return 1 on success, 0 if the quotes don't match
 */
int remove_quotes(char *str);

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
               int allow_utf8);

/**
 * Converts a binary value to two ASCII hex digits.
 * @param binary value
 * @return two ASCII digits
 */
char hex_digit_to_binary(char c);

/**
 * Converts UTF-8 characters into an unicode character.
 * //NOTE: this uses the UTF-8 standard since 2003.
 * @param UTF-8 code
 * @return unicode character; -1 on failure
 */
struct four_bytes utf8touc(struct four_bytes code);

/**
 * Returns the length of an UTF-8 sequence according to the first byte.
 * @param first byte
 * @return length (0 if the first byte is invalid)
 */
size_t get_utf8_len(char c);

/**
 * Checks if a given file has a byte order marker and skips it.
 * The file stream mustn't be invalid!
 * @param file stream
 * @return 1 if it has a BOM; 0 if this is not the case; -1 on failure
 */
int check_for_BOM(FILE *stream);

/**
 * Writes a value string to a file. Please note that the output file will
 * contain junk data if this method fails.
 * @param file struct (mustn't be NULL)
 * @param value string
 * @return 1 if the string is valid; 0 otherwise
 */
int write_value(const struct ini_file *file, const char *value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_HELPER_H
