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

#ifndef REDIS_PUB_HPP
#define REDIS_PUB_HPP

#include "c_redisPublisher.h"
#include "Config.hpp"
#include <string>
#include <memory>

using std::string;
using std::make_shared;
using std::shared_ptr;

class RedisPublisher {
public:
  Config * config;
  redisContext * c;
  string hostname;
  string password;
  RedisPublisher(string configFilePath = "");
  int Publish(string channel, map <string, string> message);

};

#endif
