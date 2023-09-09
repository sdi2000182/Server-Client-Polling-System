# Multi-Threaded Network Server (Poller)

This project is a multi-threaded network server implemented in C/C++ named "poller." The server conducts a pre-election poll of political parties and stores user votes and poll statistics. It is designed to run in a Unix environment.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [License](#license)

## Overview

The "poller" server is divided into several components:

- **Master Thread**: Responsible for accepting client connections and placing them in a buffer.
- **Worker Threads**: Handle client requests, record votes, and communicate with clients.
- **Buffer**: A shared data structure used to synchronize communication between master and worker threads.
- **Data Storage**: Stores user votes and poll statistics in separate log files.

The `server.c` file contains the main implementation of the multi-threaded network server known as "poller." This server conducts a pre-election poll of political parties, stores user votes, and manages poll statistics. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Signal Handling**: The `handle_sigint` function is responsible for handling the SIGINT signal (Ctrl+C) to gracefully exit the server.

- **Thread Monitoring**: The `threadIsActive` function checks whether a given thread is active or not. It uses `pthread_kill` to determine the thread's status.

- **Initialization**: The `initialize` function initializes the pool used for buffering client connections. It allocates memory for the buffer and sets initial values for pool attributes.

- **Buffer Management**: The `place` function places a new socket descriptor into the buffer, while the `obtain` function retrieves a socket descriptor from the buffer.

- **Error Handling**: The `errors` function is used for reporting and handling errors. It closes sockets and performs shutdown operations when errors occur.

- **Main Function**: The `main` function is the entry point of the server. It initializes various components, including socket handling, buffer, worker threads, and signal handling.

## workers.c - Worker Threads

The `workers.c` file contains the implementation of worker threads used in the multi-threaded network server. These worker threads handle client connections, process user votes, and manage various states of the server. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Worker Threads**: The `worker` function represents the main routine for each worker thread. It handles client connections, processes user votes, and transitions between different states of the server.

- **Client Connection Handling**: Worker threads continuously monitor the buffer for incoming client connections. When a connection is available, a worker thread retrieves it, processes the client's requests, and handles voting procedures.

- **State Management**: Worker threads manage various states of the server, including the initial state, username acquisition, results retrieval, and quitting. They transition between these states based on client interactions.

- **Error Handling**: The `errors` function is used for reporting and handling errors during client interactions. It ensures that the server gracefully handles errors and cleans up resources.

Default (GPT-3.5)

User
Σκοπός
Ο στόχος αυτής της εργασίας είναι να εξοικειωθείτε µε τον προγραμματισμό συστήματος σε Unix
περιβάλλον και συγκεκριμένα, µε τη δημιουργία νημάτων και τη δικτυακή επικοινωνία. H εργασία
χωρίζεται σε τρία μέρη.
Problem 1: Multi-threaded network server (50 points).
Περιγραφή - Εκτέλεση.
Γράψτε έναν πολυνηματικό εξυπηρετητή δικτύου σε C/C++ που ονομάζεται poller το οποίο εκτελεί μια
προεκλογική δημοσκόπηση κομμάτων. Το πρόγραμμα θα το τρέχετε από τη γραμμή εντολής με τα
ακόλουθα ορίσματα
prompt> poller [portnum] [numWorkerthreads] [bufferSize] [poll-log][poll-stats]
Συγκεκριμένα τα ορίσματα είναι:
● portnum: ο αριθμός θύρας που ακούει ο εξυπηρετητής
● numWorkerthreads: ο αριθμός νημάτων εργατών που θα δημιουργήσει για να κάνει τη
δημοσκόπηση. Πρέπει να είναι > 0.
● bufferSize: το μέγεθος ενός buffer που θα κρατά συνδέσεις από πελάτες που περιμένουν να
εξυπηρετηθούν. Πρέπει να είναι > 0.
● poll-log: το όνομα ενός αρχείου όπου θα αποθηκεύονται τα ονόματα χρηστών και οι ψήφοι
τους.
● poll-stats: το όνομα ενός αρχείου όπου θα αποθηκεύονται τα αποτελέσματα (π.χ., ο αριθμός
χρηστών που έχουν ψηφίσει καθε κόμμα).
Τα ορίσματα είναι όλα υποχρεωτικά και πρέπει να δίνονται με τη συγκεκριμένη σειρά και μόνο.
Για παράδειγμα, αν τρέξετε το πρόγραμμα σας με τα ακόλουθα ορίσματα:
prompt> poller 5634 8 16 pollLog.txt pollStats.txt
τότε ο εξυπηρετητής θα ακούει στη θύρα 5634, θα δημιουργήσει 8 νήματα εργάτες, θα χρησιμοποιήσει
ένα buffer που κρατά μέχρι 16 συνδέσεις που περιμένουν εξυπηρέτηση, και θα αποθηκεύει τις ψήψους
των χρηστών στο pollLog.txt και τα αποτελέσματα της δημοσκόπησης στο αρχείο pollStats.txt.
Υλοποίηση
Ο εξυπηρετητής που θα υλοποιήσετε θα υποστηρίζει
● τη δημιουργία ενός νήματος-αρχηγού
● τη δημιουργία του buffer για την αποθήκευση των συνδέσεων
● την ενημέρωση του log αρχείου με τα στατιστικά της δημοσκόπησης
Master thread
Στην υλοποίηση σας, θα πρέπει να έχετε ένα νήμα-αρχηγό (master thread) που ξεκινά δημιουργώντας
numWorkerThread νήματα-εργάτες. Το master thread θα δέχεται συνδέσεις από πελάτες με την
accept κλήση συστήματος και θα τοποθετεί τους περιγραφείς αρχείων που αντιστοιχούν στις
συνδέσεις σε έναν buffer συγκεκριμένου μεγέθους (που ορίζεται από το bufferSize). Το νήμα-αρχηγός
ΔΕΝ θα διαβάζει από τις συνδέσεις που δέχεται. Απλώς, όποτε δέχεται κάποια σύνδεση θα τοποθετεί
τον socket descriptor στον buffer και θα συνεχίζει να δέχεται επόμενες συνδέσεις.
Workers threads
Η δουλειά των νημάτων-εργατών είναι να διαβάζουν τις αιτήσεις από τα socket descriptors και να
εξυπηρετούν τους πελάτες. Ένα νήμα-εργάτης ξυπνά όταν υπάρχει τουλάχιστον ένας descriptor στον
buffer (δηλαδή τουλάχιστον ένας πελάτης έχει συνδεθεί στον εξυπηρετητή).
Όταν ξυπνά, το νήμα-εργάτης, στέλνει το μήνυμα “SEND NAME PLEASE” και περιμένει να διαβάσει
από τον socket descriptor την απόκριση (το όνομα ενός ψηφοφόρου) από τον πελάτη.
Αν ο ψηφοφόρος έχει ήδη ψηφίσει, στέλνει απόκριση “ALREADY VOTED” και τερματίζει τη σύνδεση.
Αλλιώς στέλνει το μήνυμα “SEND VOTE PLEASE” και περιμένει να διαβάσει την απόκριση (μια
συμβολοσειρά με το όνομα ενός κόμματος).
Θα τυπώνει σε μια γραμμή, στο τέλος του poll-log αρχείου, το όνομα του ψηφοφόρου και το κόμμα που
ψήφισε. Τέλος στέλνει ένα μήνυμα “VOTE for Party XYZ RECORDED” (όπου το XYZ είναι το κόμμα
που ψήφισε ο ψηφοφόρος) και τερματίζει τη σύνδεση.
Συγχρονισμός Master-workers μέσω του buffer.
Το νήμα-αρχηγός και τα νήματα-εργάτες έχουν σχέση παραγωγού-καταναλωτή και έτσι στην υλοποίηση
σας θα πρέπει οι προσβάσεις τους στο κοινό buffer να συγχρονίζονται.
Συγκεκριμένα:
● το νήμα-αρχηγός πρέπει να μπλοκάρεται και να περιμένει όταν ο buffer είναι γεμάτος
● ένα νήμα-εργάτης πρέπει να περιμένει αν ο buffer είναι άδειος.
Με αυτή τη προσέγγιση, αν υπάρχουν περισσότερα νήματα-εργάτες από ενεργές συνδέσεις, τότε
κάποια από τα νήματα-εργάτες θα μπλοκάρονται, περιμένοντας νέες συνδέσεις να φτάσουν στον
εξυπηρετητή.
Σε αυτήν την εργασία, θα πρέπει να χρησιμοποιήσετε μεταβλητές συνθήκης στην υλοποίηση σας. Αν
η υλοποίηση σας κάνει οτιδήποτε busy-waiting, θα υπάρξει ποινή (-20 μονάδες).
Εξυπηρετητής- Αποθήκευση Στατιστικών
Ο εξυπηρετητής θα πρέπει να κρατήσει κάποια στοιχεία για την δημοσκόπηση:
● τα ονόματα χρηστών που έχουν συμμετάσχει μέχρι στιγμής στη δημοσκόπηση μαζί με τις
ψήφους τους, και
● τον αριθμό των χρηστών που έχουν ψηφίσει καθε κόμμα.
Στην υλοποίηση, θα πρέπει να αποθηκεύονται οι ψήφοι που λαμβάνει ο εξυπηρετητής στα αρχεία
poll-log και poll-stats καθώς στέλνονται. Προσοχή εάν πάρει σήμα τερματισμού ο εξυπηρετητής θα
πρέπει να γράψει τις ψήφους που έχει πάρει ως εκείνη τη στιγμή πριν τερματίσει ώστε το αποτέλεσμα
να είναι σωστό. Μπορείτε να κρατήσετε αυτά τα στοιχεία σε οποιαδήποτε δομή δεδομένων σας
εξυπηρετεί.
Καθώς τα νήματα που εξυπηρετούν τους χρήστες θα πρέπει να ενημερώνουν τα δεδομένα που κρατά
ο server, θα πρέπει να υλοποιήσετε και τον κατάλληλο συγχρονισμό ώστε να γίνονται
τροποποιήσεις στα δεδομένα με σωστό τρόπο.
Ασχέτως τον τρόπο που θα επιλεγεί για την ενημέρωση του αρχείου, η τελική τους μορφή θα έχει ώς
εξής
αρχείο poll-log:
voter_name1 party_voted_for1
voter_name2 party_voted_for2
.
.
.
O εξυπηρετητής θα τρέχει μέχρι να λάβει ένα SIGINT (control-C) σήμα. Όταν λάβει SIGINT, θα γράφει
τα αποτελέσματα της δημοσκόπησης στο αρχείο poll-stats με την ακόλουθη μορφή:
αρχείο poll-stats
party_name1 number_of_votes
party_name2 number_of_votes
party_name3 number_of_votes
.
.
.
TOTAL total_number_of_votes
markdown readme for github explaining the project etc
ChatGPT
It looks like you have provided a detailed description of a programming assignment related to creating a multi-threaded network server in a Unix environment. The assignment involves creating a server named "poller" that conducts a pre-election poll of political parties. Below, I'll provide a breakdown of the assignment's requirements and implementation details:

Assignment Overview
Problem Description
The goal of this assignment is to familiarize yourself with system programming in a Unix environment, specifically focusing on creating threads and network communication. The assignment is divided into three parts, and Problem 1 is to create a multi-threaded network server.

Command-Line Arguments
The program should be run from the command line with the following arguments:

shell
Copy code
prompt> poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
Where:

portnum: The port number the server will listen on.
numWorkerthreads: The number of worker threads to create for conducting the poll.
bufferSize: The size of a buffer that holds connections from clients waiting to be served.
poll-log: The name of a file to store user names and their votes.
poll-stats: The name of a file to store poll results.
All arguments are mandatory and must be provided in the specified order.

Implementation Details
Here are some key implementation details for your server:

Master Thread
The master thread should create numWorkerthreads worker threads.
It accepts connections from clients using the accept system call and



User
i want u to write markdown readme for github
ChatGPT
Certainly! Here's a template for a GitHub README.md file for your multi-threaded network server project:

markdown
Copy code
# Multi-Threaded Network Server (Poller)

This project is a multi-threaded network server implemented in C/C++ named "poller." The server conducts a pre-election poll of political parties and stores user votes and poll statistics. It is designed to run in a Unix environment.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Project Structure](#project-structure)
- [How to Build and Run](#how-to-build-and-run)
- [License](#license)

## Overview

The "poller" server is divided into several components:

- **Master Thread**: Responsible for accepting client connections and placing them in a buffer.
- **Worker Threads**: Handle client requests, record votes, and communicate with clients.
- **Buffer**: A shared data structure used to synchronize communication between master and worker threads.
- **Data Storage**: Stores user votes and poll statistics in separate log files.

## Usage

To run the "poller" server, use the following command line arguments:

```shell
prompt> poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
portnum: Port number the server listens on.
numWorkerthreads: Number of worker threads for conducting the poll.
bufferSize: Size of the buffer for holding client connections.
poll-log: File name to store user names and their votes.
poll-stats: File name to store poll results.
Make sure to provide all arguments in the specified order.

Implementation Details
The master thread accepts client connections and places file descriptors in the buffer.
Worker threads read client requests, record votes, and communicate with clients.
Synchronization is achieved through conditional variables to handle buffer access.
User votes and poll statistics are stored in separate log files.
Project Structure
Here is an overview of the project's structure:

plaintext
Copy code
- src/             # Source code directory
  - poller.c       # Main server implementation
  - buffer.c       # Buffer implementation
  - worker.c       # Worker thread implementation
- include/         # Header files
- docs/            # Documentation files
- README.md        # This README file
- LICENSE.txt      # Project license
How to Build and Run
To build and run the "poller" server, follow these steps:

Clone this repository:

shell
Copy code
git clone https://github.com/your-username/poller.git
Navigate to the project directory:

shell
Copy code
cd poller
Build the project using your preferred build system (e.g., make, CMake, etc.).

Run the server with the appropriate command-line arguments:

shell
Copy code
./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
The server will start, and you can connect clients to it.

License
This project is licensed under the MIT License. See the LICENSE.txt file for details.

css
Copy code

Feel free to customize this template with specific details and instructions relevant to your project.



User
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
}continue readme about server.c
ChatGPT
Certainly! Here's the continuation of the README for your "server.c" file:

server.c - Multi-Threaded Network Server
The server.c file contains the main implementation of the multi-threaded network server known as "poller." This server conducts a pre-election poll of political parties, stores user votes, and manages poll statistics. Below, you'll find an overview of key functionalities and components in this file.

Overview
Signal Handling: The handle_sigint function is responsible for handling the SIGINT signal (Ctrl+C) to gracefully exit the server.

Thread Monitoring: The threadIsActive function checks whether a given thread is active or not. It uses pthread_kill to determine the thread's status.

Initialization: The initialize function initializes the pool used for buffering client connections. It allocates memory for the buffer and sets initial values for pool attributes.

Buffer Management: The place function places a new socket descriptor into the buffer, while the obtain function retrieves a socket descriptor from the buffer.

Error Handling: The errors function is used for reporting and handling errors. It closes sockets and performs shutdown operations when errors occur.

Main Function: The main function is the entry point of the server. It initializes various components, including socket handling, buffer, worker threads, and signal handling.

Building and Running
To build and run the server, you need to compile this file along with other required source files. Ensure you have all the necessary dependencies installed and follow these steps:

Clone the project repository:

shell
Copy code
git clone https://github.com/your-username/poller.git
cd poller
Compile the project using your preferred build system (e.g., make, CMake, etc.).

Run the server with appropriate command-line arguments:

shell
Copy code
./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
Replace [portnum], [numWorkerthreads], [bufferSize], [poll-log], and [poll-stats] with the desired values.

License
This file and the entire project are licensed under the MIT License. See the LICENSE.txt file for details.

This README provides an overview of the server.c file, but it's important to include additional details, explanations, and comments directly in your code for a more comprehensive understanding. Additionally, consider creating separate header files and modularizing your code for improved readability and maintainability.

Feel free to expand this README with any specific implementation details, explanations, or usage instructions that are relevant to your project.




User
write the markdown code for all that
ChatGPT
Certainly! Here's the Markdown code for the README, including both the project overview and the server.c section:

markdown
Copy code
# Poller - Multi-Threaded Network Server for Election Polling

Poller is a multi-threaded network server implemented in C/C++ for conducting pre-election polls of political parties. This server facilitates network communication and manages client connections, votes, and polling statistics. It is designed to be run from the command line with customizable parameters.

## Table of Contents

- [Project Overview](#project-overview)
- [server.c - Multi-Threaded Network Server](#serverc-multi-threaded-network-server)
  - [Overview](#overview)
  - [Building and Running](#building-and-running)
  - [License](#license)

---

## Project Overview

The Poller project aims to familiarize users with system programming in a Unix environment, specifically focusing on creating multi-threaded network servers and network communication. The project is divided into three main parts.

### Problem 1: Multi-threaded Network Server (50 points)

#### Description

Write a multi-threaded network server in C/C++ called `poller` that conducts pre-election polls of political parties. The program can be executed from the command line with the following arguments:

```shell
prompt> poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
Specifically, the arguments are as follows:

portnum: The port number the server listens on.
numWorkerthreads: The number of worker threads to create for polling.
bufferSize: The size of a buffer that holds connections from waiting clients.
poll-log: The name of a file to store user names and their votes.
poll-stats: The name of a file to store poll results (e.g., the number of votes for each party).
All arguments are mandatory and must be provided in the specified order.

Implementation
The server implementation includes the following components:

Master Thread: The server has a master thread that creates worker threads. The master thread accepts client connections and places socket descriptors in a buffer for processing by worker threads.

Worker Threads: Worker threads handle client requests. They send and receive messages to/from clients, record votes, and update polling statistics. Worker threads also ensure synchronized access to shared resources.

Buffer Synchronization: The server uses condition variables to synchronize access to the shared buffer between the master thread and worker threads.

Error Handling: The server handles errors, closes sockets, and performs graceful shutdown when necessary.

Signal Handling: The server responds to SIGINT (Ctrl+C) to exit gracefully, ensuring that poll results are recorded correctly before termination.

Polling Statistics: The server maintains information about user votes and party-specific vote counts.

Building and Running
To build and run the server, follow these steps:

Clone the project repository:

shell
Copy code
git clone https://github.com/your-username/poller.git
cd poller
Compile the project using your preferred build system.

Run the server with the desired command-line arguments:

shell
Copy code
./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
Replace [portnum], [numWorkerthreads], [bufferSize], [poll-log], and [poll-stats] with the desired values.

License
This project and its components, including server.c, are licensed under the MIT License. For details, see the LICENSE.txt file.

Note: This README provides an overview of the server.c file and the Poller project. For a more comprehensive understanding, consult the code comments and additional documentation within the project source files.

css
Copy code

You can use this Markdown code as a README.md file for your GitHub repository. Make sure to replace `[portnum]`, `[numWorkerthreads]`, `[bufferSize]`, `[poll-log]`, and `[poll-stats]` with actual values and provide more specific details about your project as needed.



User
Certainly! Here's the continuation of the README for your "server.c" file:

## server.c - Multi-Threaded Network Server

The `server.c` file contains the main implementation of the multi-threaded network server known as "poller." This server conducts a pre-election poll of political parties, stores user votes, and manages poll statistics. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Signal Handling**: The `handle_sigint` function is responsible for handling the SIGINT signal (Ctrl+C) to gracefully exit the server.

- **Thread Monitoring**: The `threadIsActive` function checks whether a given thread is active or not. It uses `pthread_kill` to determine the thread's status.

- **Initialization**: The `initialize` function initializes the pool used for buffering client connections. It allocates memory for the buffer and sets initial values for pool attributes.

- **Buffer Management**: The `place` function places a new socket descriptor into the buffer, while the `obtain` function retrieves a socket descriptor from the buffer.

- **Error Handling**: The `errors` function is used for reporting and handling errors. It closes sockets and performs shutdown operations when errors occur.

- **Main Function**: The `main` function is the entry point of the server. It initializes various components, including socket handling, buffer, worker threads, and signal handling.

### Building and Running

To build and run the server, you need to compile this file along with other required source files. Ensure you have all the necessary dependencies installed and follow these steps:

1. Clone the project repository:

   ```shell
   git clone https://github.com/your-username/poller.git
   cd poller
   ```

2. Compile the project using your preferred build system (e.g., `make`, `CMake`, etc.).

3. Run the server with appropriate command-line arguments:

   ```shell
   ./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
   ```

   Replace `[portnum]`, `[numWorkerthreads]`, `[bufferSize]`, `[poll-log]`, and `[poll-stats]` with the desired values.

### License

This file and the entire project are licensed under the MIT License. See the [LICENSE.txt](LICENSE.txt) file for details.

---

This README provides an overview of the `server.c` file, but it's important to include additional details, explanations, and comments directly in your code for a more comprehensive understanding. Additionally, consider creating separate header files and modularizing your code for improved readability and maintainability.

Feel free to expand this README with any specific implementation details, explanations, or usage instructions that are relevant to your project.
for this MARKDOWN CODE NOWW
ChatGPT
Certainly! Here's the Markdown code continuation for your server.c section:

markdown
Copy code
## server.c - Multi-Threaded Network Server

The `server.c` file contains the main implementation of the multi-threaded network server known as "poller." This server conducts a pre-election poll of political parties, stores user votes, and manages poll statistics. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Signal Handling**: The `handle_sigint` function is responsible for handling the SIGINT signal (Ctrl+C) to gracefully exit the server.

- **Thread Monitoring**: The `threadIsActive` function checks whether a given thread is active or not. It uses `pthread_kill` to determine the thread's status.

- **Initialization**: The `initialize` function initializes the pool used for buffering client connections. It allocates memory for the buffer and sets initial values for pool attributes.

- **Buffer Management**: The `place` function places a new socket descriptor into the buffer, while the `obtain` function retrieves a socket descriptor from the buffer.

- **Error Handling**: The `errors` function is used for reporting and handling errors. It closes sockets and performs shutdown operations when errors occur.

- **Main Function**: The `main` function is the entry point of the server. It initializes various components, including socket handling, buffer, worker threads, and signal handling.

### Building and Running

To build and run the server, you need to compile this file along with other required source files. Ensure you have all the necessary dependencies installed and follow these steps:

1. Clone the project repository:

   ```shell
   git clone https://github.com/your-username/poller.git
   cd poller
Compile the project using your preferred build system (e.g., make, CMake, etc.).

Run the server with appropriate command-line arguments:

shell
Copy code
./poller [portnum] [numWorkerthreads] [bufferSize] [poll-log] [poll-stats]
Replace [portnum], [numWorkerthreads], [bufferSize], [poll-log], and [poll-stats] with the desired values.

License
This file and the entire project are licensed under the MIT License. See the LICENSE.txt file for details.

This README provides an overview of the server.c file, but it's important to include additional details, explanations, and comments directly in your code for a more comprehensive understanding. Additionally, consider creating separate header files and modularizing your code for improved readability and maintainability.

Feel free to expand this README with any specific implementation details, explanations, or usage instructions that are relevant to your project.

css
Copy code

You can use this Markdown code as a continuation of your existing README.md file, following the previous section. Make sure to replace placeholders such as `[portnum]`, `[numWorkerthreads]`, `[bufferSize]`, `[poll-log]`, `[poll-stats]`, and `[your-username]` with the actual values and information relevant to your project.



User
THE SAME THING ABOUT workers.c 
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
ChatGPT
Here's the continuation of the README for your "workers.c" file:

workers.c - Worker Threads
The workers.c file contains the implementation of worker threads used in the multi-threaded network server. These worker threads handle client connections, process user votes, and manage various states of the server. Below, you'll find an overview of key functionalities and components in this file.

Overview
Worker Threads: The worker function represents the main routine for each worker thread. It handles client connections, processes user votes, and transitions



User
write markdown code lov
ChatGPT
Certainly! Here's the continuation of the README for your "workers.c" file in markdown format:

markdown
Copy code
## workers.c - Worker Threads

The `workers.c` file contains the implementation of worker threads used in the multi-threaded network server. These worker threads handle client connections, process user votes, and manage various states of the server. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Worker Threads**: The `worker` function represents the main routine for each worker thread. It handles client connections, processes user votes, and transitions between different states of the server.

- **Worker Identification**: Each worker thread is identified by a unique ID, which is used to track and manage worker-specific tasks and responsibilities.

- **Client Connection Handling**: Worker threads continuously monitor the buffer for incoming client connections. When a connection is available, a worker thread retrieves it, processes the client's requests, and handles voting procedures.

- **State Management**: Worker threads manage various states of the server, including the initial state, username acquisition, results retrieval, and quitting. They transition between these states based on client interactions.

- **Error Handling**: The `errors` function is used for reporting and handling errors during client interactions. It ensures that the server gracefully handles errors and cleans up resources.

## pollswayer.c - Client Application

The `pollswayer.c` file represents the client application used to connect to the multi-threaded network server and facilitate the voting process. It reads voter data from an input file, connects to the server, and sends votes for processing. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Client Threads**: The `client` function represents the main routine for each client thread. It connects to the server, sends user information, and casts votes based on the provided voter data.

- **Client Initialization**: The client application initializes socket connections to the server using the server name and port number provided as command-line arguments.

- **Voter Data Handling**: The application reads voter data from an input file, extracting voter names and party affiliations, ensuring that duplicate voters are not processed.

- **Voting Process**: Clients interact with the server, sending user information and casting votes. They respond to server prompts and display messages based on the server's responses.

  ## server_helper.c - Server Helper Functions

The `server_helper.c` file contains essential helper functions and logic for the multi-threaded network server in the "poller" project. These functions manage server states, handle client interactions, and process votes. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Server State Initialization**: The `initialize_server_states` function initializes semaphores used for server state management, ensuring thread synchronization during critical sections.

- **Client Interaction**: Functions like `init_state`, `get_username`, `vote_selection`, and `results_server` handle client interactions. They manage user authentication, vote processing, and result retrieval.

- **Vote Handling**: The `vote_selection` function manages the voting process, validating votes, and recording them in the server's data structures.

- **Server State Management**: The server uses semaphores to control access to shared resources during client interactions and username processing.



