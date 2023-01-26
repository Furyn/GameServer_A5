#include <app/constants.hpp>
#include <utils/exit_capsule.hpp>
#include <enet/enet.h>
#include <math/quaternion.hpp>
#include <iostream>
#include <string>
#include "Serialization.hpp"
#include "Protocole.hpp"
#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "Player.hpp"
#include "Ball.hpp"

struct ServerData
{
	ENetHost* host;
	Player player1;
	Player player2;
	Ball ball;
	bool gameStarted = false;
	int nbPlayer = 0;
};

void game_tick(ServerData& serverData);
void network_tick(ServerData& serverData);
void handle_message(const std::vector<std::uint8_t>& message, ENetEvent event);

int main()
{
	// Initialisation d'enet
	if (enet_initialize() != 0)
	{
		std::cerr << "failed to initialize enet" << std::endl;
		return EXIT_FAILURE;
	}

	// On planifie la libération d'enet à la fin de la fonction
	ExitCapsule uninitEnet([&] { enet_deinitialize(); });

	ENetAddress address;
	enet_address_set_ip(&address, "::");

	address.port = AppPort;

	ServerData serverData;

	serverData.player1.position = { -2 , 0 };
	serverData.player1.playerNumber = 1;
	serverData.player2.position = { 16 , 0 };
	serverData.player2.playerNumber = 2;
	serverData.ball.position = { 7 , 0 };

	serverData.host = enet_host_create(&address, 10, 10, 0, 0, 0);
	if (!serverData.host)
	{
		std::cout << "Failed to create ENet host (is port free?)" << std::endl;
		return EXIT_FAILURE;
	}

	// On planifie la destruction du host ENet à la fin de la fonction
	ExitCapsule freeHost([&] { enet_host_destroy(serverData.host); });

	std::uint32_t nextGameTick = enet_time_get();
	std::uint32_t nextNetworkTick = enet_time_get();

	// Boucle principale
	for (;;)
	{
		// On récupère le temps actuel
		std::uint32_t now = enet_time_get();

		// On gère les événements ENet
		ENetEvent event;
		while (enet_host_service(serverData.host, &event, 1) > 0)
		{
			do
			{
				switch (event.type)
				{
					// Un nouveau joueur s'est connecté
					case ENetEventType::ENET_EVENT_TYPE_CONNECT: {
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " connected!" << std::endl;

						if (enet_peer_get_id(event.peer) > 1) {
							enet_peer_disconnect_now(event.peer, 0);
							break;
						}

						serverData.nbPlayer++;

						if (serverData.nbPlayer == 1) {
							serverData.gameStarted = true;
						}

						playerNumberPacket playerNumberPacket;
						playerNumberPacket.joueurNumber = enet_peer_get_id(event.peer) + 1;

						ENetPacket* packet = build_packet(playerNumberPacket, ENET_PACKET_FLAG_RELIABLE);
						enet_peer_send(event.peer, 0, packet);

						enet_packet_dispose(packet);
						break;
					}
					// Un joueur s'est déconnecté
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " disconnected!" << std::endl;
						serverData.nbPlayer--;
						break;

					// Un joueur a été déconnecté par timeout
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " disconnected (timed out)!" << std::endl;
						serverData.nbPlayer--;
						break;

					// On a reçu des données d'un joueur
					case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
						std::vector<std::uint8_t> content(enet_packet_get_length(event.packet));
						std::memcpy(&content[0], event.packet->data, enet_packet_get_length(event.packet));

						handle_message(content, event);

						enet_packet_dispose(event.packet);
						break;
				}
			}
			while (enet_host_check_events(serverData.host, &event) > 0);
		}

		// On vérifie si assez de temps s'est écoulé pour faire avancer la logique du jeu
		if (now >= nextGameTick)
		{
			// On met à jour la logique du jeu
			game_tick(serverData);

			// On prévoit la prochaine mise à jour
			nextGameTick += gameTickInterval;
		}

		// Même chose pour le réseau
		if (now >= nextNetworkTick)
		{
			network_tick(serverData);

			nextNetworkTick += networkTickInterval;
		}
	}

	return EXIT_SUCCESS;
}

void game_tick(ServerData& serverData)
{
	if (!serverData.gameStarted) {
		return;
	}
	serverData.player1.UpdatePhysics(gameTickInterval / TickRate);
	serverData.player2.UpdatePhysics(gameTickInterval / TickRate);
	serverData.ball.UpdatePhysics(serverData.player1, serverData.player2, gameTickInterval / TickRate);
}

void network_tick(ServerData& serverData)
{

}

void handle_message(const std::vector<std::uint8_t>& message, ENetEvent event) {

	std::size_t offset = 0;
	Opcode opcode = static_cast<Opcode>(Unserialize_i32(message, offset));
	switch (opcode)
	{
		case Opcode::C_PlayerName: {
			PlayerNamePacket playerName = PlayerNamePacket::Unserialize(message, offset);
			std::cout << "Player #" << enet_peer_get_id(event.peer) << " Name : " << playerName.name << std::endl;
			break;
		}
		default: {

			break;
		}
	}
	

}
