// protocol.h

typedef struct {
char msg_id;
char msg_number;
char data[5][1024];
} protocolPacket;
