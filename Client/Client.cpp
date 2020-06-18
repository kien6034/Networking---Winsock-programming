// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

const sf::Uint8 audioData = 1;

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	system("pause");
	int ret = connect(client, (SOCKADDR*)& addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		printf("connect() failed: error code %d\n", ret);
		return 1;
	}

	
	//record
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Recording sounds");
	sf::Event event;

	if (!sf::SoundBufferRecorder::isAvailable())
		printf("You need to install a mic");

	sf::SoundBufferRecorder recorder;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::R) {
					recorder.start(44100);
				}
				else if (event.key.code == sf::Keyboard::S) {
					recorder.stop();
					buffer = recorder.getBuffer();
					sound.setBuffer(buffer);
					sound.play();

					//get sample counts  
					const _int16 soundBuf = buffer.getSampleCount();
			
					//TODO: transform buffer type to char*
					const char* buf = "audio on user side recorded \n";

					//TODO: send char* buffer to the server
					send(client, buf, strlen(buf), 0);
				}
			}
			window.clear(sf::Color(0, 240, 255));
			window.display();
		}
	}

	// truyen du lieu : cho normal text, voi audio thi khong can thiet
	while (1)
	{
		char buf[256];

		scanf("%s", &buf);
		send(client, buf, strlen(buf), 0);
	}

	closesocket(client);
	WSACleanup();
}
