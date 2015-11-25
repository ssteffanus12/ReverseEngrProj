#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <time.h>

using namespace std;



protocolPacket* analyzePE(char **argv) {
        protocolPacket *p = (protocolPacket *)malloc(sizeof(protocolPacket));
	    p->msg_id = 0;
		p->msg_number = 0;
		
		//stringstream strs;
		//string temp_str;
		//char* char_type;
        
		HANDLE hMapObject,hFile;                //File Mapping Object
        LPVOID lpBase;                          //Pointer to the base memory of mapped file
        PIMAGE_DOS_HEADER dosHeader;            //Pointer to DOS Header
		PIMAGE_NT_HEADERS ntHeader;             //Represents the PE header format
        PIMAGE_FILE_HEADER fileHeader;          //Represents the COFF header format
		PIMAGE_OPTIONAL_HEADER optionalHeader;  //Represents the optional header format
		
		// if valid dos file :
               
                //Open the Exe File
                hFile = CreateFile(argv[3],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
                if(hFile == INVALID_HANDLE_VALUE){printf("\nERROR : Could not open the file specified\n"); }
               
                //Mapping Given EXE file to Memory
                hMapObject = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
                lpBase = MapViewOfFile(hMapObject,FILE_MAP_READ,0,0,0);
               
                //Get the DOS Header Base
                dosHeader = (PIMAGE_DOS_HEADER)lpBase;// 0x04000000
				ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)dosHeader + dosHeader->e_lfanew);
				fileHeader = (PIMAGE_FILE_HEADER)&ntHeader->FileHeader;
               // sectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)ntHeader + sizeof(IMAGE_NT_HEADERS) + (fileHeader->NumberOfSections - 1) * sizeof(IMAGE_SECTION_HEADER));
                optionalHeader = (PIMAGE_OPTIONAL_HEADER)&ntHeader->OptionalHeader;
				
			   //Check for Valid DOS file
                if(dosHeader->e_magic == IMAGE_DOS_SIGNATURE){
                        //Dump the Dos Header info
                        printf("\nValid Dos Exe File\n------------------\n");
                        printf("\nDumping DOS Header Info....\n---------------------------");      
                        printf("\n%-36s%d\n","Number of sections : ", optionalHeader->SizeOfInitializedData); 
						p->dataSize[0] = sprintf(p->data[0], "%d", optionalHeader->SizeOfInitializedData); 
					
				
						printf("\n%-36s%d\n","Number of symbols : ", fileHeader->NumberOfSymbols); 
						p->dataSize[1] = sprintf(p->data[1], "%d", fileHeader->NumberOfSymbols); 
						
						
						printf("\n%-36s%d\n","Size of optional header : ", optionalHeader->SizeOfImage); 
						p->dataSize[2] = sprintf(p->data[2], "%d", optionalHeader->SizeOfImage);
					
						printf("\n%-36s%d\n","Size of code : ", optionalHeader->SizeOfCode); 
						p->dataSize[3] = sprintf(p->data[3], "%d", optionalHeader->SizeOfCode);
						
				
                        printf("\n%-36s%s\n","Time Stamp :",ctime(((const time_t *) &(fileHeader->TimeDateStamp)))); 
						
			
						strcpy(p->data[4], ctime(((const time_t *) &(fileHeader->TimeDateStamp))));
						p->dataSize[4] = strlen(ctime(((const time_t *) &(fileHeader->TimeDateStamp))));
						
                    	printf("\n===============================================================================\n");
						return p;
                }
                else {
                    printf("\nGiven File is not a valid DOS file\n");
					return NULL;
                }	
				
}


