//
//  BBuffer.cpp
//  NeoKI
//
//  Created by Ayrton Cavalieri on 2/09/2016.
//  Copyright © 2016 AyrtonC. All rights reserved.
//

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
