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

void playerPositionPacket::Serialize(std::vector<std::uint8_t>& byteArray) const
{
	Serialize_i32(byteArray, playerNumber);
	Serialize_f32(byteArray, playerPosX);
	Serialize_f32(byteArray, playerPosY);
	Serialize_i32(byteArray, playerNumber);
}

playerPositionPacket playerPositionPacket::Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	playerPositionPacket packet;
	packet.playerNumber = Unserialize_i32(byteArray, offset);
	packet.playerPosX = Unserialize_f32(byteArray, offset);
	packet.playerPosY = Unserialize_f32(byteArray, offset);
	packet.inputIndex = Unserialize_i32(byteArray, offset);

	return packet;
}

void PlayerInputPacket::Serialize(std::vector<std::uint8_t>& byteArray) const
{
	std::int32_t value = 0;
	value |= (isUp) ? 1 << 0 : 0;
	value |= (isDown) ? 1 << 1 : 0;
	Serialize_i32(byteArray, value);
}

PlayerInputPacket PlayerInputPacket::Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset)
{
	std::uint8_t value = Unserialize_i32(byteArray, offset);

	PlayerInputPacket packet;

	packet.isUp = (value & (1 << 0));
	packet.isDown = (value & (1 << 1));

	return packet;
}
