#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <time.h>

using namespace std;

void encrypt(char *string, int stringLength);
void decrypt(char *string, int stringLength);

protocolPacket* analyzePE(char **argv) {
        protocolPacket *p = (protocolPacket *)malloc(sizeof(protocolPacket));
	    p->msg_id = 0;
		
		
        
		HANDLE hMapObject,hFile;                //File Mapping Object
        LPVOID lpBase;                          //Pointer to the base memory of mapped file
        PIMAGE_DOS_HEADER dosHeader;            //Pointer to DOS Header
		PIMAGE_NT_HEADERS ntHeader;             //Represents the PE header format
        PIMAGE_FILE_HEADER fileHeader;          //Represents the COFF header format
		PIMAGE_OPTIONAL_HEADER optionalHeader;  //Represents the optional header format
		
	
                hFile = CreateFile(argv[3],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
                if(hFile == INVALID_HANDLE_VALUE){printf("\nERROR : Could not open the file specified\n"); }
               
             
                hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
                lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
               
               
                dosHeader = (PIMAGE_DOS_HEADER)lpBase;
				ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)dosHeader + dosHeader->e_lfanew);
				fileHeader = (PIMAGE_FILE_HEADER)&ntHeader->FileHeader;
                optionalHeader = (PIMAGE_OPTIONAL_HEADER)&ntHeader->OptionalHeader;
				
			   //Check for Valid DOS file
                if(dosHeader->e_magic == IMAGE_DOS_SIGNATURE){
                     
						p->dataSize[0] = sprintf(p->data[0], "%d", optionalHeader->SizeOfInitializedData); 
					    encrypt(p->data[0], p->dataSize[0]);
				
						p->dataSize[1] = sprintf(p->data[1], "%d", fileHeader->NumberOfSymbols); 
						encrypt(p->data[1], p->dataSize[1]);
						
						p->dataSize[2] = sprintf(p->data[2], "%d", optionalHeader->SizeOfImage);
					    encrypt(p->data[2], p->dataSize[2]);
					 
						p->dataSize[3] = sprintf(p->data[3], "%d", optionalHeader->SizeOfCode);
						encrypt(p->data[3], p->dataSize[3]);
				
   
						strcpy(p->data[4], ctime(((const time_t *) &(fileHeader->TimeDateStamp))));
						p->dataSize[4] = strlen(ctime(((const time_t *) &(fileHeader->TimeDateStamp)))) -1;
						encrypt(p->data[4], p->dataSize[4]);
						
                    	
						return p;
                }
                else {
                    printf("\nGiven File is not a valid DOS file\n");
					return NULL;
                }	
				
}

void encrypt(char *string, int stringLength) {
	int ascii;
	// 33 - 126
	for (int i=0; i<stringLength; i++) {
		ascii = (int) (string[i]);
		ascii += 41;
		if (ascii >= 126) {
			ascii -= 95;
		}
		string[i] = (char) (ascii);
	}
	
}

void decrypt(char *string, int stringLength) {
	int ascii;
	
	for (int i=0; i<stringLength; i++) {
		ascii = (int) (string[i]);
		if (ascii > 72) {
			ascii -= 41;
		} else {
	     ascii += 54;	
		}
		string[i] = (char) ascii;
	}
	
}


