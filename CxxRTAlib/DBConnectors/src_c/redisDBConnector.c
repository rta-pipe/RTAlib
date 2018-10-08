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

#include "redisDBConnector.h"


redisContext *c;
redisReply *reply;
int rc_commandsSent = 0;

int connection(const char *hostname, const char * password,const char * database) {

  int port = 6379;

  struct timeval timeout = { 1, 500000 }; // 1.5 seconds

  c = redisConnectWithTimeout(hostname, port, timeout);

  if (c == NULL || c->err) {

      if (c) {

          printf("Connection error: %s\n", c->errstr);
          redisFree(c);

      } else {

          printf("Connection error: can't allocate redis context\n");

      }


  }

  reply = redisCommand(c,"AUTH %s", password);

  printf("AUTH: %s\n", reply->str);

  reply = redisCommand(c,"select %s",database);

  printf("SELECT: %s\n", reply->str);

  freeReplyObject(reply);

  return 0;

}

int close_connection() {

  reply = redisCommand(c, "EXEC");

  freeReplyObject(reply);

  /* Disconnects and frees the context */
  redisFree(c);

  return 0;
}


int streamingInsert_c(const char* modelName, const char* score, const char* query){  //query


  reply = redisCommand(c,"ZADD %s %s %s", modelName, score, query); //


  if(reply->str != NULL) {

    printf("ZADD: %s\n", reply->str);

    rc_commandsSent = -1;

    exit(-1);

  } else {

    rc_commandsSent++;

  }

  freeReplyObject(reply);

  return rc_commandsSent;

}

int batchInsert_c(const char * modelName, const char * score, const char * query, int batchsize){

  if(rc_commandsSent==0) {

    redisCommand(c,"MULTI");

    reply = redisCommand(c,"ZADD %s %s  %s", modelName, score, query);

    rc_commandsSent++;

  }else if(rc_commandsSent < batchsize) {

    reply = redisCommand(c,"ZADD %s %s  %s", modelName, score, query);

    rc_commandsSent++;

  }else if(rc_commandsSent >= (batchsize)){

    reply = redisCommand(c,"ZADD %s %s  %s", modelName, score, query); 

    redisCommand(c,"EXEC");

    rc_commandsSent = 0;

    batchInsert_c(modelName,score,query,batchsize);

  }

  // freeReplyObject(reply);
  return rc_commandsSent;

}
