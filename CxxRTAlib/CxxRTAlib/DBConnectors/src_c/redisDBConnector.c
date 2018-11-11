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

redisContext * connection(int idConnector, const char *hostname, const char * password,const char * database) {

  redisReply * reply;

  #ifdef DEBUG
  printf("[RedisDBConnector C %d ]Connecting..\n",idConnector);
  #endif


  int port = 6379;

  struct timeval timeout = { 1, 500000 }; // 1.5 seconds

  redisContext * c = redisConnectWithTimeout(hostname, port, timeout);
  // c = redisConnect(hostname, port);

  if (c == NULL || c->err) {
      if (c) {
          printf("[RedisDBConnector C %d ]Connection error: %s\n",idConnector, c->errstr);
          redisFree(c);
      } else {
          printf("[RedisDBConnector C %d] Connection error: can't allocate redis context\n",idConnector);
      }

      return NULL;
  }

  reply = redisCommand(c,"AUTH %s", password);

  bool checkAuth = checkRedisReply(c, reply, idConnector, "AUTH");
  if(! checkAuth){
    return NULL;
  }

  reply = redisCommand(c,"select %s",database);
  bool checkSelect = checkRedisReply(c,reply, idConnector,"SELECT");
  if(! checkSelect){
    return NULL;
  }

  return c;
}

bool close_connection(redisContext *c, bool flagTransaction,int idConnector) {

  if (flagTransaction) {
    redisReply * reply = redisCommand(c, "EXEC");
    bool checkCommit = checkRedisReply(c, reply, idConnector, "EXEC - close_connection()");
    if(! checkCommit){
      return false;
    }
  }

  /* Disconnects and frees the context */
  redisFree(c);

  #ifdef DEBUG
  printf("[RedisDBConnector C %d] disconected.\n",idConnector);
  #endif

  return true;
}

bool checkRedisReply(redisContext *c,redisReply * reply, int idConnector, char* functionCalling){

  if(reply){
    #ifdef DEBUG
    printf("[RedisDBConnector C %d] %s reply->str: %s\n",idConnector, functionCalling, reply->str);


    if(reply->type==REDIS_REPLY_STATUS){
      printf("REDIS_REPLY_STATUS\n");
    }
    else if(reply->type==REDIS_REPLY_ERROR){
      printf("REDIS_REPLY_ERROR\n");
    }
    else if(reply->type==REDIS_REPLY_INTEGER){
      printf("REDIS_REPLY_INTEGER: %llu\n",reply->integer);
    }
    else if(reply->type==REDIS_REPLY_NIL){
      printf("REDIS_REPLY_NIL\n");
    }
    else if(reply->type==REDIS_REPLY_STRING){
      printf("REDIS_REPLY_STRING\n");
    }
    else if(reply->type==REDIS_REPLY_ARRAY){
      printf("REDIS_REPLY_ARRAY\n");
    }
    else {
      printf("REDIS_REPLY_??\n");
    }
    #endif

  freeReplyObject(reply);

  return true;

  }
  else{
    #ifdef DEBUG
    printf("[RedisDBConnector C %d] %s reply is NULL\n",idConnector, functionCalling);
    #endif

    if(c->err == REDIS_ERR_IO){
      printf("[RedisDBConnector C %d] REDIS_ERR_IO!! There was an I/O error while creating the connection, trying to write to the socket or read from the socket. If you included errno.h in your application, you can use the global errno variable to find out what is wrong. Err: %s\n",idConnector, c->errstr);
    }
    else if(c->err==REDIS_ERR_EOF){
      printf("[RedisDBConnector C %d] REDIS_ERR_EOF!! The server closed the connection which resulted in an empty read. Err: %s\n", idConnector, c->errstr);
    }
    else if(c->err==REDIS_ERR_PROTOCOL){
      printf("[RedisDBConnector C %d] REDIS_ERR_PROTOCOL!! There was an error while parsing the protocol. Err: %s\n", idConnector,c->errstr);

    }
    else if(c->err==REDIS_ERR_OTHER){
      printf("[RedisDBConnector C %d] REDIS_ERR_OTHER!! Any other error. Currently, it is only used when a specified hostname to connect to cannot be resolved. Err: %s\n", idConnector,c->errstr);
    }
    else{
      printf("[RedisDBConnector C %d] REDIS_ERR_ %s\n",idConnector,c->errstr);
    }

    return false;

  }
}


bool streamingInsert_c(redisContext *c, int idConnector,const char* modelName, const char* score, const char* query){  //query

  redisReply * reply;

  #ifdef DEBUG
  printf("[RedisDBConnector C %d] streamingInsert_c(): ZADD %s %s %s\n", idConnector, modelName, score, query);
  #endif
  reply = redisCommand(c, "ZADD %s %s %s", modelName, score, query);

  return checkRedisReply(c, reply, idConnector, "streamingInsert");
}

bool batchInsert_c(int rc_commandsSent, redisContext *c, int idConnector,const char * modelName, const char * score, const char * query, int batchsize){

  redisReply * reply;

  if(rc_commandsSent==0) {
    reply = redisCommand(c,"MULTI");
    if(! checkRedisReply(c,reply,idConnector,"batchInsert_c_MULTI"))
      return false;
  }


  reply = redisCommand(c,"ZADD %s %s  %s", modelName, score, query);

  if(checkRedisReply(c,reply,idConnector,"batchInsert_c_ZADD")) {
    rc_commandsSent++;
  }
  else {
    return false;
  }

  if(rc_commandsSent >= batchsize){

    reply = redisCommand(c,"EXEC");

    if(checkRedisReply(c,reply,idConnector,"EXEC batchinsert-c()")) {

      return true;

    }
    else{
      return false;

    }

  }

  return true;

}

bool executeQuery_c(redisContext *c, int idConnector,const char *query){

  redisReply * reply;

  // #ifdef DEBUG
  printf("[RedisDBConnector C %d] executeQuery_c(): %s\n", query);
  // #endif
  reply = redisCommand(c, query);

  return checkRedisReply(c, reply, idConnector, "executeQuery");

}
