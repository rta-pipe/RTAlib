/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INI_CONFIG_H
#define INI_CONFIG_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// uncomment this to use static memory instead of dynamic
//#define INI_STATIC_MEMORY_SIZE 10000

// uncomment this to forbid multiline values
//#define INI_FORBID_MULTILINE

// enum ini_utf8 { INI_FORBID_UTF8, INI_ALLOW_UTF8, INI_ALLOW_UTF8_ON_BOM };

enum ini_utf8_mode {
  INI_UTF8_MODE_FORBID,
  INI_UTF8_MODE_ALLOW,
  INI_UTF8_MODE_ALLOW_WITH_BOM,
  INI_UTF8_MODE_ESCAPE
};

/**
 * Data structure for .INI files.
 */
struct ini_file {
  enum ini_utf8_mode utf8_mode;
  FILE *file;
  char equals;
  char comment;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_CONFIG_H
