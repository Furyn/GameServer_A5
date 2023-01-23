#include <app/constants.hpp>
#include <utils/exit_capsule.hpp>
#include <enet/enet.h>
#include <math/quaternion.hpp>
#include <iostream>
#include <string>
#include "Serialization.hpp"

struct ServerData
{
	ENetHost* host;
};

void tick(ServerData& serverData);

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

	serverData.host = enet_host_create(&address, 10, 10, 0, 0, 0);
	if (!serverData.host)
	{
		std::cout << "Failed to create ENet host (is port free?)" << std::endl;
		return EXIT_FAILURE;
	}

	// On planifie la destruction du host ENet à la fin de la fonction
	ExitCapsule freeHost([&] { enet_host_destroy(serverData.host); });

	std::uint32_t nextTick = enet_time_get();

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
					case ENetEventType::ENET_EVENT_TYPE_CONNECT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " connected!" << std::endl;
						break;

					// Un joueur s'est déconnecté
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " disconnected!" << std::endl;
						break;

					// Un joueur a été déconnecté par timeout
					case ENetEventType::ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " disconnected (timed out)!" << std::endl;
						break;

					// On a reçu des données d'un joueur
					case ENetEventType::ENET_EVENT_TYPE_RECEIVE:
						std::size_t offset = 0;

						std::vector<std::uint8_t> byteArray;
						byteArray.resize(enet_packet_get_length(event.packet));
						std::memcpy(&byteArray[0], event.packet->data, enet_packet_get_length(event.packet));

						std::cout << byteArray.size() << std::endl;

						int i = Unserialize_i32(byteArray, offset);
						std::string str = Unserialize_str(byteArray, offset);

						std::cout << i << " " << str << std::endl;

						std::cout << "Peer #" << enet_peer_get_id(event.peer) << " sent data (" << enet_packet_get_length(event.packet) << " bytes)" << std::endl;
						enet_packet_dispose(event.packet);
						break;
				}
			}
			while (enet_host_check_events(serverData.host, &event) > 0);
		}

		// On déclenche un tick si suffisamment de temps s'est écoulé
		if (now >= nextTick)
		{
			tick(serverData);
			nextTick += TickDelay;
		}
	}

	return EXIT_SUCCESS;
}

void tick(ServerData& serverData)
{
}
