#ifndef __serialcmdinterface_h
#define __serialcmdinterface_h

#include <wiringSerial.h>
#include <string>
#include <list>
#include "COMprotocol.h"
using namespace std;





class serialCmdInterface
{
	public:
		serialCmdInterface(string device, int baudrate);
		~serialCmdInterface();
		bool getConnectionState();


		bool connect(string device, int baudrate);
		void disconnect();
		void run();
		void stop();
	private:
		void startListening();
		void startSending();
		void dispatcher(string cmd);
		bool flush(string cmd);
		bool sendEnable;
		bool listenEnable;
		bool connectionEstablished;
		int handle = -1;
		list<SerialPacketCmd> bufOut;
		list<SerialPacketCmd> bufIn;
}




#endif
