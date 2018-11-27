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

#ifndef REDIS_PUB_SUB_H
#define REDIS_PUB_SUB_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "hiredis.h"


redisContext * c_redis_pub_connect(const char *hostname, const char * password);


int c_publish (redisContext *c, const char * channel, const char * message);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // REDIS_PUB_SUB_H
