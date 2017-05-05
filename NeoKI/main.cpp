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
#include <thread>
#include <atomic>
#include "BBuffer.hpp"
#include "SerialCom.hpp"

#define SIZEOFBUFF 4096
#define SIZEOFTYPE 128

using namespace std;

BBuffer bbuffer(SIZEOFBUFF, SIZEOFTYPE);
std::atomic<std::size_t> stop;

typedef struct param{
    SerialCom *port;
    FILE *arq;
}Param;

bool isChar(char C)
{
    return ((C >= 'A' && C <= 'Z') || (C >= 'a' && C <= 'z'));
}

bool isNum(char C)
{
    return (C >= '0' && C <= '9');
}

void readFile(Param p)
{
    char vet[5001];
    if (p.arq != NULL){
        while (fscanf(p.arq, "%5000[^\n]\n", vet) != EOF){
            if (vet[0] != ';'){
                //Preparar string.
                std::size_t i;
                for (i = 0; vet[i] != '\0'; i++){
                    if (vet[i] == ';'){
                        while (!isNum(vet[i]) && !isChar(vet[i]))
                            i--;
                        i++;
                        vet[i] = '\n';
                        vet[i + 1] = '\r';
                        vet[i + 2] = '\0';
                        break;
                    }
                }
                if (vet[i] == '\0'){
                    vet[i] = '\n';
                    vet[i + 1] = '\r';
                    vet[i + 2] = '\0';
                }
                bbuffer.enqueue(vet);
            }
        }
    }
    strcpy(vet, "EOF");
    bbuffer.enqueue(vet);
}

void writeToPrinter(Param p)
{
    std::size_t i;
    char ans[512];
    char *ptr;
    i = 0;
    ptr = (char*)malloc((5001) * sizeof(char));
    while (true){
        bbuffer.dequeue((void**)&ptr);
        if (strcmp(ptr, "EOF") == 0){
            break;
        }
        cout << i << " " << ptr;
        p.port->writeCharVec(ptr);
        p.port->readLine(ans, 256);
        cout << i << " " << ans << endl;
        ++i;
    }
    free(ptr);
}

int main(int argc, const char * argv[])
{
    Param par[2];
    char in[1024];
    char m115[8];
    int baudrate;
    long init, finali;
    strcpy(m115, "M115\n\r");
    SerialCom port(argv[1]);
    sscanf(argv[2], "%d", &baudrate);
    if (port.openPort() != 0){
        exit(port.getError());
    }
    port.setBaudrate(baudrate);
    port.makeCanonical();
    //port.makeRAW(0, 10);
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
    par[0].arq = fopen("../../../../../NeoKI/T_Rex.gcode", "rt");
    if (par[0].arq == NULL){
        cout << "Error\n";
        exit(errno);
    }
    par[1].port = &port;
    init = time(NULL);
    std::thread readfile(readFile, par[0]);
    std::thread writeSerial(writeToPrinter, par[1]);
    readfile.join();
    writeSerial.join();
    finali = time(NULL);
    printf("%ld segundos\n", finali - init);
    return 0;
}
