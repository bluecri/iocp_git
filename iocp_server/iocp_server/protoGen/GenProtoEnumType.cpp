#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cstring>
#include <queue>
#include <tchar.h>
#include <stdio.h>
#include <fstream>

#include <SDKDDKVer.h>

#include <WinSock2.h>
#include <Mswsock.h>

#include "windows.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <tchar.h>

#include <algorithm>
#include <string.h>
#include <vector>
#include <deque>
#include <queue>
#include <map>

#include <process.h>
#include <assert.h>
#include <limits.h>

#include <cstdint>
#include <memory>

#include <type_traits>

using namespace std;

#define MAX_STR_SIZE 200
class SyncExecutable : public std::enable_shared_from_this<SyncExecutable>
{
public:
	SyncExecutable(int in) : _in(in) {};
	int _in;

	template <class T>
	std::shared_ptr<T> GetSharedFromThis()
	{
		static_assert(true == std::is_convertible<T, SyncExecutable>::value, "GetSharedFromThis T should be derived from SyncExecutable");

		// this 포인터를 std::shared_ptr<T>형태로 반환.
		//(HINT: 이 클래스는 std::enable_shared_from_this에서 상속받았다. 그리고 static_pointer_cast 사용)
		//return std::shared_ptr<T>((Player*)this); ///< X X X
		return std::static_pointer_cast<T>(shared_from_this());
	}

};
class Player : public SyncExecutable
{
public:
	Player(int i, int in) : _i(i), SyncExecutable(in) {};
	int _i;
	std::shared_ptr<Player> func() {
		return  std::static_pointer_cast<Player>(GetSharedFromThis<Player>());
	}
};
int main(int argc, char* argv[])
{
	std::shared_ptr<Player> sharedPlayer = nullptr;
	sharedPlayer = make_shared<Player>(10, 20);
	sharedPlayer->GetSharedFromThis<Player>();
	std::shared_ptr<Player> sharedPlayer22 = sharedPlayer->func();
	sharedPlayer22->func();
	cout << sharedPlayer22->_i << endl;

	static_assert(true == std::is_convertible<Player, SyncExecutable>::value, "T should be derived from SyncExecutable");
	cout << std::is_convertible<Player, SyncExecutable>::value << endl;

	/*
	vector<string> classTextVec;
	char * protoFileName = "protoPack.proto";

	char inputString[MAX_STR_SIZE];
	char* messageCharStart = nullptr;
	int textCharStartIdx = 0;
	int textCharEndIdx = 0;
		
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
			classTextVec.push_back(string(&messageCharStart[textCharStartIdx], (size_t)(textCharEndIdx - textCharStartIdx)));
		}
	}
	inFile.close();


	for (auto str : classTextVec) {
		cout << str << endl;
	}

	// create protoTypeEnum.h

	ofstream outFile("./GetSource/protoTypeEnum.h");
	outFile << "enum PACKET_TYPE" << endl;
	outFile << "{" << endl;
	for (int i = 0; i < classTextVec.size() - 1; i++) {
		outFile << "	PACKET_TYPE_" << classTextVec[i] << "," << endl;
	}
	outFile << "	PACKET_TYPE_" << classTextVec[classTextVec.size() - 1] << endl;
	outFile << "};" << endl;

	outFile.close();

	// --------  ----------

	// create PacketRecvHadler.h
	ofstream outFilePacketRecvHadlerHeader("./GetSource/PacketRecvHadler.h");

	outFilePacketRecvHadlerHeader << "#pragma once" << endl;
	outFilePacketRecvHadlerHeader << "#include \"protoPack.pb.h\"" << endl;
	outFilePacketRecvHadlerHeader << "class ClientSession;" << endl;
	outFilePacketRecvHadlerHeader << "class PacketHeader;" << endl;

	outFilePacketRecvHadlerHeader << "bool PacketRecvToMsg(ClientSession* session, PacketHeader& header, google::protobuf::io::CodedInputStream& codeInputStream);" << endl;
	for (int i = 0; i < classTextVec.size(); i++) {
		outFilePacketRecvHadlerHeader << "bool PacketRecvMsgHandle(ClientSession * session, prop::" << classTextVec[i] << " *" << classTextVec[i] << ");" << endl;		//msgLobby *msgLobby);
	}
	outFilePacketRecvHadlerHeader.close();


	// create PacketRecvHandler.cpp
	ofstream outFilePacketRecvHadlerCpp("./GetSource/PacketRecvHadler__copy.cpp");

	outFilePacketRecvHadlerCpp << "bool PacketRecvToMsg(ClientSession * session, PacketHeader & header, google::protobuf::io::CodedInputStream & codeInputStream)" << endl;
	outFilePacketRecvHadlerCpp << "{" << endl;
	outFilePacketRecvHadlerCpp << "\tswitch (header.GetType())" << endl;
	outFilePacketRecvHadlerCpp << "\t{"<< endl;

	for (int i = 0; i < classTextVec.size(); i++) {
		outFilePacketRecvHadlerCpp << "\tcase PACKET_TYPE::PACKET_TYPE_" << classTextVec[i] << ":" << endl;
		outFilePacketRecvHadlerCpp << "\t{" << endl;
		outFilePacketRecvHadlerCpp << "\t\tprop::" << classTextVec[i] << " *" << classTextVec[i] << " = new prop::" << classTextVec[i] << "();" << endl;
		outFilePacketRecvHadlerCpp << "\t\tif (false == " << classTextVec[i] << "->ParseFromCodedStream(&codeInputStream))" << endl;
		outFilePacketRecvHadlerCpp << "\t\t{" << endl;
		outFilePacketRecvHadlerCpp << "\t\t\tprintf_s(\"[ERROR] : PacketRecvHandle PACKET_TYPE_" << classTextVec[i] << " error\\n\");" << endl;
		outFilePacketRecvHadlerCpp << "\t\t\treturn false;" << endl;
		outFilePacketRecvHadlerCpp << "\t\t}" << endl;
		outFilePacketRecvHadlerCpp << "\t\tPacketRecvMsgHandle(session, " << classTextVec[i] << ");" << endl;
		outFilePacketRecvHadlerCpp << "\t}" << endl;
		outFilePacketRecvHadlerCpp << "\t\tbreak;" << endl << endl;
	}

	outFilePacketRecvHadlerCpp <<"\tdefault:" << endl;
	outFilePacketRecvHadlerCpp <<"\t\tprintf_s(\"[ERROR] : PacketRecvHandle default error\\n\");" << endl;
	outFilePacketRecvHadlerCpp <<"\t\treturn false;" << endl;
	outFilePacketRecvHadlerCpp << "\t}" << endl;
	outFilePacketRecvHadlerCpp << "return true;" << endl;
	outFilePacketRecvHadlerCpp << "}" << endl;


	for (int i = 0; i < classTextVec.size(); i++) {
		outFilePacketRecvHadlerCpp << "bool PacketRecvMsgHandle(ClientSession * session, prop::" << classTextVec[i] << " *" << classTextVec[i] << ")" << endl;		//msgLobby *msgLobby);
		outFilePacketRecvHadlerCpp << "{" << endl;
		outFilePacketRecvHadlerCpp << "\treturn true;" << endl;
		outFilePacketRecvHadlerCpp << "}" << endl;

	}
	outFilePacketRecvHadlerCpp.close();
	*/
	return 0;
}