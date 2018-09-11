#include <iostream>
#include <SFML\Network.hpp>

using namespace std;
using namespace sf;

//Few honest words from programmer
//Client side may get packet extraction errors in every 3-4 turns.
//I assume it is because of we are trying client to connect and server to listen in each turn which is obviously very bad example
//You can seperate the code below into Server And Client classes and get rid of that error.However i wanted to keep them under same roof
//Since it is very useful to learn for beginners
//Author: MrWormHole (Talha Altınel)

//utility
void convertStringToLowercase(string& s) {
	locale loc;
	for (string::size_type i = 0; i < s.length(); i++) {
		s[i] = tolower(s[i], loc);
	}
}

int main() {
	TcpSocket socket;
	IpAddress ip = IpAddress::getLocalAddress();
	char connectionType; //s For Server,c For Client.We only need 1 server 1 client for 2 players
	string characters[3] = { "rock", "paper" , "scissors" };
	string winConditions[3] = { "scissorspaper" , "paperrock","rockscissors" };
	string loseConditions[3] = { "rockpaper","paperscissors","scissorsrock" };
	string player1;
	string player2;
	int player1score = 0;
	int player2score = 0;
	int round = 1;
	Packet packet;

	cout << "Welcome to my rock paper scissors multiplayer game." << endl;
	cout << "This game only uses 1 server and 1 client" << endl;
	cout << "Enter(s) for server ,Enter(c) for client" << endl;
	cin >> connectionType;

	while (true) {
		if (connectionType == 's') {
			TcpListener listener;
			listener.listen(2000);
			cout << "\n Round " << round << endl << endl;
			cin >> player1;
			if (player1 == "q") { break; }
			convertStringToLowercase(player1);
			while (player1 != characters[0] && player1 != characters[1] && player1 != characters[2]) {
				cout << " Please pick one of these valid choices(rock,paper,scissors) to play it" << endl;
				cin >> player1;
				convertStringToLowercase(player1);
			}
			listener.accept(socket); //Waiting for connection from client
			packet << player1;
			socket.send(packet);
			cout << "\n Waiting for other player's choice ...\n\n" << endl;
			socket.receive(packet); //waiting for packet from client
			if (packet >> player2) {
				if (player1 == player2) {
					cout << "\n DRAW!" << endl;
					cout << " Player1(you): " << player1score << endl;
					cout << " Player2: " << player2score << endl;
				}
				else if (player1 + player2 == winConditions[0] || player1 + player2 == winConditions[1] || player1 + player2 == winConditions[2]) {
					cout << "\n YOU WON!" << endl;
					player1score++;
					cout << " Player1(you): " << player1score << endl;
					cout << " Player2: " << player2score << endl;
				}
				else if (player1 + player2 == loseConditions[0] || player1 + player2 == loseConditions[1] || player1 + player2 == loseConditions[2]) {
					cout << "\n YOU LOST!" << endl;
					player2score++;
					cout << " Player1(you): " << player1score << endl;
					cout << " Player2: " << player2score << endl;
				}
				else {
					cout << " Something went wrong with other player" << endl;
				}
				round++;
				packet.clear();

			}
			else {
				cout << " Packet extraction error.Please type again" << endl;
			}
		}
		else if (connectionType == 'c') {
			socket.connect(ip, 2000);
			cout << "\n Round " << round << endl << endl;
			cin >> player2;
			if (player2 == "q") { break; }
			convertStringToLowercase(player2);
			while (player2 != characters[0] && player2 != characters[1] && player2 != characters[2]) {
				cout << " Please pick one of these valid choices(rock,paper,scissors) to play it" << endl;
				cin >> player2;
				convertStringToLowercase(player2);
			}
			packet << player2;
			socket.send(packet);
			cout << "\n Waiting for other player's choice ...\n\n" << endl;
			socket.receive(packet); //waiting for packet from server
			if (packet >> player1) {
				if (player2 == player1) {
					cout << "\n DRAW!" << endl;
					cout << " Player1: " << player1score << endl;
					cout << " Player2(you): " << player2score << endl;
				}
				else if (player2 + player1 == winConditions[0] || player2 + player1 == winConditions[1] || player2 + player1 == winConditions[2]) {
					cout << "\n YOU WON!" << endl;
					player2score++;
					cout << " Player1: " << player1score << endl;
					cout << " Player2(you): " << player2score << endl;
				}
				else if (player2 + player1 == loseConditions[0] || player2 + player1 == loseConditions[1] || player2 + player1 == loseConditions[2]) {
					cout << "\n YOU LOST!" << endl;
					player1score++;
					cout << " Player1: " << player1score << endl;
					cout << " Player2(you): " << player2score << endl;
				}
				else {
					cout << " Something went wrong with other player" << endl;
				}
				round++;
				packet.clear();
			}
			else {
				cout << " Packet extraction error.Please type again" << endl;
			}
		}
	}

	return 0;
}
