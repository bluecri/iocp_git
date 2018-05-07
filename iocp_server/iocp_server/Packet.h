#pragma once

struct PacketHeader
{
	DWORD _packetSize;
	DWORD _packetPriority;
	DWORD _packetType;
};

struct Packet 
{
public:
	PacketHeader header;
public:
	char* GetPacketStart();
	int GetSize();

private:
	// client <> server with protobuf
	char * packet;
	int packetLen;
};