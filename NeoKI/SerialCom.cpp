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

#include "SerialCom.hpp"

SerialCom::SerialCom(std::string serialPort, int baudrate) : _serialPort(serialPort), _fd(0), _baudRate(baudrate), _error(0)
{
    
}

SerialCom::~SerialCom()
{
    
}

std::string SerialCom::getSerialPort()
{
    return _serialPort;
}

int SerialCom::openPort()
{
    _fd = open(_serialPort.c_str(), O_RDWR | O_NONBLOCK);
    
    return 0;
}

void SerialCom::closePort()
{
    
}