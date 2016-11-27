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

#ifndef SerialCom_hpp
#define SerialCom_hpp

#include <stdint.h>   // Standard types
#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <sys/ioctl.h>
#include <iostream>

class SerialCom{
private:
    std::string _serialPort;
    int _fd;
    speed_t _baudrate;
    int _error;
    struct termios _toptions;
public:
    SerialCom(std::string serialPort);
    ~SerialCom();
    std::string getSerialPort();
    speed_t getBaudrate();
    int openPort(unsigned int baudrate);
    int closePort();
    int readUntilChar(char *buf, char until, std::size_t buf_size);
    ssize_t readBinary(void *buf, std::size_t buf_size);
    ssize_t writeCharVec(char *buf);
    ssize_t writeBinary(void *buf, std::size_t buf_size);
    void flush();
    int drain();
    int getError();
};

#endif /* SerialCom_hpp */
