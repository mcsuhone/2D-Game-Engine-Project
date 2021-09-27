#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <SFML/Network.hpp>
#include "mspacket.h"

class Server {
public:
	Server();

	~Server();
	/* Starts the server, and looks for new connections. */
	void Init();
	void Close();
private:
	/* Non-threaded connection loop. */
	void OpenConnection();

	/* Initialise the new connection and send the client it's network_id. */
	void NewConnection(MSPacket received_packet, sf::IpAddress sender_address, unsigned short sender_port);
	/* Generate a unique network_id*/
	int GenerateNetworkID();
	/* Process data received from a client. */
	void ReceiveData(MSPacket received_packet, sf::IpAddress sender_address, unsigned short sender_port);
	/* Broadcast server_data_ to clients. */
	void UpdateClients();
	/* Send data to all connections. */
	void Broadcast(MSPacket packet);
	
	
	sf::UdpSocket* socket_;
	/* Holds connections, ip/port pair as a key and stores network_id. */
	std::map<std::pair<sf::IpAddress, unsigned short>, int> connections_;
	/* Holds all data saved by server, locations of different players etc. */
	Data server_data_;
	/* Defines if thread loop should keep running. Extends to vector<bool> if multiple threads are needed. */
	bool server_running_;
	int running_network_id_;
	
};