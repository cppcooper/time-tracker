#include "../include/key_wait.h"

int16_t key_wait()
{
    union Key
    {
        int16_t key;
        int8_t  key_bytes[2];
    };
    Key input;
    input.key = 0;
    for(int i = 0; i < 2; ++i)
    {
        uint8_t j = 0;
        while(!_kbhit()){}
        input.key_bytes[i] = _getch();
        switch(input.key_bytes[i])
        {
            case 0:
            case -32:
                break;
            default:
                return input.key;
        }
    }
    return -1;
}

int16_t key_wait(void (*callback)())
{
    union Key
    {
        int16_t key;
        int8_t  key_bytes[2];
    };
    Key input;
    input.key = 0;
    for(int i = 0; i < 2; ++i)
    {
        uint8_t j = 0;
        while(!_kbhit()){
            callback();
            if(++j > 10)
                return -1;
        }
        input.key_bytes[i] = _getch();
        switch(input.key_bytes[i])
        {
            case 0:
            case -32:
                break;
            default:
                return input.key;
        }
    }
    return -1;
}

int16_t key_wait(void (*callback)(void*), void* arg)
{
    union Key
    {
        int16_t key;
        int8_t  key_bytes[2];
    };
    Key input;
    input.key = 0;
    for(int i = 0; i < 2; ++i)
    {
        uint8_t j = 0;
        while(!_kbhit()){
            callback(arg);
            if(++j > 10)
                return -1;
        }
        input.key_bytes[i] = _getch();
        switch(input.key_bytes[i])
        {
            case 0:
            case -32:
                break;
            default:
                return input.key;
        }
    }
    return -1;
}