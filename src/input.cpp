#include "stdafx.h"
#include "input.h"

int16_t key_wait();
void clear_input();

bool verify(){
    short input = -1;
    do{
        printf("Y/N\n");
        input = key_wait();
    } while(input != 'Y' && input != 'y' && input != 'N' && input != 'n');
    return input == 'Y' || input == 'y';
}

int16_t key_wait(){
    union Key
    {
        int16_t key;
        int8_t  key_bytes[2];
    };
    Key input;
    input.key = 0;
    for(int i = 0; i < 2; ++i){
        clear_input();
        while(!_kbhit()){}
        input.key_bytes[i] = _getch();
        switch(input.key_bytes[i]){
            case 0:
            case -32:
                break;
            default:
                return input.key;
        }
    }
    return -1;
}

void clear_input(){
    while (_kbhit()){
        _getch();
    }
}
