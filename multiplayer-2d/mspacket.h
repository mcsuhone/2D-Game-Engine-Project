#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include "msvector.h"

/* Override << and >> for different container classes that are defined below. */
class MSPacket : public sf::Packet {
public:
	MSPacket() = default;
	~MSPacket() = default;
private:

};

/* Define here different container classes / structures. */

struct InitData {
	int network_id;
};

/* Contains all location data needed. Idle/running animation are parsed from movement. */
struct Data {
	std::map<int, MSVector> positions;
};

/* InitData overloads*/
inline sf::Packet& operator <<(MSPacket& packet, const InitData data) {
	return packet << data.network_id;
}
inline sf::Packet& operator >>(MSPacket& packet, InitData data) {
	return packet >> data.network_id;
}

/* Data overloads*/
inline sf::Packet& operator <<(MSPacket& packet, const Data data) {
	packet << data.positions.size();
	for (auto pair : data.positions) {
		packet << pair.first << pair.second.x << pair.second.y;
	}
	return packet;
}
inline sf::Packet& operator >>(MSPacket& packet, Data data) {
	int id = 0, size = 0;
	float x = 0, y = 0;
	packet >> size;
	for (int i = 0; i < size; i++) {
		packet >> id  >> x >> y;
		data.positions[id] = MSVector(x, y);
	}
	return packet;
}