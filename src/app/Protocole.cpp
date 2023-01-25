#include "Protocole.hpp"
#include "Serialization.hpp"
#include <iostream>

void PlayerNamePacket::Serialize(std::vector<std::uint8_t>& byteArray) const
{
	Serialize_str(byteArray, name);
}

PlayerNamePacket PlayerNamePacket::Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	PlayerNamePacket packet;
	packet.name = Unserialize_str(byteArray, offset);
	return packet;
}

void playerNumberPacket::Serialize(std::vector<std::uint8_t>& byteArray) const
{
	Serialize_i32(byteArray, joueurNumber);
}

playerNumberPacket playerNumberPacket::Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	playerNumberPacket packet;
	packet.joueurNumber = Unserialize_i32(byteArray, offset);
	return packet;
}
