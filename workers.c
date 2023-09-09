#include "poller.h"



void *worker(void *arg){
	int indetif = *(int *)arg;
	int workNum;
	int readvar, i;
	int newsock;
	int connectionsNum = 0;
	int state, end, choice;
	double t2;
	FILE *fd;	
	vote *v;
	pthread_mutex_lock (& worker_id);
	workNum = workerNumber;
	workerNumber += 1;
	pthread_mutex_unlock (& worker_id);
	
	v = malloc(sizeof(vote));
		
	printf ("To nhma  %ld with id %d\n", pthread_self (), workNum);
	while(!exitc) {
		while ( pool.count > 0 && exitc!=1) {
			
			connectionsNum += 1;
			if(exitc!=1){
				newsock = obtain (& pool );
				if(exitc == 1){
					break;
				}
				pthread_cond_broadcast(& cond_nonfull );
				usleep (500000) ;}
			if(exitc==1){
				break;}
			state = ARXIKO_STATE;
			end = FALSE;
			for (;;) {
				if(exitc==1){
					break;
				}
				switch(state) {
					
					case ARXIKO_STATE:
						choice = init_state(newsock);
						switch(choice) {
							case VOTE_BEGIN:
								readvar = NEW_CLI;
								if( write(newsock, &readvar, sizeof(int) ) < 0)
								{
									errors(newsock, "server_write");
									exit(1);
								}
								state = GET_USERNAMES;
								break;
							case POLL_RESULTS:
								readvar = NEW_CLI;
								if( write(newsock, &readvar, sizeof(int) ) < 0)
								{
									errors(newsock, "server_write");
									exit(1);
								}
								state = GET_RESULTS;
								break;
							case QUIT:
								state = QUIT;
								break;
							default:
								state = QUIT;
								break;
						}
						break;
					case GET_USERNAMES:
						state = get_username(newsock, v);
						break;
					case GET_RESULTS:
						results_server(newsock);
						state = ARXIKO_STATE;
						break;
					case QUIT:
						readvar = QUIT;
						end = TRUE;
						break;
					default:
						if (state != GET_USERNAMES && state != GET_RESULTS && state != QUIT) {
							vote_selection(newsock, v,state);
							state = ARXIKO_STATE;
							break;
						}
				}
				if (end == TRUE) {
					if( write(newsock, &readvar, sizeof(int) ) < 0)
					{
						errors(newsock, "server_write");
						exit(1);
					}
					break;
				}
				if(exitc==1){
					break;
				}
				
			}
			close(newsock);
			if(exitc==1){
				break;
			}
		}
		if (exitc == 1){
			break;}		
	}
	free(v);
	pthread_exit (NULL) ; 
}