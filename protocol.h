// protocol.h

typedef struct {
char msg_id;
char data[5][1024];
int dataSize[5];
} protocolPacket;
