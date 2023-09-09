#include "poller.h"


PartyVote partyVotes[MAX_VOTES];
int glob_1=0;
int pc_STATES=0;
int tv_STATES=0;
sem_t globmut2;
sem_t globmut3;
int index2=0;


void call_display(FILE* fd){
	countVotes(votes_ser,partyVotes,size_st,size_st);
	displayResults(partyVotes,size_st,pc_STATES,fd);
}

void initialize_server_states() {
    sem_init(&globmut2, 0, 1);
	sem_init(&globmut3, 0, 1);  // Initialize with initial value 1
}

int init_state(int newsock) {
	int readnum, i, answ, size_of_write;
	char menu[] = "SEND VOTE QUIT SEE RESULTS ";
	char temp;
	
	
	if( read(newsock, &answ, sizeof(int)) < 0) {
		errors(newsock, "server_read");
		exit(1);
	}
	
	return answ;
}


void quit() {
	printf("TELOS\n");
	fflush(stdout);
}

int get_username(int newsock, vote *v) {
	sem_wait(&globmut3);
	int readnum, i, found, size_of_write, size_of_read;
	char x;
	vote temp_vote;
	
	printf("vote cred..\n");
	fflush(stdout);
	
	//gia thn pshfo
	if( read(newsock, &size_of_read, sizeof(int) ) < 0)
	{
		errors(newsock, "read_server");
		exit(1);
	}

	for (i = 0 ; i < size_of_read ; i++) {
		if( read(newsock, &x, sizeof(char) ) < 0)
		{
			errors(newsock, "server_read");
			exit(1);
		}
		
		v->username[i] = x;
	}
	strcpy(votes_ser[index2].name,v->username);
	//printf("%s and %s in %d\n",votes_ser[index2].name,v->username,index2);
	index2++;
	size_st++;
	//printf("username = %s\n", v->username);
	fflush(stdout);
	
	pthread_mutex_lock (& hash_mtx );
	found = hash_scan(v->username);
	pthread_mutex_unlock (& hash_mtx );
	
	if (found == NOT_FOUND) {
		

		readnum = VOTING1;
		if( write(newsock, &readnum, sizeof(int) ) < 0)
		{	
			errors(newsock, "server_write");
			exit(1);
		}
		
		sem_post(&globmut3);
		return (index2-1);
	}
	else if (found == FOUND) {

		/* send input */
		readnum = VOTE_AGAIN;
		if( write(newsock, &readnum, sizeof(int) ) < 0)
		{	
			errors(newsock, "server_write");
			exit(1);
		}
		sem_post(&globmut3);
		return ARXIKO_STATE;
	}
}

void vote_selection(int newsock, vote *v,int index1) {
	sem_wait(&globmut2);
	int index1_copy = index1;
	int readnum, i, found = NOT_FOUND;
	FILE *fd;
	
	int size_of_read;
	char x;
	if( read(newsock, &size_of_read, sizeof(int) ) < 0)
	{
		errors(newsock, "read_server");
		exit(1);
	}
	for (i = 0 ; i < size_of_read ; i++) {
		if( read(newsock, &x, sizeof(char) ) < 0)
		{
			errors(newsock, "server_read");
			exit(1);
		}
		
		v->answer[i] = x;
	}
	strcpy(votes_ser[index1_copy].party,v->answer);
	handle(partyVotes,&pc_STATES,v);
	sem_post(&globmut2);
	pthread_mutex_lock (& hash_mtx );
	found = hash_scan(v->username);
	pthread_mutex_unlock (& hash_mtx );	
	
	if (found == FOUND) {
		readnum = VOTE_AGAIN ;
	}
	else if (found == NOT_FOUND) {
	

		pthread_mutex_lock (& file );
		if ( (fd = fopen(info.poll_loger, "r+") ) == NULL) {
		fprintf(stderr, "Error \n");
		exit (1);
	}
	fseek(fd, 0, SEEK_END);
	fwrite(votes_ser[index1_copy].name, sizeof(char), strlen(votes_ser[index1_copy].name), fd);
	fprintf(fd," ");
	fwrite(votes_ser[index1_copy].party, sizeof(char), strlen(votes_ser[index1_copy].party), fd);
	fprintf(fd,"\n");
	fclose(fd);
	pthread_mutex_unlock (& file );
	
	
		pthread_mutex_lock (& hash_mtx );
		if (hash_insert(v) != HASH_OK) {
			errors(newsock, "Error in hash_insert");
			exit(1);
		}
		
		pthread_mutex_unlock (& hash_mtx );
	
		usleep(100000);
	
		readnum = VOTE_REGISTERED ;
	}
	
	if( write(newsock, &readnum, sizeof(int) ) < 0)
	{	
		errors(newsock, "server_write");
		exit(1);
	}
	
}

void results_server(int newsock) {
	vote v;
	int found, readnum, i, size_of_write, size_of_read;
	char results[200] ;
	
	
	//gia to username
	if( read(newsock, &size_of_read, sizeof(int) ) < 0)
	{
		errors(newsock, "read_server");
		exit(1);
	}
	for (i = 0 ; i < size_of_read ; i++) {
		if( read(newsock, &v.username[i], sizeof(char) ) < 0)
		{
			errors(newsock, "server_read");
			exit(1);
		}
	}
	
	pthread_mutex_lock (& hash_mtx );
	found = hash_scan(v.username);
	
	pthread_mutex_unlock (& hash_mtx );
	
	if (found == NOT_FOUND) {
		readnum = NOT_VOTED ;
		if( write(newsock, &readnum, sizeof(int) ) < 0)
		{	
			errors(newsock, "server_write");
			exit(1);
		}
	}
	else if (found == FOUND) {

		readnum = RESULTS ;
		if( write(newsock, &readnum, sizeof(int) ) < 0)
		{	
			errors(newsock, "server_write");
			exit(1);
		}
		size_of_write = strlen(results) + 1;
		if( write(newsock, &size_of_write, sizeof(int) ) < 0)
		{
			errors(newsock, "writeserver");
			exit(1);
		}
		for(i = 0; i < size_of_write ; i++) {
			if( write(newsock, &results[i], sizeof(char) ) < 0)
			{	
				errors(newsock, "server_write");
				exit(1);
			}
		}
		
	}
}
