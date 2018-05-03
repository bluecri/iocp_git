#pragma once

class Packet 
{
public:

public:
	char* GetPacketStart();
	int GetSize();

private:
	// client <> server with protobuf
	char * packet;
	int packetLen;
};