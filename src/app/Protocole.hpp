#pragma once
#include <enet/enet.h>
#include <cstdint>
#include <string>
#include <vector>

enum class Opcode : std::int32_t
{
	C_PlayerName = 0,
	C_PlayerMovement,
	S_WorldInit,
	S_PlayerList,
	S_PlayersPosition
};

// Un joueur souhaite renseigner son nom
struct PlayerNamePacket
{
	static constexpr Opcode opcode = Opcode::C_PlayerName;

	std::string name;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static PlayerNamePacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

// Petite fonction d'aide pour construire un packet ENet à partir d'une de nos structures de packet, insère automatiquement l'opcode au début des données
template<typename T> ENetPacket* build_packet(const T& packet, uint32_t flags)
{
	// On sérialise l'opcode puis le contenu du packet dans un std::vector<std::uint8_t>
	std::vector<std::uint8_t> byteArray;

	Serialize_i32(byteArray, static_cast<std::int32_t>(T::opcode));
	packet.Serialize(byteArray);

	// On copie le contenu de ce vector dans un packet enet, et on l'envoie au peer
	return enet_packet_create(byteArray.data(), byteArray.size(), flags);
}