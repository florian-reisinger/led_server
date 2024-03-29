#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>

#define LED1 18
#define LED2 23
#define LED3 24
#define LED4 25

#define BUFSIZE 1000
#define PORT 3000

#define QUEUE 3

void *mirror(void* arg)
{
	wiringPiSetupGpio();
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	
	digitalWrite(LED1, 0);
    digitalWrite(LED2, 0);
    digitalWrite(LED3, 0);
    digitalWrite(LED4, 0);
    
	int clientfd = *(int *)arg;
	char inbuffer[BUFSIZE];
	int decz = 0;
	
	write(clientfd, "Willkommen zum LED Server.\n\r", 27);
	write(clientfd, "\nGeben Sie eine Zahl zwischen 0 und 15 ein. Die LEDs geben die eingegeben Zahl im Binären Format wieder.\n\r", 107);
	write(clientfd, "Zum beenden q drücken.\n\n\r", 43);
	while(1){
		read(clientfd, inbuffer, sizeof(inbuffer));  
		
		if(inbuffer[0] == 'q'){
			write(clientfd, "Server wird heruntergefahren...\n\r", 12);
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
			break;
		};
			
		decz = atoi(inbuffer);
		
		if(decz >= 0 && decz <= 15){
			if(decz % 2 == 1){
				digitalWrite(LED1, 1);
				decz = (decz-1)/2;
				if(decz % 2 == 1){
					digitalWrite(LED2, 1);
					decz = (decz-1)/2;
					if(decz % 2 == 1){
						digitalWrite(LED3, 1);
						decz = (decz-1)/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					}else{
						digitalWrite(LED3, 0);
						decz = decz/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					};
				}else{
					digitalWrite(LED2, 0);
					decz = decz/2;
					if(decz % 2 == 1){
						digitalWrite(LED3, 1);
						decz = (decz-1)/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					}else{
						digitalWrite(LED3, 0);
						decz = decz/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					};
				};
			}else{
				digitalWrite(LED1, 0);
				decz = decz/2;
				if(decz % 2 == 1){
					digitalWrite(LED2, 1);
					decz = (decz-1)/2;
					if(decz % 2 == 1){
						digitalWrite(LED3, 1);
						decz = (decz-1)/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					}else{
						digitalWrite(LED3, 0);
						decz = decz/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					};
				}else{
					digitalWrite(LED2, 0);
					decz = decz/2;
					if(decz % 2 == 1){
						digitalWrite(LED3, 1);
						decz = (decz-1)/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					}else{
						digitalWrite(LED3, 0);
						decz = decz/2;
						if(decz % 2 == 1){
							digitalWrite(LED4, 1);
						}else{
							digitalWrite(LED4, 0);
						};
					};
				};
			};
		}else{
			write(clientfd, "\n\nUngueltige Zahl!\n\r",88);
			write(clientfd, "Gültige Werte: 0 - 15\n\n\r",38);
		};			
		};

	
	close(clientfd);
	return NULL;
} 
int main()
{
	int server_socket, rec_socket;
	unsigned int len;
	struct sockaddr_in serverinfo, clientinfo;
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0 );
	serverinfo.sin_family = AF_INET;
    
	serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverinfo.sin_port = htons(PORT);
	
	if (bind(server_socket, (struct sockaddr *) &serverinfo, sizeof(serverinfo)) != 0){
		printf("Fehler Socket\n");
		return 1; 
	}
	
	
	listen(server_socket, QUEUE);
	
	

	while(1){
		printf("Server wartet auf Eingabe\n");

		rec_socket = accept(server_socket, (struct sockaddr *)&clientinfo, &len);
		printf("Verbindung von %s:%d\n", inet_ntoa(clientinfo.sin_addr), ntohs(clientinfo.sin_port));
		
		pthread_t child;
		if (pthread_create(&child, NULL, mirror, &rec_socket) != 0) {
			perror("child error"); 
			return 1;
		}
		else { 
			printf("Abgekoppelt!\n"); 
			pthread_detach(child); 
		}
		
	}


	return 0;
}
