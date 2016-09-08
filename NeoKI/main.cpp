//
//  main.cpp
//  NeoKI
//
//  Created by Ayrton Cavalieri on 2/09/2016.
//  Copyright © 2016 AyrtonC. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <thread>
#include <cstdio>
#include <alloca.h>
#include "BBuffer.hpp"

#define SIZEOFBUFF 16
#define SIZEOFTYPE 5000

using namespace std;

typedef struct param{
    BBuffer *bbuffer;
    FILE *arq;
}Param;

void readFile(Param *par)
{
    char in[SIZEOFTYPE + 1];
    while (fscanf(par->arq, "%5000[^\n]\n", in) != EOF){
        par->bbuffer->enqueue(in);
    }
    strcpy(in, "EOF");
    par->bbuffer->enqueue(in);
}

void writeFile(Param *par)
{
    char *out;
    out = (char*)alloca((SIZEOFTYPE + 1) * sizeof(char));
    while (true){
        par->bbuffer->dequeue((void**)&out);
        if (strcmp(out, "EOF") != 0){
            fprintf(par->arq, "%s\n", out);
        }else{
            break;
        }
    }
}

int main(int argc, const char * argv[]) {
    char fileLine[2][SIZEOFTYPE + 1];
    char fileName[64];
    char output[64];
    BBuffer bbuffer(SIZEOFBUFF, SIZEOFTYPE);
    thread threads[2];
    Param params[2];
    strcpy(fileName, "/Users/ayrtex/Documents/Habilite/NeoKI/NeoKI/Cube.gcode");
    strcpy(output, "/Users/ayrtex/Documents/Habilite/NeoKI/NeoKI/Cube0.gcode");
    params[0].arq = fopen(fileName, "rt");
    if (params[0].arq == NULL){
        printf("Arquivo não encontrado.\n");
        return 1;
    }
    params[1].arq = fopen(output, "w+");
    if (params[1].arq == NULL){
        printf("Não foi possível criar o arquivo.\n");
        fclose(params[0].arq);
        return 2;
    }
    params[0].bbuffer = &bbuffer;
    params[1].bbuffer = &bbuffer;
    threads[0] = thread(readFile, &params[0]);
    threads[1] = thread(writeFile, &params[1]);
    for (auto& th : threads){
        th.join();
    }
    if (fseek(params[0].arq, 0, SEEK_SET) != 0){ //Rewind in file.
        cout << "Erro ao rebobinar o arquivo de entrada.\n";
        return -10;
    }
    if (fseek(params[1].arq, 0, SEEK_SET) != 0){ //Rewind out file.
        cout << "Erro ao rebobinar o arquivo de saída.\n";
        return -11;
    }
    for (int i = 0; fscanf(params[0].arq, "%5000[^\n]\n", fileLine[0]) != EOF; ++i){
        fscanf(params[1].arq, "%5000[^\n]\n", fileLine[1]);
        if (strcmp(fileLine[0], fileLine[1]) != 0){
            printf("Linha %d com conteúdo incongruente.\nCONTEÚDO ARQUIVO 1: %s\nCONTEÚDO ARQUIVO 2:%s\n", i + 1, fileLine[0], fileLine[1]);
            fclose(params[0].arq);
            fclose(params[1].arq);
            return -3;
        }
    }
    fclose(params[0].arq);
    fclose(params[1].arq);
    cout << "Programa concluído.\n";
    return 0;
}
