#include "client.h"



Client::Client() {
	socket_ = new sf::UdpSocket();
	if (socket_->bind(sf::Socket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind socket!\n";
	}
	socket_->setBlocking(true);

	network_id_ = -1;
	server_address_ = std::pair<sf::IpAddress, unsigned short>("", 0);
}

Client::~Client() {
	delete socket_;
}

bool Client::Connect(sf::IpAddress ip, unsigned short port) {
	server_address_ = std::pair<sf::IpAddress, unsigned short>(ip, port);
	MSPacket packet;
	InitData init_data{ network_id_ };
	packet << init_data;

	std::cout << "Connecting to server with address " << ip << ":" << port << " ...\n";

	/* Send any packet to server. */
	SendData(packet);
	/* Wait for server to response with network id. */
	MSPacket received_packet;
	InitData received_data{ 0 };

	ReceiveData(&received_packet);
	received_packet >> received_data;
	
	/* Register received ip address*/
	network_id_ = received_data.network_id;
	socket_->setBlocking(false);
	std::cout << "Connected with network id: " << network_id_ << "\n";
	return true;
}

void Client::SendGameData(Game* game) {
	World* world = game->GetWorld();
	MSPacket packet;
	Data data;
	data.positions[network_id_] = world->GetPlayer(network_id_)->GetPosition();
	packet << data;
	SendData(packet);
}

void Client::ReceiveGameData(Game* game) {
	MSPacket packet;
	ReceiveData(&packet);
	Data received_data;
	packet >> received_data;
	/* Handle positional data. */
	std::map<int, Player*> players = game->GetWorld()->GetPlayers();

	for (auto pair : received_data.positions) {
		/* See if player is already found. */
		if (players.find(pair.first) != players.end()) {
			players[pair.first]->SetPosition(pair.second);
		}
		else {
			game->GetWorld()->AddPlayer(pair.first, pair.second);
		}
	}
}

void Client::SendData(MSPacket packet) {
	socket_->send(packet, "192.168.0.102", server_address_.second);
}

void Client::ReceiveData(MSPacket* packet) {
	MSPacket new_packet;
	socket_->receive(new_packet, server_address_.first, server_address_.second);
	*packet = new_packet;
}