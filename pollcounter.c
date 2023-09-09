#include "poller.h"


void countVotes(const Voter voters[], PartyVote partyVotes[], int totalVotes, int partyCount) {
    for (int i = 0; i < totalVotes; i++) {
        for (int j = 0; j < partyCount; j++) {
            //printf("%s  and %s \n",voters[i].party, partyVotes[j].party);
            if (strcmp(voters[i].party, partyVotes[j].party) == 0) {
                // printf("voter is %d and part is %s\n",partyVotes[j].voteCount,partyVotes[j].party);
                partyVotes[j].voteCount++;
                break;
            }
        }
    }
}

int compareVoter(const void* a, const void* b) {
    const Voter* voter1 = (const Voter*)a;
    const Voter* voter2 = (const Voter*)b;
    return strcmp(voter1->name, voter2->name);
}

void displayResults( PartyVote partyVotes[], int totalVotes, int partyCount,FILE *fd) {
    qsort(partyVotes, partyCount, sizeof(PartyVote), compareVoter);
    for (int i = 0; i < partyCount; i++) {
        fprintf(fd,"%s: %d\n", partyVotes[i].party, partyVotes[i].voteCount);
    }
    fprintf(fd,"Total votes casted: %d\n", totalVotes);
}

void handle(PartyVote partyVotes[],int* partyCount,vote* v){
int partyIndex = -1;
for (int i = 0; i < *partyCount; i++) {
    if (strcmp(partyVotes[i].party, v->answer) == 0) {
        partyIndex = i;
        break;
    }
}

if (partyIndex == -1) {
    printf("NEOW v->answer is %s\n",v->answer);
    strcpy(partyVotes[*partyCount].party, v->answer);
    (*partyCount)++;
}
}
