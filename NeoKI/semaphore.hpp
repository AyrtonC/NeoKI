// Copyright [2016] [Ayrton Cavalieri de Almeida]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef Semaphore_hpp
#define Semaphore_hpp

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstddef>

class Semaphore {
private:
    std::atomic<std::size_t> _n;
    std::mutex mtx;
    std::condition_variable cv;
public:
    Semaphore(std::size_t n);
    Semaphore(const Semaphore&) = delete;
    Semaphore(Semaphore&&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;
    
    void up();
    void down();
    bool tryDown();
};

#endif /* semaphore_hpp */
