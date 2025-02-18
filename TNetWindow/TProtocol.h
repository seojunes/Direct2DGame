#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE  4
#define PACKET_DATA_SIZE  1024
#define PACKET_MAX_PACKET_SIZE  (PACKET_DATA_SIZE+PACKET_HEADER_SIZE)
// 바이트 패킹 작업
#pragma pack(push, 1)
typedef struct
{
	WORD len;
	WORD type;
}PACKET_HEADER;
typedef struct
{
	PACKET_HEADER ph;
	char		  msg[PACKET_DATA_SIZE];
}UPACKET;

typedef struct
{
	int  id;
	char name[32];
}JOIN_USER;
typedef struct
{
	char name[32];
}USER_NAME;
#pragma pack(pop)

#define PACKET_CHAT_NAME_SC_REQ   100
#define PACKET_CHAT_NAME_CS_ACK   101
#define PACKET_CHAT_MSG  1000
#define PACKET_GAME_START 10000
#define PACKET_GAME_END   10001
#define PACKET_JOIN_USER   20000
#define PACKET_DRUP_USER   20001
#define PACKET_JOIN_REQ   30000
#define PACKET_JOIN_ACK   30001

