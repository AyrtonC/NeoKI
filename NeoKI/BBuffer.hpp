//
//  BBuffer.hpp
//  NeoKI
//
//  Created by Ayrton Cavalieri on 2/09/2016.
//  Copyright © 2016 AyrtonC. All rights reserved.
//

#ifndef BBuffer_hpp
#define BBuffer_hpp

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "semaphore.hpp"

class BBuffer{
private:
    Semaphore _empty;
    Semaphore _full;
    std::size_t _toSend;
    std::size_t _sent;
    std::size_t _sizeOfBuffer;
    std::size_t _sizeOfType;
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
