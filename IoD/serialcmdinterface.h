#ifndef __serialcmdinterface_h
#define __serialcmdinterface_h

#include <wiringSerial.h>
#include <string>
#include <list>
#include "COMprotocol.h"
#include <thread>







class serialCmdInterface
{
	public:
		serialCmdInterface(std::string device, int baudrate);
		~serialCmdInterface();
		bool getConnectionState();
		bool serialFlush(std::string cmd);
		bool connect();
		void disconnect();
		void run();
		void stop();
	protected:
		void startListening();
		void startSending();
		void stopSending();
		void stopListening();
		virtual void serialDispatcher(std::string cmd);
		void Sending();
		void Listening();
		std::string device;
		int baudrate;
		bool sendEnable;
		bool listenEnable;
		bool connectionEstablished;
		int handle = -1;
		char pollOne();
		bool sendOne(std::string m_string);
		bool sendOne(char m_char);
		std::list<std::string> bufOut;
		std::list<std::string> bufIn;
		volatile bool rtr;
		//convert Funktions especialy for the Protocol
		std::string to_flushString(short number);
		std::string to_flushString(unsigned char number);
		unsigned char to_uchar(const std::string& flushString);
		short to_short(const std::string& flushString);
		void plotFlushStringToConsole(const std::string& flushString);
};




#endif
