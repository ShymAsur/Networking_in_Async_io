#include <iostream>
#include <olc_net.h>
enum class customMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer,
	Value
};

int main() {

	olc::net::message<customMsgTypes> msg;
	msg.header.id = customMsgTypes::FireBullet;
	

	return 0;
}   