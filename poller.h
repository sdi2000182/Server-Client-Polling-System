#ifndef _H_
#define _H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


#define HASH_CAPACITY 512
#define HASH_OK -1
#define HASH_ERROR -2
#define FOUND 5
#define NOT_FOUND 6
#include <semaphore.h>
#define QUIT 100
#define MENU 200
#define NEW_CLI 300
#define	VOTE_AGAIN 400
#define NOT_VOTED 500
#define RESULTS 600
#define VOTING1 700
#define VOTE_REGISTERED 800
#define MAX_WORD_LENGTH 100
#define MAX_VOTES 256
#define VOTE_BEGIN 900
#define MAX_PARTIES 100
#define POLL_RESULTS 1000
sem_t semaphore;
#define GET_USERNAMES 1500
#define GET_RESULTS 1600
#define STATE_VOTE_SELECTION 1400
#define ARXIKO_STATE -5
#define TRUE 1
#define FALSE 2

typedef struct {
    char name[MAX_VOTES];
    // char surname[MAX_VOTES];
    char party[MAX_VOTES];
} Voter;

void initialize_server_states();

typedef struct {
    char username[MAX_VOTES];
    char party[MAX_VOTES];
} Voter1;

typedef struct {
    char party[MAX_VOTES];
    int voteCount;
} PartyVote;




typedef struct {
	char *poll_stat;
	char *poll_loger;
	char *polling_start;
}file_info; 

Voter votes_ser[MAX_VOTES];
int size_st;
file_info info;
sem_t globmut2;

typedef struct vote_struct{
	char username[20];
	char answer[20];	//YES or NO
	struct vote_struct *next;
} vote; 

typedef struct {
	int start ;
	int end ;
	int count ;
	int bufferSize;
	int *buffer;
} pool_t ;

int workerNumber; /*Number of Worker*/
double master_time;
	

volatile sig_atomic_t exitc;
sem_t globmut3;
pthread_mutex_t mtx ;
pthread_mutex_t mtx1 ;
pthread_cond_t cond_nonempty ;
pthread_cond_t cond_nonfull ;
pool_t pool ;

pthread_mutex_t file ;	
pthread_mutex_t stat_file;	
pthread_mutex_t worker_id;	
pthread_mutex_t hash_mtx;	

int sock_global;


void handle_sigint(int signum) ;
void initialize ( pool_t * pool, int bufferSize );
void place ( pool_t * pool , int data );
int obtain( pool_t * pool ) ;
void errors(int newsock, char *msg);
int init_state(int newsock);
void quit();
int get_username(int newsock, vote *v);
void vote_selection(int newsock, vote *v,int tate) ;
void results_server(int newsock);
void *worker();


void countVotes(const Voter voters[], PartyVote partyVotes[], int totalVotes, int partyCount);
void displayResults(PartyVote partyVotes[], int totalVotes, int partyCount,FILE*);
void handle(PartyVote partyVotes[],int* partyCount,vote* v);
void call_display(FILE* fd);

void menu(int sock);
void cred(int sock);
void dupvoter(int sock);
void not_voted(int sock) ;
void results(int sock);
void subject(int sock);
void registered(int sock);
void errors_client(int newsock, char *msg);

int hash_init();
int hash_insert(vote *v);
int hash_scan(char *username);
int hash_function(char* value);
#endif