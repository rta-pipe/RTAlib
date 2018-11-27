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

#include "RedisPublisher.hpp"

RedisPublisher::RedisPublisher(string configFilePath) {

  config = Config::getIstance(configFilePath);
  hostname = config->file["Redis"]["host"].getString();
  password = config->file["Redis"]["password"].getString();

  c = c_redis_pub_connect(hostname.c_str(), password.c_str());

  if( c != NULL ) {


    #ifdef DEBUG
    cout << "[RedisPublisherC++] Connected" << endl;
    #endif

  }else{

    cout << "[RedisPublisherC++] Connection FAILED!" << endl;

  }

}

int RedisPublisher::Publish(string channel, map <string, string> event ) {

  #ifdef DEBUG
  cout << "[RedisPublisher]: Publish!" <<endl;
  #endif


  string message;

  for (map< string ,string >::iterator it=event.begin(); it!=event.end(); ++it) {

    message += it->first + " " + it->second + " ";

  }

  #ifdef DEBUG
  cout << "[RedisPublisher] message: " <<  message << endl;
  cout << "[RedisPublisher] channel: "<<channel << endl;
  #endif

  c_publish(c, channel.c_str(), message.c_str());

}
