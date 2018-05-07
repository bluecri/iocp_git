#include "stdafx.h"
#include "Packet.h"
#include "../../3rdParty/google_protobuf/protoGen/GetSource/protoTypeEnum.h"

PacketHeader::PacketHeader(PACKET_TYPE packetType, DWORD packetSize, PACKET_PRIO _packetPriority)
{
}

PacketHeader::~PacketHeader()
{
}

Packet::Packet(PACKET_TYPE packetType, protobuf::Message&& msg, PACKET_PRIO _packetPriority)
	: _header(packetType, msg.ByteSizeLong() + sizeof(PacketHeader), _packetPriority), _msg(std::move(msg))
{
}

Packet::~Packet()
{
}

void Packet::SetPriority(PACKET_PRIO prio)
{
	_header._packetPriority = prio;
}

PACKET_PRIO Packet::GetPriority() const
{
	return _header._packetPriority;
}

DWORD Packet::GetSize() const
{
	return _header._packetSize;
}

PACKET_TYPE Packet::GetType() const
{
	return _header._packetType;
}

const protobuf::Message & Packet::GetMsg() const
{
	return _msg;
}

const PacketHeader & Packet::GetHeader() const
{
	return _header;
}

