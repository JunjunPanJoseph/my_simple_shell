// COMP1521 18s2 mysh ... command history
// Implements an abstract data object

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(const char *s);

// Command History
// array of command lines
// each is associated with a sequence number

#define MAXHIST 20
#define MAXSTR  200

#define HISTFILE ".mymysh_history"
/*
typedef struct _history_entry {
   int   seqNumber;
   char *commandLine;
} HistoryEntry;

typedef struct _history_list {
   int nEntries;
   HistoryEntry commands[MAXHIST];
} HistoryList;
*/
typedef struct _history_node{
    int seqNumber;
    char *commandLine;
    HN next;
} *HN;

typedef struct _history_list {
   int nEntries;
   HN head;
} HistoryList;

static HN newHN(char *cmdLine, int seqNumber){
    HN tmp = malloc(sizeof(*HN));
    tmp->commandLine = cmdLine;
    tmp->seqNumber = seqNumber;
    tmp->next = NULL;

    return tmp;
}
static HN addHN(HN pre, HN new){
    new->next = pre;
    return pre;
}
static HN deleteHN(HN now){
    free(now->commandLine);
    free(now);
    return NULL;
}
static void remove_tail(HN now){
    if (now == NULL) return NULL;
    if (now->next == NULL){
        deleteHN(now);
        return NULL;
    }
    now->next = remove_tail(now->next);
    return now;
}

static void free_linklist(HN head){
    if (head ==NULL) return
    tmp = head->next;
    deleteHN(head);
    free_linklist(tmp);
}
HistoryList CommandHistory;

// initCommandHistory()
// - initialise the data structure
// - read from .history if it exists

int initCommandHistory()
{
   // TODO
   CommandHistory.nEntries = 0;
   CommandHistory.head = NULL;
   // avoid error
   FILE *tmp = fopen(HISTFILE, "w");
   fclose(tmp);
   
   
    FILE *fp = fopen(HISTFILE, "r");
    int tmp_n;
    char buffer[MAXSTR];
    char *tmp;
    while(fscanf(fp, " %3d  %s\n", &tmp_n, buffer)){
        tmp =  malloc(MAXSTR);
        strncpy(tmp, buffer, MAXSTR);
        addToCommandHistory(tmp, tmp_n);
    }
    fclose(fp);
    return 1;
   //read file
}

// addToCommandHistory()
// - add a command line to the history list
// - overwrite oldest entry if buffer is full

void addToCommandHistory(char *cmdLine, int seqNo)
{
   // TODO
   CommandHistory.head = addHN(CommandHistory.head, newHN(cmdLine, seqNumber));
   if (CommandHistory.nEntries == MAXHIST){
       CommandHistory.head = remove_tail(CommandHistory.head);
   } else {
       CommandHistory.nEntries++;
   }
}

// showCommandHistory()
// - display the list of 

void showCommandHistory(FILE *outf)
{
    //FILE *fp = fopen(HISTFILE, "r");
    FILE *fp = outf;
    int tmp_n;
    char buffer[MAXSTR];
    char *tmp;
    while(fscanf(fp, " %3d  %s\n", &tmp_n, buffer)){
        printf(" %3d  %s\n", tmp_n, buffer);
    }
    fclose(fp);
}

static char *rec_getCommandFromHistory(int cmdNo, HN node){
    if (node == NULL) return NULL;
    if (node->seqNumber == cmdNo) return node->commandLine;
    return rec_getCommandFromHistory(cmdNo, node->next);
}
// getCommandFromHistory()
// - get the command line for specified command
// - returns NULL if no command with this number
char *getCommandFromHistory(int cmdNo)
{
   return rec_getCommandFromHistory(cmdNo, CommandHistory.head);
}

// saveCommandHistory()
// - write history to $HOME/.mymysh_history
static void rec_save(FILE *fp, HN head){
    if (head == NULL) return;
    rec_save(fp,  head->next);
    fprintf(fp, " %3d  %s\n", head->seqNumber, head->commandLine);
    
}
void saveCommandHistory()
{
    FILE *fp = fopen(HISTFILE, "w");
    rec_save(fp, CommandHistory.head);
    fclose(fp);
}
int getNum(){
    if (CommandHistory.head == NULL) return 0;
    return CommandHistory.head->seqNumber;
}
// cleanCommandHistory
// - release all data allocated to command history

void cleanCommandHistory()
{
   free_linklist(CommandHistory.head)
}

