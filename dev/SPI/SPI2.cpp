#include <iostream>
#include "SPI2.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	cout << endl << "starting..." << endl;
	
	Init();

	while(true)
	{
		char cmd[230];
		cout << "  Command:  " << flush;
		cin >> cmd;
		int tmp;

		if( cmd[0] == 's' )
			break;

		if( cmd[0] == 'r' )
		{
			speedA = 0;
			speedB = 0;
			speedC = 0;
			speedD = 0;
		}
		else if( cmd == "arm" )
		{
			speedA = MOTOR_ZERO_LEVEL;
			speedB = MOTOR_ZERO_LEVEL;
			speedC = MOTOR_ZERO_LEVEL;
			speedD = MOTOR_ZERO_LEVEL;
		}
		else if( cmd[0] == 'i' )
		{
			speedA++;
			speedB++;
			speedC++;
			speedD++;	
		}
		else
		{
			switch( cmd[0] )
			{
				case 'a':
					cin >> tmp;
					speedA = sratio(tmp);
					break;
				case 'b':
					cin >> tmp;
					speedB = sratio(tmp);
					break;
				case 'c':
					cin >> tmp;
					speedC = sratio(tmp);
					break;
				case 'd':
					cin >> tmp;
					speedD = sratio(tmp);
					break;
			}
		}
	}

	
	return 0;
}