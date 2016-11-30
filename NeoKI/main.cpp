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

#include <iostream>
#include <cstring>
#include <thread>
#include <cstdio>
#include <cstddef>
#include <alloca.h>
#include <time.h>
#include <unistd.h>
#include "BBuffer.hpp"
#include "SerialCom.hpp"

#define SIZEOFBUFF 16
#define SIZEOFTYPE 5000

using namespace std;

BBuffer bbuffer(SIZEOFBUFF, SIZEOFTYPE);

typedef struct param{
    SerialCom *port;
    FILE *arq;
}Param;

void readFile(Param *par)
{
    
}

void writeToPrinter(Param *par)
{
    
}

int main(int argc, const char * argv[])
{
    Param par[2];
    char in[1024];
    char m115[8];
    int baudrate;
    strcpy(m115, "M115\n\r");
    SerialCom port(argv[1]);
    sscanf(argv[2], "%d", &baudrate);
    if (port.openPort() != 0){
        exit(port.getError());
    }
    port.setBaudrate(baudrate);
    port.makeCanonical();
    //port.makeRAW(0, 11);
    port.drain();
    while (port.readLine(in, 1023) != 0 && strstr(in, "wait") == NULL){
        cout << in << endl;
    }
    cout << "\nREAD\n\n";
    port.writeCharVec(m115);
    for (int i = 0; i < 10; i++){
        baudrate = (int)port.readLine(in, 1023);
        if (baudrate == 0 || strstr(in, "wait") != NULL){
            break;
        }
        if (baudrate < 0){
            exit(port.getError());
        }
        cout << i << " " <<in << endl;
    }
    par[0].arq = fopen("../../../../../NeoKI/r2d2.gcode", "rt");
    if (par[0].arq == NULL){
        cout << "Error\n";
        exit(errno);
    }
    par[1].port = &port;
    
    return 0;
}
