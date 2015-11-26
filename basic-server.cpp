/***********************************************************************
 basic-server.cpp - Implements a fairly basic single-threaded Winsock 
    server program that waits for a connection, accepts it, echoes back
    any data it receives, and then goes back to listening when the
    client drops the connection.

 Compiling:
    VC++: cl -GX basic-server.cpp main.cpp ws-util.cpp wsock32.lib
    BC++: bcc32 basic-server.cpp main.cpp ws-util.cpp
    
 This program is hereby released into the public domain.  There is
 ABSOLUTELY NO WARRANTY WHATSOEVER for this product.  Caveat hacker.
***********************************************************************/

#include "ws-util.h"
#include "protocol.h"
#include <winsock.h>

#include <iostream>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////
// Constants

const int kBufferSize = 1024;
char *requestDataStr = "requestingData";
char *sendDataStr = "sendingData";
const char *specialPhrase = "StruggleBus";

////////////////////////////////////////////////////////////////////////
// Prototypes
protocolPacket *readPEoutput();
SOCKET SetUpListener(const char* pcAddress, int nPort);
SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote);
//bool EchoIncomingPacketsStruct(SOCKET sd);
bool EchoIncomingPackets(SOCKET sd, bool firstMsg, bool secondMsg);
bool firstMsg, secondMsg = false;


//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(const char* pcAddress, int nPort, protocolPacket *p, bool dataFromServer)
{
	
    // Begin listening for connections
    cout << "Establishing the listener..." << endl;
    SOCKET ListeningSocket = SetUpListener(pcAddress, htons(nPort));
    if (ListeningSocket == INVALID_SOCKET) {
        cout << endl << WSAGetLastErrorMessage("establish listener") << 
                endl;
        return 3;
    }

    // Spin forever handling clients
    while (1) {
        // Wait for a connection, and accepting it when one arrives.
        cout << "Waiting for a connection..." << flush;
        sockaddr_in sinRemote;
        SOCKET sd = AcceptConnection(ListeningSocket, sinRemote);
        if (sd != INVALID_SOCKET) {
            cout << "Accepted connection from " <<
                    inet_ntoa(sinRemote.sin_addr) << ":" <<
                    ntohs(sinRemote.sin_port) << "." << endl;
					firstMsg = true;
        }
        else {
            cout << endl << WSAGetLastErrorMessage(
                    "accept connection") << endl;
            return 3;
        }
        
        // Bounce packets from the client back to it.
        if (EchoIncomingPackets(sd, firstMsg, secondMsg)) {
            // Successfully bounced all connections back to client, so
            // close the connection down gracefully.
            cout << "Shutting connection down..." << flush;
			
            if (ShutdownConnection(sd)) {
                cout << "Connection is down." << endl;
            }
            else {
                cout << endl << WSAGetLastErrorMessage(
                        "shutdown connection") << endl;
                return 3;
            }
        }
        else {
            cout << endl << WSAGetLastErrorMessage(
                    "echo incoming packets") << endl;
            return 3;
        }
		
    }

#if defined(_MSC_VER)
    return 0;       // warning eater
#endif
}


//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.

SOCKET SetUpListener(const char* pcAddress, int nPort)
{
    u_long nInterfaceAddr = inet_addr(pcAddress);
    if (nInterfaceAddr != INADDR_NONE) {
        SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
        if (sd != INVALID_SOCKET) {
            sockaddr_in sinInterface;
            sinInterface.sin_family = AF_INET;
            sinInterface.sin_addr.s_addr = nInterfaceAddr;
            sinInterface.sin_port = nPort;
            if (bind(sd, (sockaddr*)&sinInterface, 
                    sizeof(sockaddr_in)) != SOCKET_ERROR) {
                listen(sd, 1);
                return sd;
            }
        }
    }

    return INVALID_SOCKET;
}


//// AcceptConnection //////////////////////////////////////////////////
// Waits for a connection on the given socket.  When one comes in, we
// return a socket for it.  If an error occurs, we return 
// INVALID_SOCKET.

SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote)
{
    int nAddrSize = sizeof(sinRemote);
    return accept(ListeningSocket, (sockaddr*)&sinRemote, &nAddrSize);
}
////////////////////////////////////////////////////////////////////

protocolPacket *readPEoutput() {
	ifstream infile;
    infile.open ("serverOUTPUT.txt");
	
	int i=0;
	std::string line;
	
	protocolPacket *p = (protocolPacket *)malloc(sizeof(protocolPacket));
	p->msg_id = 0;
	
	 while (!infile.eof())
    {
      getline(infile,line);
      strcpy(p->data[i], line.c_str());
	  p->dataSize[i] = line.length();
	  i+=1;
	  if (i==5) {break;}
	}
     
	
	return p;
}







//// EchoIncomingPackets ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.


bool EchoIncomingPackets(SOCKET sd, bool firstMsg, bool secondMsg)
{

	// Read data from client
    char acReadBuffer[kBufferSize];
	memset(acReadBuffer, 0, kBufferSize);
    int nReadBytes;
    do {
		
		if (firstMsg) {
			
			nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
			if (nReadBytes > 0) {
				cout << "Received " << nReadBytes << " bytes from client." << endl;
          
		  
				firstMsg = false; secondMsg = true;
				char *str = NULL;
				str = (char *)malloc(nReadBytes * sizeof(char));
				strncpy(str,acReadBuffer,nReadBytes);
				
				if (strcmp(str, specialPhrase) != 0) {
					free(str);
					cout << "Incorrect username and password" << endl;
					return false;
				}
		   
			} else if (nReadBytes == SOCKET_ERROR) {
				return false;
			}
		}
			
		else if (secondMsg) {
			
			nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
			if (nReadBytes > 0) {
				cout << "Received " << nReadBytes << " bytes from client." << endl;
          
				secondMsg = false;
				char *str = NULL;
				str = (char *)malloc(nReadBytes * sizeof(char));
				strncpy(str,acReadBuffer,nReadBytes);
				
				if (strcmp(str,requestDataStr) == 0) {
					
					ifstream infile;
					infile.open ("serverOUTPUT.txt");
	
					int i=0, msgID=0;
					std::string line;
	
					protocolPacket *p = (protocolPacket *)malloc(sizeof(protocolPacket));
					p->msg_id = 0;
	 
					while (!infile.eof()) {
						getline(infile,line);
				        if (line.length() <=1) {
							getline(infile, line);
						}
						strcpy(p->data[i], line.c_str());
						p->dataSize[i] = line.length();
						i+=1;
						if (i==5) {
							if (send(sd, ((char *)p), sizeof(protocolPacket), 0) != SOCKET_ERROR) {
								i=0;
								p = (protocolPacket *)malloc(sizeof(protocolPacket));
								p->msg_id = ++msgID;
							} else {
							return false;
							}
							
						 }
					}
					send(sd, NULL, 0, 0);
					return true; 
				}
			} else if (nReadBytes == SOCKET_ERROR) {
				return false;
			}
		}
		
		
		else  {
			FILE *fileID;
			
			memset(acReadBuffer, 0, kBufferSize);
	
			protocolPacket *p = (protocolPacket *) malloc(sizeof(protocolPacket));
			nReadBytes = recv(sd, ((char *)p), sizeof(protocolPacket), 0);
	
			if (nReadBytes > 0) {
				cout << "Received " << nReadBytes << " bytes from client." << endl;
            
				fileID = fopen("serverOUTPUT.txt","a");
				for (int i=0; i<5; i++) {
					fprintf(fileID, "%s\n", p->data[i]);
				}
			fclose(fileID);
	 
			} else if (nReadBytes == SOCKET_ERROR) {
				return false;
			}
			
		}

    } while (nReadBytes != 0);

	
    cout << "Connection closed by peer." << endl;
    return true;
}


