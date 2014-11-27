#include <iostream>
#include <stdint.h>
#include <stdio.h>
using namespace std;



int main(int argc, char const *argv[])
{
	uint8_t r[4] = {0xA,0xB,0xC,0xD};

	updateTx(r);

	return 0;
}