#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include "mspacket.h"
#include "game.h"

class Client {
public:
	Client();
	~Client();

	int GetNetworkId() { return network_id_; }
	/* Establish connection to server and get network_id. */
	bool Connect(sf::IpAddress ip, unsigned short port);
	/* Send local changes. */
	void SendGameData(Game* game);
	/* Receive server changes and update game. */
	void ReceiveGameData(Game* game);
private:
	void SendData(MSPacket packet);
	void ReceiveData(MSPacket* packet);
	int network_id_;
	sf::UdpSocket* socket_;
	std::pair<sf::IpAddress, unsigned short> server_address_;
};