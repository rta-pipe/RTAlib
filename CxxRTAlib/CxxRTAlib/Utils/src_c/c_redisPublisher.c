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

#include "c_redisPublisher.h"

redisContext * c_redis_pub_connect(const char *hostname, const char * password) {

  redisContext * c = redisConnect(hostname, 6379);

  if (c == NULL || c->err) {
      if (c) {
          printf("[c_redisPublisher]Connection error: %s\n", c->errstr);
          redisFree(c);
      } else {
          printf("[c_redisPublisher] Connection error: can't allocate redis context\n");
      }

      return NULL;
  }

  return c;

}


int c_publish (redisContext * c, const char * channel, const char * message) {

    redisCommand(c, "PUBLISH %s %s",  channel, message);

    return 0;

}
