/*#include <SFML/Graphics.hpp>

void main()
{
	sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
}*/


#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define SERVER_PORT 55000
const sf::IpAddress SERVER_IP = sf::IpAddress(127, 0, 0, 1);

enum packetType { HANDSHAKE, LOGIN, MOVE };

sf::Packet& operator>>(sf::Packet& packet, packetType& type)
{
	int typeInt;
	packet >> typeInt;
	type = static_cast<packetType>(typeInt);
	return packet;
}

void HandleHandshake(sf::Packet& data)
{
	std::string message;
	data >> message;

	std::cout << "Handshake message: " << message << std::endl;
}

void HandleLogin(sf::Packet& data)
{
	std::string usuari, contrasenya;
	data >> usuari >> contrasenya;

	std::cout << "Login received - Usuari: " << usuari
		<< ", Contrasenya: " << contrasenya << std::endl;
}

int main()
{
	sf::TcpSocket socket;
	if (socket.connect(SERVER_IP, SERVER_PORT) != sf::Socket::Status::Done)
	{
		std::cout << "Connection failed" << std::endl;
		return 1;
	}

	std::cout << "Connected to server" << std::endl;
	socket.setBlocking(false);

	sf::Packet packet;

	bool gameOver = false;
	while (!gameOver)
	{
		if (socket.receive(packet) == sf::Socket::Status::Done)
		{
			packetType type;
			packet >> type;

			switch (type)
			{
			case HANDSHAKE:
				HandleHandshake(packet);
				break;

			case LOGIN:
				HandleLogin(packet);
				break;

			case MOVE:
				std::cout << "MOVE packet received" << std::endl;
				break;

			default:
				std::cout << "Unknown packet type" << std::endl;
				break;
			}

			packet.clear();
		}
		if (socket.receive(packet) == sf::Socket::Status::Disconnected)
		{
			gameOver = true;
		}
	}
}