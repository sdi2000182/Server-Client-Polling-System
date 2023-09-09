#include "poller.h"


char *serverName;
int portNumber, numVotes=0;
int glob=0;
int partyCount = 0;
int glob2 = 0 ;
sem_t globmut;
sem_t globmut1;


void errors_client(int newsock, char *msg) {
	perror(msg);
	close(newsock);
	close(sock_global);
	shutdown(sock_global, SHUT_RDWR);	
}



void handleVoters(FILE *file, Voter voters[], PartyVote partyVotes[], int *totalVotes, int *partyCount) {
    char line[MAX_VOTES];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline character

        char name[2*MAX_VOTES];
        char surname[2*MAX_VOTES];
        char party[2*MAX_VOTES];
        sscanf(line, "%s %s %s", name, surname, party);

        strcpy(voters[*totalVotes].name, name);
        strcat(voters[*totalVotes].name, " ");
        strcat(voters[*totalVotes].name, surname);
		//printf("voters in main party is %s\n",party);
        strcpy(voters[*totalVotes].party,party);

        int hasVoted = 0;
        for (int i = 0; i < *totalVotes; i++) {
            if (strcmp(voters[*totalVotes].name, voters[i].name) == 0) {
                hasVoted = 1;
                break;
            }
        }

        if (!hasVoted) {
            int partyIndex = -1;
            for (int i = 0; i < *partyCount; i++) {
                if (strcmp(partyVotes[i].party, party) == 0) {
                    partyIndex = i;
                    break;
                }
            }

            if (partyIndex == -1) {
                strcpy(partyVotes[*partyCount].party, party);
                (*partyCount)++;
            }

            (*totalVotes)++;
        }
    }
	// for(int i =0 ; i<*totalVotes;i++){
	// 	printf("voters outside loop is %s\n",voters[i].party);
	// }
}

Voter voters[MAX_VOTES];
//Voter1 voters1[MAX_VOTES];
PartyVote partyVotes[MAX_VOTES];

void polling_starter(int sock, int choice) {
	char x;
	int output;
	int read_size, i;
	

	if(choice == 1) 
		output = VOTE_BEGIN;
	else if(choice == 2) 
		output = POLL_RESULTS;
	else if(choice == 3)
		output = QUIT;
	if (write( sock, &output, sizeof(int) ) < 0) {
		perror("read");
		close(sock);
		exit(1);
	}

}

void duplicate_voter(int sock) {
	printf("Duplicate Voter!\n");
	fflush(stdout);
}

void everything_ok(int sock) {
	printf("You have succesfully voted\n");
	fflush(stdout);
}

//int ind1=0;

void send_vote(int sock,int ind1) {
	sem_wait(&globmut);
	char x;
	char vote;
	int read_size, i;
	char voting[20];
	int size_name = strlen(voters[ind1].party)+1;
	
	/*Random Username*/
	for (i = 0 ; i < size_name - 1 ; i++)
		voting[i] = voters[ind1].party[i];
	voting[size_name - 1] = '\0'; 

	int write_size = strlen(voting) + 1;
	/*Sending size to server*/
	if( write(sock, &write_size, sizeof(int) ) < 0)
	{
		errors_client(sock, "write_client");
		exit(1);
	}
	
	/*credentials response*/
	/* sending username */
	for(i = 0; i < write_size ; i++) {
		if( write(sock, &voting[i], sizeof(char) ) < 0)
		{
			errors_client(sock, "write_client");
			exit(1);
		}
	}
	//ind1++;
	sem_post(&globmut);
}

void send_user(int sock,int ind) {
	sem_wait(&globmut1);
	int i, size_name;
	char  username[20];
	int write_size;
	
	size_name = strlen(voters[ind].name)+1;
	for (i = 0 ; i < size_name - 1 ; i++)
		username[i] = voters[ind].name[i];
	username[size_name - 1] = '\0'; 

	write_size = strlen(username) + 1;
	//stelno size
	if( write(sock, &write_size, sizeof(int) ) < 0)
	{
		errors_client(sock, "write_client");
		exit(1);
	}
	
	//stelno username
	for(i = 0; i < write_size ; i++) {
		if( write(sock, &username[i], sizeof(char) ) < 0)
		{
			errors_client(sock, "write_client");
			exit(1);
		}
	}
	//ind++;
	sem_post(&globmut1);
}


void *client(void *arg) {
	int index = *(int *)arg;
	//printf("entered client\n");
	int sock;
	struct sockaddr_in server;
	struct sockaddr *serverptr = ( struct sockaddr *) &server;	
	struct hostent *rem;
	
	int input, choice;
	
	if( ( sock = socket ( AF_INET, SOCK_STREAM, 0) ) < 0) {
		perror("socket");
		exit(1);
	}
	
	/* Find server address */
	if (( rem = gethostbyname ( serverName) ) == NULL ) {
		herror("gethostbyname"); 
		exit (1);
	}
	
	server.sin_family = AF_INET;	/* Internet domain */
	memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
	server.sin_port = htons ( portNumber );
	
	if ( connect ( sock, serverptr, sizeof( server ) ) < 0) {
		perror("connect") ;
		exit(1);
	}
		
	printf("Connecting to port %d \n", portNumber);
	choice = 1;
	polling_starter(sock, choice);
	if( read(sock, &input, sizeof(int)) < 0 )	
	{
		printf("read1\n");
		exit(1);
	}
	send_user(sock,index);
	
	
	if( read(sock, &input, sizeof(int)) < 0 )
	{
		printf("read1\n");
		exit(1);
	}
	if (input == VOTING1) {
		send_vote(sock,index);
		if( read(sock, &input, sizeof(int)) < 0 )	
		{
			printf("read1\n");
			exit(1);
		}
		everything_ok(sock);
	}
	else if (input == VOTE_AGAIN) {
		duplicate_voter(sock);
	}
	
	choice = 3;
	polling_starter(sock, choice);
	if( read(sock, &input, sizeof(int)) < 0 )
	{
		printf("read1\n");
		exit(1);
	}
	
	close(sock);
	pthread_exit (NULL) ; 
	
	
}


int main(int argc, char **argv) {
	sem_init(&globmut,0,1);
	sem_init(&globmut1,0,1);
	int i;
	int err;
    char line[MAX_VOTES];
	
	pthread_t *thr ;
	
	srand(time (NULL) );
	
	if(argc != 4) {
		fprintf(stderr, "Wrong input..\n");
		fprintf(stderr, "Paradeigma: prompt> ./pollSwayer [servername] [portnum] [inputFile]) \n");
		exit( EXIT_FAILURE );
	}
	
	serverName = malloc( (strlen(argv[1]) + 1) * sizeof(char));
	strcpy(serverName, argv[1]);
	portNumber = atoi(argv[2]);
	char* inputFile = malloc( (strlen(argv[3]) + 1) * sizeof(char));
	strcpy(inputFile, argv[3]);
	FILE *inputFD;
	inputFD = fopen(inputFile, "r");
	handleVoters(inputFD, voters, partyVotes, &numVotes, &partyCount);
    countVotes(voters,partyVotes,numVotes,partyCount);
	thr = malloc(sizeof(pthread_t) * numVotes);
	if (thr == NULL) {
		perror ("malloc ");
		exit (1) ;
	}
	int thread_args[numVotes];

    for (i = 0; i < numVotes; i++) {
        thread_args[i] = i;
        if (pthread_create(&thr[i], NULL, client, (void *)&thread_args[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
	
	
	for (i = 0 ; i < numVotes ; i++) {
		if (err = pthread_join (*( thr +i), NULL )) {
			perror (" pthread_join ");
			exit (1) ;
		}
	}

	free(serverName);

	pthread_exit (NULL );
}
