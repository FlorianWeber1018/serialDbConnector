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

		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout<<"app started"<<endl;
		ArduIoInterface* m_ArduIoInterface = new ArduIoInterface("/dev/ttyACM0", 57600, "localhost", 3306, "IoD", "637013", "heizung");
		while(1){
			if(m_ArduIoInterface->connect()){
				cout<<"sucessfully connected to both"<<std::endl;
				m_ArduIoInterface->mainloop();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	cout<<"app ended"<<endl;
	return 0;

}
