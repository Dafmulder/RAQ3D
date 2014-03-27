#include <iostream>
using namespace std;

unsigned char byteBuffer[8] = {0x00, 0x00, 0x00, 0x00};
unsigned char programStack[8][8] =
{{0x01, 0x0F, 0xFF, 0x04},	// compare function ( 0x0F > 0xFF ) -> False -> return 0x04
{0x01, 0xFF, 0xAA, 0x02},	// compare function ( 0xFF > 0xAA ) -> True -> return 0x02
{0x02, 0x33, 0x44, 0xFF},	// compare function ( 0xFF < 0xAA ) -> True -> return 0xFF
{0x03, 0x00, 0x00, 0x00},	// exit program with exit code 0x00
{0x00, 0x00, 0x00, 0x00}};	// NOP (no operations)

unsigned char function01(unsigned char inputA, unsigned char inputB, unsigned char inputC);
unsigned char function02(unsigned char inputA, unsigned char inputB, unsigned char inputC);
unsigned char function03(unsigned char inputA);

void loadLine(unsigned char line)
{
	memcpy(byteBuffer, programStack[line], 8);
}

void doFunction(unsigned char funcId)
{
	switch(funcId){
		case 0x01:
			cout << "return function01 = " << hex << (int) function01(byteBuffer[1], byteBuffer[2], byteBuffer[3]) << endl;
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
	cout << "end of program : code " << inputA << endl;
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
