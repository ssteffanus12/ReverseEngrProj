#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace std;

int main() {
  int number=33;
  stringstream strs;
  strs << number;
  string temp_str = strs.str();
  char* char_type = (char*) temp_str.c_str();
  printf("length: %d\n", strlen(char_type));
  printf("str: %s\n", char_type);
}