//
//  semaphore.hpp
//  NeoKI
//
//  Created by Ayrton Cavalieri on 2/09/2016.
//  Copyright © 2016 AyrtonC. All rights reserved.
//

#ifndef semaphore_hpp
#define semaphore_hpp

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
