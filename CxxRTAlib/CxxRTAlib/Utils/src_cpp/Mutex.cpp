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

#include "Mutex.hpp"
Mutex* Mutex::_instance = 0;

Mutex::Mutex(){
  #ifdef DEBUG
  cout << "Mutex" << endl;
  #endif

  pthread_mutex_init(&mutex, NULL);
}

void Mutex::mutexLock(){

  pthread_mutex_lock(&mutex);

}

void Mutex::mutexUnlock(){

  pthread_mutex_unlock(&mutex);

}

Mutex* Mutex::getIstance(){

  if(_instance == 0)
    _instance = new Mutex();

  return _instance;
}

void Mutex::deleteInstance() {

  delete _instance;

  _instance = 0;

}
