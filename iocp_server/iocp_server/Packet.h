#pragma once


class protobuf::Message;
class Packet;
enum PACKET_TYPE;

enum PACKET_PRIO
{
	MUST,
	CANREMOVE,
	REMOVE
};


class PacketHeader
{
public:
	PacketHeader(PACKET_TYPE packetType, DWORD packetSize, PACKET_PRIO _packetPriority = PACKET_PRIO::MUST);
	~PacketHeader();

private:
	DWORD _packetSize;
	PACKET_PRIO _packetPriority;
	PACKET_TYPE _packetType;

	friend class Packet;
};

class Packet
{
public:
	Packet(PACKET_TYPE packetType, protobuf::Message&& msg, PACKET_PRIO _packetPriority = PACKET_PRIO::MUST);
	~Packet();

	void SetPriority(PACKET_PRIO prio);

	PACKET_PRIO GetPriority() const;
	DWORD GetSize() const;
	PACKET_TYPE GetType() const;
	const protobuf::Message& GetMsg() const;
	const PacketHeader& GetHeader() const;

private:
	PacketHeader _header;
	protobuf::Message&& _msg;
};