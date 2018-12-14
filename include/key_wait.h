#pragma once
#include <conio.h>
#include <utility>
#include <cstdint>

short key_wait();
short key_wait(void (*callback)());
short key_wait(void (*callback)(void*),void* arg);