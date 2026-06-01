SERVER = chat_server
CLIENT = chat_client

.PHONY: all clean client server

all: $(SERVER) $(CLIENT)

client: $(CLIENT)

server: $(SERVER)

clean:
	rm *.o

client_system_v.o: client_system_v.c
	gcc client_system_v.c -c -lrt -lpthread -lncurses

server_system_v.o: server_system_v.c
	gcc server_system_v.c -c -lncurses -lrt -lpthread

server_func.o: server_func.c
	gcc server_func.c -c -lncurses -lrt -lpthread

$(SERVER): server_system_v.o server_func.o message_system_v.h
	gcc server_system_v.o server_func.o -o $(SERVER) -lrt -lpthread -lncurses

$(CLIENT): client_system_v.o client_func.o message_system_v.h
	gcc client_system_v.o server_func.o -o $(CLIENT) -lrt -lpthread -lncurses