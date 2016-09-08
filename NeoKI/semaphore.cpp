//
//  semaphore.cpp
//  NeoKI
//
//  Created by Ayrton Cavalieri on 2/09/2016.
//  Copyright © 2016 AyrtonC. All rights reserved.
//

#include "semaphore.hpp"

Semaphore::Semaphore(std::size_t n) : _n(n)
{
    
}

void Semaphore::up()
{
    std::unique_lock<std::mutex> lck(mtx);
    ++_n;
    cv.notify_all();
}

void Semaphore::down()
{
    std::unique_lock<std::mutex> lck(mtx);
    while (_n == 0){
        cv.wait(lck);
    }
    --_n;
}

bool Semaphore::tryDown()
{
    std::unique_lock<std::mutex> lck(mtx);
    if (_n == 0){
        return false;
    }else{
        --_n;
    }
    return true;
}