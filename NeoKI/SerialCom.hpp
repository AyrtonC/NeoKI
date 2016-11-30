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

#define MAXTRIES 3

class SerialCom{
private:
    std::string _serialPort;
    int _fd;
    speed_t _baudrate;
    int _error;
    struct termios _toptions[2]; //0 - Canonical, 1 - RAW
    int _RAWTerminal;
public:
    SerialCom(std::string serialPort);
    ~SerialCom();
    
    // getSerialPort - returns a string with the serial port path.
    std::string getSerialPort();
    
    // getBaudrate - returns a speed_t containing the speed of the port.
    speed_t getBaudrate();
    
    // openPort - open the serial port, return -1 in case of error.
    int openPort();
    
    /* setBaudrate - sets port baudrate (speed), return -1 in case of error.
     Parameters:
        baudrate - The speed of the port, must be positive. */
    bool setBaudrate(int baudrate);
    
    /* makeRAW - configures a port to work in RAW mode, return -1 in case of error.
     Parameters:
        minBytes - The minimum numbers of bytes to be read.
        waitTime - The maximum time to wait for input. */
    int makeRAW(unsigned char minBytes, unsigned char waitTime);
    
    //makeCanonical - configures a port to work in Canonical mode, return -1 in case of error.
    int makeCanonical();
    
    //closePort - Closes the port, return -1 in case of error.
    int closePort();
    
    /* readLine - attempt to read a line ended in \n and returns the number of bytes read.
     Return the number of bytes read, or -1 in case of error.
     Parameters:
        buf - char vector.
        buf_size - Size of the vector. */
    ssize_t readLine(char *buf, std::size_t buf_size);
    
    /* readBinary - attempt read n bytes from the port.
     Return the number of bytes read, or -1 in case of error.
     Parameters:
        buf - space in memory.
        buf_size - size of the space. */
    ssize_t readBinary(void *buf, std::size_t buf_size);
    
    /* writeCharVec - write a C string ended in NULL character.
     Return the number of bytes written, or -1 in case of error.
     Parameters:
        buf - NULL terminated string. */
    ssize_t writeCharVec(char *buf);
    
    /* writeBinary - attempt to write n bytes to the port.
     Return the number of bytes written, or -1 in case of error.
     Parameters:
        buf - space in memory.
        buf_size - number of bytes to be written. */
    ssize_t writeBinary(void *buf, std::size_t buf_size);
    
    // flush - flush the port content.
    void flush();
    
    //drain - drain the port content.
    int drain();
    
    // getError - return the error code of the last recorded error.
    // Definitions are found in errno header.
    int getError();
    
    //isRAWTerminal - return a int witch tells the current mode.
    //-1 - mode was not set explicitly, 0 - Canonical mode, 1 - RAW mode.
    int isRAWTerminal();
};

#endif /* SerialCom_hpp */
