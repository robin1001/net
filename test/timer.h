// Copyright (c) 2017 Personal (Binbin Zhang)
// Created on 2017-06-07
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <unistd.h>

class Timer {
 public:
  Timer() { Reset(); }
  void Reset() { gettimeofday(&this->time_start_, &time_zone_); }
  // Returns time in seconds.
  double Elapsed() {
    struct timeval time_end;
    gettimeofday(&time_end, &time_zone_);
    double t1, t2;
    t1 = static_cast<double>(time_start_.tv_sec) +
         static_cast<double>(time_start_.tv_usec / (1000 * 1000));
    t2 = static_cast<double>(time_end.tv_sec) +
         static_cast<double>(time_end.tv_usec / (1000 * 1000));
    return t2-t1;
  }

 private:
  struct timeval time_start_;
  struct timezone time_zone_;
};

#endif  // TIMER_H_
