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

SerialCom::SerialCom(std::string serialPort) : _serialPort(serialPort), _fd(0), _baudrate(0), _error(0), _RAWTerminal(-1)
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

int SerialCom::openPort()
{
    if (_fd != 0){
        closePort();
    }
    
    _fd = open(_serialPort.c_str(), O_RDWR | O_NOCTTY);
    if(_fd == -1){
        _error = errno;
        return -1;
    }
    
    //Setting common terminal configuration
    for (int i = 0; i < 2; i++){
        if (tcgetattr(_fd, &_toptions[i]) < 0){
            _error = errno;
            return -1;
        }
        _toptions[i].c_cflag &= ~PARENB;
        _toptions[i].c_cflag &= ~CSTOPB;
        _toptions[i].c_cflag &= ~CSIZE;
        _toptions[i].c_cflag |= CS8;
    }
    //Setting first terminal configuration
    /* Canonical mode */
    _toptions[0].c_lflag |= ICANON;
    
    //Setting second terminal configuration
    // no flow control
    _toptions[1].c_cflag &= ~CRTSCTS;
    _toptions[1].c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    
    _toptions[1].c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    _toptions[1].c_oflag &= ~OPOST; // make raw
    
    _toptions[1].c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    _toptions[1].c_cc[VMIN]  = 0;
    _toptions[1].c_cc[VTIME] = 10;
    
    return 0;
}

bool SerialCom::setBaudrate(int baudrate)
{
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
    for (int i = 0; i < 2; i++){
        cfsetispeed(&_toptions[i], _baudrate);
        cfsetospeed(&_toptions[i], _baudrate);
    }
    return true;
}

int SerialCom::makeRAW()
{
    tcsetattr(_fd, TCSANOW, &_toptions[1]);
    if(tcsetattr(_fd, TCSAFLUSH, &_toptions[1]) < 0){
        _error = errno;
        return -1;
    }
    _RAWTerminal = 1;
    return 0;
}

int SerialCom::makeCanonical()
{
    tcsetattr(_fd, TCSANOW, &_toptions[0]);
    if(tcsetattr(_fd, TCSAFLUSH, &_toptions[0]) < 0){
        _error = errno;
        return -1;
    }
    _RAWTerminal = 0;
    return 0;
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

ssize_t SerialCom::readLine(char *buf, std::size_t buf_size)
{
    ssize_t tries = 0;
    ssize_t ret = 0;
    buf[0] = '\0';
    if(_RAWTerminal == 0){
ReadAgain0:
        ret = read(_fd, buf, buf_size);
        if (ret == -1){
            if (errno == EAGAIN && tries < MAXTRIES){
                tries++;
                goto ReadAgain0;
            }
            _error = errno;
            return -1;
        }else if (ret != 0){
            for (std::size_t i = 0; i < buf_size; i++){
                if (buf[i] == '\r' || buf[i] == '\n'){
                    buf[i] = '\0';
                    break;
                }
            }
        }
    }else if(_RAWTerminal == 1){
        std::size_t i;
        for (i = 0; i < buf_size; i++){
ReadAgain00:
            ret = read(_fd, &buf[i], 1);
            if (ret == -1){
                if (errno == EAGAIN && tries < MAXTRIES){
                    tries++;
                    goto ReadAgain00;
                }
                _error = errno;
                buf[i] = '\0';
                break;
            }else if(ret == 0){
                buf[i] = '\0';
                break;
            }else{
                if (buf[i] == '\r' || buf[i] == '\n'){
                    buf[i] = '\0';
                    break;
                }
            }
        }
        if (i == buf_size){
            buf[i - 1] = '\0';
        }
    }
    return ret;
}

ssize_t SerialCom::readBinary(void *buf, std::size_t buf_size)
{
    size_t times = 0;
    ssize_t ret;
ReadAgain1:
    memset(buf, '\0', buf_size); //Cleans the buffer
    ret = read(_fd, buf, buf_size);
    if (ret == -1){
        if (errno == EAGAIN && times < MAXTRIES){
            times++;
            goto ReadAgain1;
        }
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


int SerialCom::isRAWTerminal()
{
    return _RAWTerminal;
}
