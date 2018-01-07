#ifndef __serialcmdinterface_h
#define __serialcmdinterface_h

#include <wiringSerial.h>
#include <string>
#include <list>
#include "COMprotocol.h"
#include <thread>

using namespace std;





class serialCmdInterface
{
	public:
		serialCmdInterface(string device, int baudrate);
		~serialCmdInterface();
		bool getConnectionState();
		bool serialFlush(string cmd);
		bool connect();
		void disconnect();
		void run();
		void stop();
	protected:
		void startListening();
		void startSending();
		void stopSending();
		void stopListening();
		virtual void serialDispatcher(string cmd);
		void Sending();
		void Listening();
		string device;
		int baudrate;
		bool sendEnable;
		bool listenEnable;
		bool connectionEstablished;
		int handle = -1;
		char pollOne();
		bool sendOne(string m_string);
		bool sendOne(char m_char);
		list<string> bufOut;
		list<string> bufIn;
		volatile bool rtr;

};




#endif
