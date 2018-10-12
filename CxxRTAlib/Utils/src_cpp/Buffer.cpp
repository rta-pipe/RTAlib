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

#include "Buffer.hpp"

Buffer::Buffer(std::string name, int size) {

  this->buffer = (EVTbase**) new EVTbase* [size];
  this->size = size;
  fill = 0;
  use  = 0;
  circularBuffer = 0;
  _name = name;
  semname1 = "empty_";
  semname1 += name;
  sem_unlink(semname1.c_str());
  semname2 = "full_";
  semname2 += name;
  sem_unlink(semname2.c_str());
  //sem_init(&empty, 0, size); // MAX buffers are empty to begin with...
  //sem_init(&full, 0, 0);    // ... and 0 are full
  empty = sem_open(semname1.c_str(), O_CREAT, 0644, size);
  if (empty == SEM_FAILED) {
    perror(" empty sem_open() failed ");
    exit(0);
  }
  full = sem_open(semname2.c_str(), O_CREAT, 0644, 0);
  if (full == SEM_FAILED) {
    perror(" empty sem_open() failed ");
    exit(0);
  }
  pthread_mutex_init(&mutex, NULL);

}

Buffer::~Buffer() {
	sem_close(empty);
	sem_unlink(semname1.c_str());
	sem_close(full);
	sem_unlink(semname2.c_str());
}

void Buffer::put(EVTbase* data) {
	int ret = sem_wait(empty);
	if(ret != 0) {
		perror("empty sem_wait() failed ");
		exit(0);
	}
	// scope of lock reduced
	pthread_mutex_lock(&mutex);

	this->buffer[fill] = data;
	fill = (fill + 1) % size;

	pthread_mutex_unlock(&mutex);

	ret = sem_post(full);
	if(ret != 0) {
		perror("full sem_post() failed ");
		exit(0);
	}

}

bool Buffer::isFull() {
	if(fill == size - 1)
		return true;

	return false;
}

EVTbase* Buffer::get() {
	int ret = sem_wait(full);
	if(ret != 0) {
		perror("full sem_wait() failed ");
		exit(0);
	}

	// scope of lock reduced
	pthread_mutex_lock(&mutex);

	EVTbase* b = buffer[use];
	use = (use + 1) % size;

	pthread_mutex_unlock(&mutex);

	ret = sem_post(empty);
	if(ret != 0) {
		perror("empty sem_post() failed ");
		exit(0);
	}

	return b;
}

EVTbase* Buffer::getNextCircularBuffer() {

	EVTbase* b = buffer[circularBuffer];
	circularBuffer = (circularBuffer + 1) % size;
	return b;
}


int Buffer::getBufferSize() {
	return this->size;
}
