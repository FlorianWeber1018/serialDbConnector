#include "main.h"
#include <iostream>
#include "serialcmdinterface.h"
#include "mysqlcon.h"
#include <string>
#include "COMprotocol.h"
#include <chrono>
#include <thread>
#include "ArduIoInterface.h"
using namespace std;


//meine main
int main()
{
	cout<<"app started"<<endl;

		ArduIoInterface* m_ArduIoInterface = new ArduIoInterface("/dev/ttyACM0", 57600, std::string host, unsigned int port, "192.168.178.92", 3306, "root", "637013", "heizung");
/*		m_SerialInterface->run();
		string myCMD1 = "freeMem";
		string myCMD2 = "io get value adc0";
//		cout << "string prepared for flush(): " << myCMD << endl;
		m_SerialInterface->flush(myCMD1);
		m_SerialInterface->run();*/
		while(1){
//			m_SerialInterface->flush(myCMD1);
//			m_SerialInterface->flush(myCMD2);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	cout<<"app ended"<<endl;
	return 0;

}
