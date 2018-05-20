//TCP kliens
//gcc client.c -o client
//./client 9876
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 

#define SIZE 4096

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[SIZE];

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		printf("Problem in creating the socket");
		exit(2);
	}
	
	char *PORT = argv[1];
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =  htons(atoi(PORT));


	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
		printf("Problem in connecting to the server");
		exit(3);
	}

	
	printf("OK - Lapot kérsz \nPASSZ - passzolás\n");
		
	for (;;)
	{
		//egy menet
		
		int osszeg = 0;
		int osszeg_ellen = 0;
		
		for(;;)
		{
			//a kliens lényegében mindig csak vár egy üzenetre a szervertől, és ha kap valamit, akkor azt értelmezi
			memset(buffer, 0, 256);
			recv(sockfd, buffer, SIZE,0);
			//str = trim(str);
				
			//ha a kapott üzenet 1-el kezdődik, az azt jelenti, hogy kártyát osztottak nekünk, és ezt feldolgozzuk
			if(buffer[0] == '1'){
				//szétbontjuk a sztringet:
				//levágjuk a kezdő számot, majd felbontjuk ',' karakterekre
				char buffer_copy[SIZE];
				memcpy(buffer_copy, buffer, strlen(buffer)+1);
				char *p;
				p = strtok(buffer_copy, ",");

				char lap;
				char szin;
				if(p)
				{
					szin = p[2];
				}
				p = strtok(NULL, ",");
				if(p){
					lap = p[0];
					if(lap=='1'){
						lap='2';
					}
				}				
				printf("Lapod: %c %c\n", szin, lap);
				if(lap == 'J' || lap == 'Q' || lap == 'K'){
					osszeg += 10;
				}
				else if(lap == 'A'){
					if(osszeg < 11){
						osszeg +=11;
					}
					else{
						osszeg +=1;
					}
				}
				else{
					int ertek = lap - '0';
					osszeg += ertek ;
				}
				printf("Összesen: %d\n", osszeg);
			}
			else if(buffer[0] == '2'){
				//ha a kapott üzenet 2-vel kezdődik, az azt jelenti, lezárult egy kör, és az eredményt kaptuk meg
				// az eredményt küldték
				printf("%s \n",buffer);
				printf("\n\n\n\nLEZÁRULT A KÖR");
				printf("\n================================================================\n\n\n\n\n\n");
				break;
			}
			else if(buffer[0] == '3'){
				//ha a kapott üzenet 3-al kezdődik, az azt jelenti, hogy kártyát osztottak a másik játékosnak, és ezt feldolgozzuk
				//szétbontjuk a sztringet:
				//levágjuk a kezdő számot, majd felbontjuk ',' karakterekre
				char buffer_copy[SIZE];
				memcpy(buffer_copy, buffer, strlen(buffer)+1);
				char *p;
				p = strtok(buffer_copy, ",");

				char lap;
				char szin;
				if(p)
				{
					szin = p[2];
				}
				p = strtok(NULL, ",");
				if(p){
					lap = p[0];
					if(lap=='1'){
						lap='2';
					}
				}
				printf("Ellenfeled lapja: %c %c\n", szin, lap);
				if(lap == 'J' || lap == 'Q' || lap == 'K'){
					osszeg_ellen += 10;
				}
				else if(lap == 'A'){
					if(osszeg_ellen < 11){
						osszeg_ellen +=11;
					}
					else{
						osszeg_ellen +=1;
					}
				}
				
				else{
					int ertek = lap - '0';
					osszeg_ellen += ertek ;
				}
				printf("Ellenfeled összesen: %d\n", osszeg_ellen);
			}
			else if(buffer[0] == '4')
			{
				//ha a kapott üzenet 4-el kezdődik, az azt jelenti, hogy vár tőlünk választ a szerver
				//hogy passzolni szeretnék, vagy jöhet a következő kártya
				printf("Kér még kártyát vagy passzol?\n");
				char *beolvas = malloc(SIZE);
				memset(buffer, 0, 256);
				scanf("%255s", beolvas);
				memcpy(buffer, beolvas, strlen(beolvas)+1);
				send(sockfd, buffer, SIZE, 0);
			}
				
			printf("\n--------------------------\n");
				
		}

	}
		
	
	return 0;
}

