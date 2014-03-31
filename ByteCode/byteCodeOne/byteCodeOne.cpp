#include <iostream>

#include <string.h>

#include <cstdlib>

#include <stdint.h>

using namespace std;



uint8_t byteBuffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t programStack[8][5] =

{{0x01, 0x01, 0x02, 0x01, 0x01},	// Addition on x01 by value in x02, store in x01, jump to x01

{0x06, 0x01, 0x03, 0x00, 0x02},	 // Compare x01 with 0x02, if x01 > x02 do line x03 else x01

{0x07, 0x00, 0x00, 0x00, 0xFF},

{0x06, 0x00, 0x00, 0x00, 0xFF},	 // exit program with exit code 0x00

{0xFF, 0x00, 0x00, 0x00, 0xFF}};	// NOP (no operations)



/*

bytecode [FUNCTION] [INPUTA] [INPUTB] [OUTPUT] [JMP]

e.g. [0x01] [0x02] [0x03] [0x04] [0x05]

Will do:



SUM(INPUTA, INPUTB) -> OUPUT -> JUMP

*/



uint8_t memStack[8][2] =

{{0x00, 0x00},

{0x00, 0x00},	// i

{0x00, 0x01},	// + 1

{0x00, 0x05},	// compare value

{0x00, 0x08},

{0x85, 0x06},

{0x00, 0x07},

{0x00, 0x00}};



uint8_t sum();	 // Addition ( a + b )

uint8_t subtract();	// Substraction ( a - b )

uint8_t mul();	 // Multiplication ( a * b )

uint8_t div();	 // Division ( a / b )

uint8_t comp();	 // Comparison ( a > b )

uint8_t compex();	// Comparison with execute ( a > b ) -> execute ( c )

uint8_t stop();	 // End run ( return a )

uint8_t (*OP[8])();	// Function array

int16_t memVar(uint8_t point);



void loadLine(uint8_t line)

{

cout << "jmp -> " << (int)line << endl;

memcpy(byteBuffer, programStack[line], 5);	// Copy line from program stack to bytebuffer

cout

<< "---------"

<< endl

<< "b0: " << (int) byteBuffer[0]

<< " b1: " << (int)byteBuffer[1]

<< " b2: " << (int)byteBuffer[2]

<< " b3: " << (int)byteBuffer[3]

<< " b4: " << (int)byteBuffer[4]

<<endl;



cout

<< "m0: " << memVar(0)

<< " m1: " << memVar(1)

<< " m2: " << (int)memVar(2)

<< " m3: " << (int)memVar(3)

<< " m4: " << (int)memVar(4)

<<endl;



loadLine((*OP[byteBuffer[0]])());	 // Execute line and show

}



void loadFunc()	// Load function pointers into array

{

OP[0] = NULL;

OP[1] = sum;

OP[2] = subtract;

OP[3] = mul;

OP[4] = div;

OP[5] = comp;

OP[6] = compex;

OP[7] = stop;

}



int16_t memVar(uint8_t point)	 // Take variable from memory

{

int16_t ret = 0;	 // Initialize return value

ret = memStack[point][1];	 // Push least significant byte to return

ret += (memStack[point][0] & 0x7F) << 8;	// Discard sign bit and push most significant byte (actually 7 bits)

if(memStack[point][0] &= 0b10000000) ret = -ret;	// If sign bit is high make return negative

return(ret);

}



void memSet(uint8_t loc, int16_t dat)

{

cout << "memset loc : " << (int)loc << " dat : " << (int)dat << endl;

bool checkAbs = (dat < 0);

dat = abs(dat);	 // TODO remove abs... find out why bits fuckup if you don't

memStack[loc][0] = (dat >> 8) & 0x7F;	 // Shift most significant byte into place and remove sign bit

if(checkAbs)

{

memStack[loc][0]|=0x80;	 // If input value was negative, add sign bit

}

memStack[loc][1] += dat;	 // Add least significant bit

}



uint8_t sum()

{

memSet(byteBuffer[3], memVar(byteBuffer[1]) + memVar(byteBuffer[2]));

return(byteBuffer[4]);

}



uint8_t subtract()

{

memSet(byteBuffer[3], memVar(byteBuffer[1])-memVar(byteBuffer[2]));

return(byteBuffer[4]);

}



uint8_t mul()

{

memSet(byteBuffer[3], memVar(byteBuffer[1])*memVar(byteBuffer[2]));

return(byteBuffer[4]);

}



uint8_t div()

{

memSet(byteBuffer[3], memVar(byteBuffer[1])/memVar(byteBuffer[2]));

return(byteBuffer[4]);

}



uint8_t comp()

{

memSet(byteBuffer[3], memVar(byteBuffer[1])>memVar(byteBuffer[2]));

return(byteBuffer[4]);

}



uint8_t compex()

{

if(memVar(byteBuffer[1])>memVar(byteBuffer[2]))

{

return(byteBuffer[4]);

}

return(byteBuffer[3]);

}



uint8_t stop()

{

exit(byteBuffer[1]);

}



int main()

{

loadFunc();	// Load functions into memory

loadLine(0);

return 0;

}
