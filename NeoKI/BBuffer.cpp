// Copyright [2016] [Ayrton Cavalieri de Almeida]
 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
 
// http://www.apache.org/licenses/LICENSE-2.0
 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "BBuffer.hpp"

BBuffer::BBuffer(std::size_t sizeOfBuffer, std::size_t sizeOfType) : _empty(sizeOfBuffer), _full(0), _toSend(0), _sent(0), _sizeOfBuffer(sizeOfBuffer), _sizeOfType(sizeOfType), info(new uint8_t[sizeOfBuffer * sizeOfType])
{
    //info = calloc(sizeOfBuffer, sizeOfType);
}

BBuffer::~BBuffer()
{
    uint8_t *ptr = (uint8_t*)info;
    delete [] ptr;
    //free(info);
}

void BBuffer::enqueue(void *buf)
{
    void *ptr;
    _empty.down();
    ptr = (void*)((std::size_t)info + ((_toSend % _sizeOfBuffer) * _sizeOfType));
    ++_toSend;
    memcpy(ptr, buf, _sizeOfType);
    _full.up();
}

void BBuffer::dequeue(void **buf)
{
    void *ptr;
    _full.down();
    ptr = (void*)((std::size_t)info + ((_sent % _sizeOfBuffer) * _sizeOfType));
    ++_sent;
    memcpy(*buf, ptr, _sizeOfType);
     _empty.up();
}

std::size_t BBuffer::sizeOfBuffer()
{
    return _sizeOfBuffer;
}

std::size_t BBuffer::sizeOfDataType()
{
    return _sizeOfType;
}
