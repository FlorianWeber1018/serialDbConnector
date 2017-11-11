#ifndef __serialcmdinterface_h
#define __serialcmdinterface_h

#include <string>
#include <list>
using namespace std;
struct serialCmdInterfacePacket
{
	list<string> request;
	list<string> answer;
	bool packetComplete;
}




class serialCmdInterface
{
	public:
		serialCmdInterface(string device, string baudrate);
		~serialCmdInterface();
		bool getConnectionState();
		
		
		bool connect();
		void disconnect();
		void run();
		void stop();
	private:
		bool sendCommand(string cmd);
		bool connectionEstablished;
		string device;
		string baudrate;
		list<SerialPacketCmd> mypackets;
}




#endif
