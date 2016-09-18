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

#include "StateMachine.hpp"

StateMachine::StateMachine()
{
    
}

StateMachine::~StateMachine()
{
    
}

void StateMachine::setSerialPort(SerialCom *port)
{
    self()._port = port;
    memset(self().readBuff, '\0', 1024 * sizeof(char));
}

StateMachine& StateMachine::self()
{
    static StateMachine ret;
    return ret;
}

int StateMachine::getStats()
{
    self().characteristics = 0;
    
    return self().characteristics;
}
