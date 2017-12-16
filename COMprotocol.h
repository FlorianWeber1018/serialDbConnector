/*
 * COMprotocol.h
 *
 * Created: 09.10.2017 21:16:22
 *  Author: Flo
 */


#ifndef PROTKOLDEF_H_
#define PROTKOLDEF_H_
	#define eot 13			//End of Transmission
	#define ack 10			//wenn befehl komplett geparst wurde		(antwort arduino->pi)
	#define nack 21			//wenn befehl nicht komplett geparst wurde
	#define _space 32		//leerzeichen
	#define SizeBufOutMax 120;
	#define SizeBufInMax 400;
#endif /* PROTKOLDEF_H_ */
