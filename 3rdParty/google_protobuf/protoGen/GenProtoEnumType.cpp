#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cstring>
#include <queue>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
using namespace std;

#define MAX_STR_SIZE 200

int main(int argc, char* argv[])
{
	//read until 'message'
	//loop
		//make PACKET_TYPE_Text
		//read { } stk
		//find message
	//loop end

	vector<string> textVec;

	char inputString[MAX_STR_SIZE];
	char* messageCharStart = nullptr;
	int textCharStartIdx = 0;
	int textCharEndIdx = 0;

	char * protoFileName = "protoPack.proto";
	//char * protoFileName;
	//protoFileName = new char[strlen(argv[1])+1];
	//strcpy_s(protoFileName, strlen(argv[1]) + 1, static_cast<const char*>(argv[1]));
	
	ifstream inFile(protoFileName);
	while (!inFile.eof()) {
		inFile.getline(inputString, MAX_STR_SIZE);
		textCharStartIdx = 0;
		textCharEndIdx = 0;

		if (strlen(inputString) == 0)
		{
			continue;
		}

		if (nullptr != (messageCharStart = strstr(inputString, "message")))
		{
			textCharStartIdx += 7;
			while (messageCharStart[textCharStartIdx] == ' ') {
				textCharStartIdx++;		//start text index
			}
			textCharEndIdx = textCharStartIdx;
			while (!(messageCharStart[textCharEndIdx] == ' ' || messageCharStart[textCharEndIdx] == '{' || messageCharStart[textCharEndIdx] == '\0')) {
				textCharEndIdx++;	//empty text index
			}
			cout << &messageCharStart[textCharStartIdx] << endl;
			textVec.push_back(string(&messageCharStart[textCharStartIdx], (size_t)(textCharEndIdx - textCharStartIdx)));
		}
	}

	for (auto str : textVec) {
		cout << str << endl;
	}

	ofstream outFile("./GetSource/protoTypeEnum.h");
	outFile << "enum PACKET_TYPE" << endl;
	outFile << "{" << endl;
	for (int i = 0; i < textVec.size() - 1; i++) {
		outFile << "	PACKET_TYPE_" << textVec[i] << "," << endl;
	}
	outFile << "	PACKET_TYPE_" << textVec[textVec.size() - 1] << endl;
	outFile << "};" << endl;

	inFile.close();
	outFile.close();

	return 0;
}