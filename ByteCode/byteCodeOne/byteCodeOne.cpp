#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdint.h>
using namespace std;

uint8_t byteBuffer[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t programStack[8][4] =
{{0x01, 0x02, 0x03, 0x04},	
{0x05, 0x03, 0x04, 0x00},	
{0x02, 0x04, 0x02, 0x00},	
{0x06, 0x00, 0x00, 0x00},	// exit program with exit code 0x00
{0x00, 0x00, 0x00, 0x00}};	// NOP (no operations)

uint8_t memStack[8][2] =
{{0x00, 0x00},	
{0x00, 0x08},	
{0x7F, 0x06},	
{0x00, 0x07},	
{0x00, 0x00}};

uint8_t sum();		// Addition ( a + b )
uint8_t subtract();	// Substraction ( a - b )
uint8_t mul();		// Multiplication ( a * b ) 
uint8_t div();		// Division ( a / b ) 
uint8_t comp();		// Comparison ( a > b )
uint8_t compex();	// Comparison with execute ( a > b ) -> execute ( c ) 
uint8_t stop();		// End run ( return a ) 
uint8_t (*OP[4])();	// Function array

void loadLine(uint8_t line)
{
	memcpy(byteBuffer, programStack[line], 4);	// Copy line from program stack to bytebuffer
	cout << "b0: " << (int) byteBuffer[0] << " b1: " << (int)byteBuffer[1] << " b2: " << (int)byteBuffer[2] << " b3: " << (int)byteBuffer[3] <<endl;
}

void loadFunc()		// Load function pointers into array
{
	OP[0] = sum;
	OP[1] = subtract;
	OP[2] = mul;
	OP[3] = div;
	OP[4] = comp;
	OP[5] = compex;
	OP[6] = stop;
}

int16_t memVar(uint8_t point)		// Take variable from memory
{
	int16_t ret = 0;		// Initialize return value
	ret = memStack[point][1];	// Push least significant byte to return
	ret += (memStack[point][0] & 0x7F) << 8;	// Discart sign bit and push most significant byte (actually 7 bits)
	if(memStack[point][0] &= 0b10000000) ret = -ret;	// If sign bit is high make return negative
	return(ret);
}

uint8_t sum()
{
	return(memVar(byteBuffer[1])+memVar(byteBuffer[2]));
}

uint8_t subtract()
{
	return(memVar(byteBuffer[1])-memVar(byteBuffer[2]));
}

uint8_t mul()
{
	return(memVar(byteBuffer[1])*memVar(byteBuffer[2]));
}

uint8_t div()
{
	return(memVar(byteBuffer[1])/memVar(byteBuffer[2]));
}

uint8_t comp()
{
	return(memVar(byteBuffer[1])>memVar(byteBuffer[2]));
}

uint8_t compex()
{
	if(memVar(byteBuffer[1])>memVar(byteBuffer[2]))
	return(memVar(byteBuffer[3]));
}

uint8_t stop()
{
	exit(memVar(byteBuffer[0]));
}

int main() {
	cout << "Hello, you've been RAQ3D!" << endl; // prints Hello, you've been RAQ3D!
	int i = 0;	// Program iterator
	loadFunc();
	while(1)
	{
		if(!programStack[i][0]==0x00)	// If function byte is not NOP
		{
			loadLine(i);		// Load program line into working memory
			cout << (int)(*OP[byteBuffer[0]])() << endl;	// Execute line and show result
		}
		
		if(i > 6) exit(0);
		i++;
	}
	return 0;
}
