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

#ifndef StateMachine_hpp
#define StateMachine_hpp

#include <cstring>
#include <functional>
#include "SerialCom.hpp"

/////////////////////// This creates a function and I don't know how! /////////////////////////
struct FunctorHelper
{
    typedef std::function<FunctorHelper()> type;
    FunctorHelper(type f) : m_func(f) {}
    operator type() { return m_func; }
    type m_func;
};
using StateFunc = FunctorHelper::type;
///////////////////////////// DO NOT TOUCH!!! /////////////////////////////////////////////////

class StateMachine{
private:
    char readBuff[1024];
    SerialCom *_port;
    int characteristics;
    static StateFunc readPort();
    StateMachine();
public:
    void setSerialPort(SerialCom *port);
    static StateMachine& self();
    virtual ~StateMachine();
    int getStats();
};

#endif /* StateMachine_hpp */
