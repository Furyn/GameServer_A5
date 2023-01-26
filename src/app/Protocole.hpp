#pragma once
#include <enet/enet.h>
#include <cstdint>
#include <string>
#include <vector>

enum class Opcode : std::int32_t
{
	C_PlayerName = 0,
	S_PlayerNumber = 1,
	C_PlayerInput = 2,
	S_PlayerPosition = 3,
	S_BallPosition = 4,
};

// Un joueur souhaite renseigner son nom
struct PlayerNamePacket
{
	static constexpr Opcode opcode = Opcode::C_PlayerName;

	std::string name;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static PlayerNamePacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

// On dit au joueur si il est J1 ou J2
struct playerNumberPacket
{
	static constexpr Opcode opcode = Opcode::S_PlayerNumber;

	int32_t joueurNumber;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static playerNumberPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

struct PlayerInputPacket
{
	static constexpr Opcode opcode = Opcode::C_PlayerInput;

	bool isUp;
	bool isDown;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static PlayerInputPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

struct PlayerPositionPacket
{
	static constexpr Opcode opcode = Opcode::S_PlayerPosition;

	int32_t playerNumber;
	float playerPosX;
	float playerPosY;
	int32_t inputIndex;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static PlayerPositionPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
};

struct BallPositionPacket
{
	static constexpr Opcode opcode = Opcode::S_BallPosition;

	float ballPosX;
	float ballPosY;

	void Serialize(std::vector<std::uint8_t>& byteArray) const;
	static BallPositionPacket Unserialize(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
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