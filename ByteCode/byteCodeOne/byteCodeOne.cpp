#include <iostream>
#include <string.h>
#include <cstdlib>
using namespace std;

unsigned char byteBuffer[8] = {0x00, 0x00, 0x00, 0x00};
unsigned char programStack[8][8] =
{{0x01, 0x02, 0x03, 0x04},	// compare function ( mempoint(0x02) > mempoint(0x04) ) -> return mempoint(0x04)
{0x01, 0x03, 0x04, 0x02},	// compare function ( mempoint(0x03) > mempoint(0x04) ) -> return mempoint(0x02)
{0x02, 0x04, 0x02, 0x00},	// compare function ( mempoint(0x04) < mempoint(0x02) ) -> return mempoint(0x00)
{0x03, 0x00, 0x00, 0x00},	// exit program with exit code 0x00
{0x00, 0x00, 0x00, 0x00}};	// NOP (no operations)

unsigned char memStack[8][3] =
{{0x00, 0x00},	
{0x00, 0x08},	
{0x7F, 0x06},	
{0x00, 0x07},	
{0x00, 0x00}};

unsigned char function01(unsigned char inputA, unsigned char inputB, unsigned char inputC);
unsigned char function02(unsigned char inputA, unsigned char inputB, unsigned char inputC);
unsigned char function03(unsigned char inputA);

void loadLine(unsigned char line)
{
	memcpy(byteBuffer, programStack[line], 8);
}

int16_t memVar(unsigned char point)
{
	int16_t ret = 0;		// Initialize return value
	ret = memStack[point][1];	// Push least significant byte to return
	ret += (memStack[point][0] & 0x7F) << 8;	// Discart sign bit and push most significant byte (actually 7 bits)
	if(memStack[point][0] &= 0b10000000) ret = -ret;	// If sign bit is high make return negative
	return(ret);
}

void doFunction(unsigned char funcId)
{
	switch(funcId){
		case 0x01:
			cout << "return function01 = " << hex << (int) function01(memVar(byteBuffer[0]), memVar(byteBuffer[1]), memVar(byteBuffer[2])) << endl;
			break;
		case 0x02:
			cout << "return function02 = " << hex << (int) function02(byteBuffer[1], byteBuffer[2], byteBuffer[3]) << endl;
			break;
		case 0x03:
			function03(byteBuffer[1]);
		default:
			break;
	}
}

unsigned char function01(unsigned char inputA, unsigned char inputB, unsigned char inputC)
{
	if(inputA>inputB)
	{
		return(inputC);					// Return inputC as value if B < A
	}
	return(0);							// Comparison is false
}

unsigned char function02(unsigned char inputA, unsigned char inputB, unsigned char inputC)
{
	if(inputA<inputB)
	{
		return(inputC);					// Return inputC as value if B > A
	}
	return(0);							// Comparison is false
}

unsigned char function03(unsigned char inputA)
{
	cout << "end of program : code " << hex << (int) inputA << endl;
	exit(inputA);						// Exit program with return value from input
}

int main() {
	cout << "Hello, you've been RAQ3D!" << endl; // prints Hello, you've been RAQ3D!
	int i = 0;							// Program iterator
	while(1)
	{
		if(!programStack[i][0]==0x00)	// If function byte is not NOP
		{
			loadLine(i);				// Load program line into working memory
			doFunction(byteBuffer[0]);	// Do function
		}
		i++;
	}
	return 0;
}
