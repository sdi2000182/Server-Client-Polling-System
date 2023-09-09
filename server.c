

#include "poller.h"
#include <semaphore.h>
#include <errno.h>


PartyVote partyVotesServer[MAX_VOTES];
int glob_server=0;
int tV =0 ;
int partyCounter;
volatile sig_atomic_t exit_flag = 0;

void handle_sigint(int signum) {
    exit_flag = 1;
	exitc = 1;
    printf("Exit!\n");
    close(sock_global);
	shutdown(sock_global, SHUT_RDWR);
	pthread_cond_broadcast (& cond_nonempty);

}


int threadIsActive(pthread_t thread) {
    int result = pthread_kill(thread, 0);
    if (result == 0) {
        return 1; // Thread is active
    } else if (result == ESRCH) {
        return 0; // Thread is not active
    } else {
        perror("pthread_kill");
        return -1; // Error occurred
    }
}



void initialize ( pool_t * pool, int bufferSize ) {
	pool -> start = 0;
	pool -> end = -1;
	pool -> count = 0;
	pool -> bufferSize = bufferSize;
	pool -> buffer = malloc (bufferSize*sizeof(int));
	if (pool -> buffer == NULL) {
		perror ("buffer malloc ");
		exit (1) ;
	}
	
}

void place ( pool_t * pool , int data ) {
	pthread_mutex_lock (& mtx );
	while (pool -> count >= pool->bufferSize ) {
		printf (" >> Found Buffer Full \n");
		pthread_cond_wait (& cond_nonfull , &mtx);
	}
	pool -> end = (pool -> end + 1) % pool->bufferSize ;
	pool -> buffer [pool -> end ] = data ;
	pool -> count ++;
	printf("place: %d\n", pool->count);
	pthread_mutex_unlock (& mtx );
}

int obtain( pool_t * pool ) {
	int data = 0;
	pthread_mutex_lock (& mtx );
	while (pool -> count <= 0 && exitc!=1) {
		printf (" >> Found Buffer Empty \n");
		if(exitc==1){
		}
		pthread_cond_wait (& cond_nonempty , &mtx );
	}
	data = pool -> buffer [pool -> start ];
	pool -> start = (pool -> start + 1) % pool->bufferSize ;
	pool -> count --;
	pthread_mutex_unlock(&mtx);
	//printf("data is %d\n",data);
	return data ;
}

void errors(int newsock, char *msg) {
	perror(msg);
	printf("\n");
	fflush(stdout);
	close(newsock);
	close(sock_global);
	shutdown(sock_global, SHUT_RDWR);	
}





int main(int argc, char *argv[]) {
	pthread_t *thr ;
	pthread_t cons , prod ;
	initialize_server_states(); 
	size_st = 0;
	char r;
	int err;
	int port, sock, newsock;
	int i, WorkerNum, bufferSize, size, read;
	FILE *fd;
	(void) signal(SIGINT,handle_sigint);
	vote *v;
	struct timeval tim1;
	unsigned int serverlen, clientlen;			
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;
	
	if ( argc < 7 )				
	{
		fprintf(stderr, "Paradeigma: ./poller [portnum] [WorkerNum] [buff] [polling] [stat.txt] [log.txt]\n");
		exit(0);
	}
	exitc = 0;
	
	port = atoi(argv[1]);	//thyra 
	WorkerNum = atoi(argv[2]);	//workers
	if (WorkerNum < 0) {
		fprintf(stderr, "Workers>0\n");
		exit(0);
	}
	bufferSize = atoi(argv[3]);
	if (bufferSize < 0) {
		fprintf(stderr, "Buff>0\n");
		exit(0);
	}
	
	fd = fopen(argv[4], "r");
	if (fd == NULL) {
		fprintf(stderr, "To polling start arxeio dn yparxei!\n");
		exit(0);	
	}
	size = 0;
	while (!feof(fd)) {
		fread(&r, sizeof(char), 1, fd) ;
		size += 1;
	}
	info.polling_start = malloc(sizeof(char)*size);
	fseek(fd, 0, SEEK_SET);
	i = 0;
	while (!feof(fd)) {
		fread(&info.polling_start[i], sizeof(char), 1, fd);
		i += 1;
	}
	
	fclose(fd);
	
	info.poll_stat = malloc((strlen(argv[5]) +1) * sizeof(char));
	sprintf(info.poll_stat, "%s", argv[5]);

	if (fopen(info.poll_stat, "r") == NULL) {
		if (fopen(info.poll_stat, "w") == NULL) {
			fprintf(stderr, "Error!\n");
			exit(0);
		}
	}
	info.poll_loger = malloc((strlen(argv[6])) * sizeof(char));
	sprintf(info.poll_loger, "%s", argv[6]);
	if (hash_init() != HASH_OK) {
		fprintf(stderr, "Error!\n");
		exit(0);
	}
	
	
	fd = fopen(info.poll_loger, "w");
	if ( fd == NULL) {
		fprintf(stderr, "Error!\n");
		exit(0);
	}
	
	pthread_mutex_init (& hash_mtx , 0);
	
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
		perror("socket");
		exit(1);
	}
	port = atoi(argv[1]);
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY); 
	server.sin_port = htons(port); 
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof(server);
	
	clientptr = (struct sockaddr *) &client;
	clientlen = sizeof (client);
	
	sock_global = sock;
	
	if (bind(sock, serverptr, serverlen) < 0) {/* Bind socket to address */
		perror("bind"); exit(1); 
	}
	if (listen(sock, 5) < 0) { /* Listen for connections */
		perror("listen"); exit(1); 
	}
	
	printf("Listening for connections to port %d\n", port);
	
	
	initialize (& pool, bufferSize );
	pthread_mutex_init (& mtx , 0);
	pthread_mutex_init (& mtx1, 0);
	pthread_cond_init (& cond_nonempty , 0);
	pthread_cond_init (& cond_nonfull , 0);
	
	pthread_mutex_init (& file , 0);
	pthread_mutex_init (& stat_file , 0);
	
	thr = malloc(sizeof(pthread_t) * WorkerNum);
	if (thr == NULL) {
		perror ("malloc ");
		exit (1) ;
	}
	printf("there\n");
	
	pthread_mutex_init (& worker_id , 0);
	workerNumber = 0;
	int thread_args[WorkerNum];
	for (i = 0 ; i < WorkerNum ; i++) {
		thread_args[i] = i;
		if (err = pthread_create (& thr[i] , NULL , worker , (void *)&thread_args[i])) { /* New thread */
			perror (" pthread_create " );
			exit (1) ;
		}
		
	}
	fd_set readfds;
    int maxfd = sock + 1;
	 while (!exit_flag) {
        fd_set readfds;
        FD_ZERO(&readfds);
     
	    FD_SET(sock, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // Timeout of 100ms

        int ready_fds = select(sock + 1, &readfds, NULL, NULL, &timeout);
        if (ready_fds < 0) {
            if (exit_flag) {
                // Check if termination signal received while waiting for a connection
                break;
            } else {
                perror("select");
                continue; // Continue accepting connections
            }
        } else if (ready_fds == 0) {
            // Timeout occurred, no new connections
            continue;
        }

        if (FD_ISSET(sock, &readfds)) {
            if ((newsock = accept(sock, clientptr, &clientlen)) < 0) {
                if (exit_flag) {
                    // Check if termination signal received while waiting for a connection
                    break;
                } else {
                    perror("accept");
                    continue; // Continue accepting connections
                }
            }
            place (&pool , newsock );
			printf (" producer : %d\n", newsock );
			pthread_cond_broadcast (& cond_nonempty );
			usleep (0) ;
        }
    }

    close(sock);
	if ( (fd = fopen(info.poll_stat, "w") ) == NULL) {
		fprintf(stderr, "Error while opening data file to write vote counters!\n");
		exit (1);
	}
	
	fseek(fd, 0, SEEK_END);
	call_display(fd);
	fclose(fd);
	pthread_cond_destroy (& cond_nonempty );
	pthread_cond_destroy (& cond_nonfull );
	pthread_mutex_destroy (& mtx );
	pthread_mutex_destroy (& hash_mtx );
	pthread_mutex_destroy (& worker_id );
	pthread_mutex_destroy (& file );
	pthread_mutex_destroy (& stat_file );
	//hash_free();
	free(pool.buffer);
	printf("Exit\n");
	// exit(1);
	for (i = 0; i < WorkerNum; i++) {
    printf("Joining thread %d\n", i);
    if (err = pthread_join(*(thr + i), NULL)) {
        perror("pthread_join");
        exit(1);
    }
    printf("Thread %d joined successfully\n", i);
    fflush(stdout);
	}

	pthread_exit (NULL );
}