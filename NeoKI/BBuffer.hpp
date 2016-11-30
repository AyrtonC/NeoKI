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

#ifndef BBuffer_hpp
#define BBuffer_hpp

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "Semaphore.hpp"

class BBuffer{
private:
    Semaphore _empty;
    Semaphore _full;
    std::size_t _toSend;
    std::size_t _sent;
    const std::size_t _sizeOfBuffer;
    const std::size_t _sizeOfType;
    void *info;
    
public:
    BBuffer(std::size_t sizeOfBuffer, std::size_t sizeOfType);
    ~BBuffer();
    void enqueue(void *buf);
    void dequeue(void **buf);
    std::size_t sizeOfBuffer();
    std::size_t sizeOfDataType();
};

#endif /* BBuffer_hpp */
