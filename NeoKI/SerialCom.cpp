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

SerialCom::SerialCom(std::string serialPort) : _serialPort(serialPort), _fd(0), _baudrate(0), _error(0)
{
    
}

SerialCom::~SerialCom()
{
    if (_fd != 0)
        closePort();
}

std::string SerialCom::getSerialPort()
{
    return _serialPort;
}

speed_t SerialCom::getBaudrate()
{
    return _baudrate;
}

int SerialCom::openPort(unsigned int baudrate)
{
    if (_fd != 0){
        closePort();
    }
    _fd = open(_serialPort.c_str(), O_RDWR | O_NONBLOCK);
    if(_fd == -1){
        _error = errno;
        return -1;
    }
    if (tcgetattr(_fd, &_toptions) < 0){
        _error = errno;
        return -1;
    }
    switch(baudrate) {
        case 4800:
            _baudrate = B4800;
            break;
        case 9600:
            _baudrate = B9600;
            break;
#ifdef B14400
        case 14400:
            _baudrate = B14400;
            break;
#endif
        case 19200:
            _baudrate = B19200;
            break;
#ifdef B28800
        case 28800:
            _baudrate = B28800;
            break;
#endif
        case 38400:
            _baudrate = B38400;
            break;
        case 57600:
            _baudrate = B57600;
            break;
        case 115200:
            _baudrate = B115200;
            break;
#ifdef B230400
        case 230400:
            _baudrate = B230400;
            break;
#endif
        default:
            _baudrate = baudrate;
            break;
    }
    cfsetispeed(&_toptions, _baudrate);
    cfsetospeed(&_toptions, _baudrate);
    _toptions.c_cflag &= ~PARENB;
    _toptions.c_cflag &= ~CSTOPB;
    _toptions.c_cflag &= ~CSIZE;
    _toptions.c_cflag |= CS8;
    // no flow control
    _toptions.c_cflag &= ~CRTSCTS;
    _toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    _toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    
    _toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    _toptions.c_oflag &= ~OPOST; // make raw
    _toptions.c_cc[VMIN]  = 0;
    _toptions.c_cc[VTIME] = 0;
    tcsetattr(_fd, TCSANOW, &_toptions);
    if(tcsetattr(_fd, TCSAFLUSH, &_toptions) < 0){
        _error = errno;
        return -1;
    }
    return drain();
}

int SerialCom::closePort()
{
    int ret;
    ret = 0;
    if (_fd != 0){
        tcdrain(_fd);
        ret = close(_fd);
        _fd = 0;
    }
    return ret;
}

int SerialCom::readUntilChar(char *buf, char until, std::size_t buf_size)
{
    std::size_t i;
    ssize_t n;
    for (i = 0; i < buf_size; i++){
        n = read(_fd, &buf[i], 1);
        if (n == -1){
            _error = errno;
            buf[i] = '\0';
            return -1;
        }else if(n == 0){
            i--;
            continue;
        }else{
            if (buf[i] == until){
                buf[i] = '\0';
                break;
            }
        }
    }
    if (i == buf_size){
        buf[i - 1] = '\0';
    }
    return 0;
}

ssize_t SerialCom::readBinary(void *buf, std::size_t buf_size)
{
    ssize_t ret = read(_fd, buf, buf_size);
    if (ret == -1){
        _error = errno;
        return -1;
    }
    return ret;
}

ssize_t SerialCom::writeCharVec(char *buf)
{
    ssize_t ret = write(_fd, buf, strlen(buf));
    if (ret == -1){
        _error = errno;
        return -1;
    }
    return ret;
}

ssize_t SerialCom::writeBinary(void *buf, std::size_t buf_size)
{
    ssize_t ret = write(_fd, buf, buf_size);
    if (ret == -1){
        _error = errno;
        return -1;
    }
    return ret;
}

void SerialCom::flush()
{
    sleep(2); //required to make flush work in USB serial.
    tcflush(_fd,TCIOFLUSH);
}

int SerialCom::drain()
{
    if (tcdrain(_fd) < 0){
        _error = errno;
        return -1;
    }
    return 0;
}

int SerialCom::getError()
{
    return _error;
}
