all: server client

server: 	server.o
	gcc -o server server.o

server.o:	server.c
	gcc -c server.c

client: 	client.o analysis_command_line.o get_filename.o
	gcc -o client client.o analysis_command_line.o get_filename.o

client.o: 	client.c client.h get_filename.h
	gcc -c client.c 

analysis_command_line.o: analysis_command_line.c analysis_command_line.h
	gcc -c analysis_command_line.c


get_filename.o: get_filename.c get_filename.h
	gcc -c get_filename.c

clean:
	rm *.o server client
