#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;



char* encrypt(char *string, int stringLength) {
	int ascii;
	// 32 - 126
	for (int i=0; i<stringLength; i++) {
		ascii = (int) (string[i]);
		ascii += 41;
		if (ascii >= 126) {
			ascii -= 95;
		}
		string[i] = (char) (ascii);
	}
	return string;
}

char* decrypt(char *string, int stringLength) {
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
	return string;
}


int main() {
	std::string str;
	char *encrypted, *decrypted;
    cout << "Enter a string" << endl;
	
    std::getline (std::cin,str);
	printf("%d\n", str.length());
    char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	printf("original str: %s\n", cstr);
    encrypted = encrypt(cstr, strlen(cstr));
	printf("encrypted str: %s\n", encrypted);
	decrypted = decrypt(cstr, strlen(encrypted));
	printf("decrypted str: %s\n", decrypted);
	
	return 0;
}