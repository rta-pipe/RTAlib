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

#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <iostream>
#include <fcntl.h>

#include "EVTbase.hpp"

using std:: string;


class Buffer {

  private:

    EVTbase ** buffer;
    int fill;
    int use;
    int circularBuffer;
    sem_t* empty;
    sem_t* full;
    pthread_mutex_t mutex;
    std::string semname1;
    std::string semname2;

    std::string _name;

    int size;

public:

  Buffer(string name, int size);
  ~Buffer();
  ///Put data into local buffer
  ///The call is blocking if the buffer is full. Test it before with isFull()
  void put(EVTbase* data);

  ///get processed data from buffer
  ///The call is blocking if the buffer is empty.
  EVTbase* get();

  std::string getName() {
    return _name;
  }

  int getBufferSize();

  bool isFull();

  EVTbase* getNextCircularBuffer();


};

#endif
