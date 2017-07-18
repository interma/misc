/*
 desc:
 author: interma (interma@outlook.com)
 created: 12/05/16 10:38:23 CST
*/

#include <iostream>
#include <cstdio>
#include <string>

std::string calculateIV(const std::string& initIV, unsigned long counter) {

  char IV[initIV.length()];

  int i = initIV.length(); // IV length
  int j = 0; // counter bytes index
  unsigned int sum = 0;
  while (i-- > 0) {
    // (sum >>> Byte.SIZE) is the carry for addition
    sum = (initIV[i] & 0xff) + (sum >> 8);
    if (j++ < 8) { // Big-endian, and long is 8 bytes length
      sum += (char) counter & 0xff;
      counter >>= 8;
    }
    IV[i] = (char) sum;
  }

  return std::string(IV, initIV.length());
}

int main() {
	char iv[] = "1234567890123456";
	const char* str = "test";

	std::string initIV (iv);
	std::string result;

	for (int i = 0; i < 16; i++) {
		result = calculateIV(initIV, i);
		std::cout<<result.length()<<std::endl;
		std::cout<<result<<std::endl;
	}



	return 0;
}
