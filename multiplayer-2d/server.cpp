#include "server.h"


Server::Server() {
	socket_ = new sf::UdpSocket();
	if (socket_->bind(5555) != sf::Socket::Done) {
		std::cout << "Failed to bind socket!\n";
	}
	socket_->setBlocking(true);

	server_running_ = false;
	running_network_id_ = 1;
}

Server::~Server() {
	delete socket_;
}

void Server::Init() {
	std::cout << "Server: Initialized.\n";
	OpenConnection();
}

void Server::Close() {
	server_running_ = false;
}

void Server::OpenConnection() {
	server_running_ = true;

	MSPacket received_packet;
	sf::IpAddress sender_address;
	unsigned short sender_port;

	std::cout << "Server: Looking for new connections.\n";
	while (server_running_) {
		if (socket_->receive(received_packet, sender_address, sender_port) == sf::Socket::Done) {
			if (connections_.find(std::pair<sf::IpAddress, unsigned short>(sender_address, sender_port)) != connections_.end()) {
				ReceiveData(received_packet, sender_address, sender_port);
			}
			else {
				std::cout << "Server: Found new connection.\n";
				NewConnection(received_packet, sender_address, sender_port);
			}
		}
		UpdateClients();
	}
}

void Server::NewConnection(MSPacket received_packet, sf::IpAddress sender_address, unsigned short sender_port) {
	int network_id = GenerateNetworkID();
	connections_[std::pair<sf::IpAddress, unsigned short>(sender_address, sender_port)] = network_id;
	MSPacket packet;
	InitData init_data = { network_id };
	std::cout << "Sending network id: " << init_data.network_id << "\n";
	packet << init_data;

	socket_->send(packet, sender_address, sender_port);

	std::cout << "New connection from address: " << sender_address << ":" << sender_port << ", assigned with NetworkID: " << network_id << "\n";
}

int Server::GenerateNetworkID() {
	int return_id = running_network_id_;
	running_network_id_++;
	return return_id;
}

void Server::ReceiveData(MSPacket received_packet, sf::IpAddress sender_address, unsigned short sender_port) {
	int network_id = connections_[std::pair<sf::IpAddress, unsigned short>(sender_address, sender_port)];
	Data received_data;
	received_packet >> received_data;

	if (received_data.positions.size() > 0) {
		// Save received data in server_data_
		for (auto pair : received_data.positions) {
			server_data_.positions[pair.first] = pair.second;
		}
	}
}

void Server::UpdateClients() {
	MSPacket packet;
	packet << server_data_;
	Broadcast(packet);
}

void Server::Broadcast(MSPacket packet) {
	for (auto connection : connections_) {
		socket_->send(packet, connection.first.first, connection.first.second);
	}
}