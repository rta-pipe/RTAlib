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

#ifndef REDIS_DB_CONNECTOR_H
#define REDIS_DB_CONNECTOR_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hiredis.h"
#include "errno.h"

redisContext * connection(int idConnector,const char *hostname, int port, const char * password, const char * database);

bool checkRedisReply(redisContext *c, redisReply * r, int idConnector, char* functionCalling);

bool close_connection(redisContext *c, bool flagTransaction, int idConnector);

int insertData(int idConnector);

bool streamingInsert_c(redisContext *c, int idConnector,const char* modelName, const char* score, const char* query);

bool batchInsert_c(int rc_commandsSent, redisContext *c,int idConnector,const char * modelName, const char * score, const char * query, int batchsize);

bool executeQuery_c(redisContext *c, int idConnector,const char *query);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // REDIS_DB_CONNECTOR_H
