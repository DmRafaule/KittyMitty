#pragma once

#define OUT(str, ...) printf(str,##__VA_ARGS__)

#define NOTHING 0x00 //0b00000000
#define FLOOR   0x01 //0b00000001
#define WALL    0x02 //0b00000010
#define STEPS   0x04 //0b00000100
#define ROOF    0x08 //0b00001000