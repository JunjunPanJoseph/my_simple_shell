// mysh.c ... a small shell
// Started by John Shepherd, September 2018
// Completed by <<YOU>>, September/October 2018

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <glob.h>
#include <assert.h>
#include <fcntl.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(char *);

// Function forward references

void trim(char *);
int strContains(char *, char *);
char **tokenise(char *, char *);
char **fileNameExpand(char **);
void freeTokens(char **);
char *findExecutable(char *, char **);
int isExecutable(char *);
void prompt(void);


// Global Constants

#define MAXLINE 200

// Global Data

/* none ... unless you want some */


// Main program
// Set up enviroment and then run main loop
// - read command, execute command, repeat

int main(int argc, char *argv[], char *envp[])
{
   pid_t pid;   // pid of child process
   int stat;    // return status of child
   char **path; // array of directory names
   int cmdNo;   // command number
   int i;       // generic index

   // set up command PATH from environment variable
   for (i = 0; envp[i] != NULL; i++) {
      if (strncmp(envp[i], "PATH=", 5) == 0) break;
   }
   if (envp[i] == NULL)
      path = tokenise("/bin:/usr/bin",":");
   else
      // &envp[i][5] skips over "PATH=" prefix
      path = tokenise(&envp[i][5],":");
#ifdef DBUG
   for (i = 0; path[i] != NULL;i++)
      printf("path[%d] = %s\n",i,path[i]);
#endif

   // initialise command history
   // - use content of ~/.mymysh_history file if it exists

   cmdNo = initCommandHistory();

   // main loop: print prompt, read line, execute command

   char line[MAXLINE];
   char buffer[MAXLINE];
   prompt();
   char *tmp;
   // glob buffer
   glob_t gbuffer;
   int i;
   int fd[2];
   while (fgets(line, MAXLINE, stdin) != NULL) {
        trim(line); // remove leading/trailing space

        // TODO
        // Code to implement mainloop goes here
        // Uses
        // - addToCommandHistory()
        // - showCommandHistory()
        // - and many other functions
        // TODO
        //exit
        int error = 1;
        if (strcmp(line, "exit") == 0) {
            break;
        }
        if (strcmp(line, "") == 0) {
            prompt();
            continue;
        }
        if (line[0] == '!'){
            int tmpi = -1;
            if (strcmp(line, "!!") == 0){
                tmp = getCommandFromHistory(1); 
                strcpy(line, tmp);
                free(tmp);
                error = 0;
            } else if (fscanf(line, "!%d", &tmpi)){
                tmp = getCommandFromHistory(tmpi); 
                strcpy(line, tmp);
                free(tmp);
                error = 0;
            }
        }
        
        if (strcmp(line, "h") == 0 || strcmp(line, "history") == 0){
            FILE *fp = fopen(HISTFILE, "r");
            showCommandHistory(fp)
            fclose(fp);
            error = 0;
        }
        if (strcmp(line, "pwd") == 0){
            getpwd(buffer, MAXLINE);
            printf("%s\n", buffer);
            error = 0;
        }


        if (strcmp(command[0], "cd") == 0){
            if (chdir(command[1]) == 0){
                getpwd(buffer, MAXLINE);
                printf("%s\n", buffer);
                error = 0;
            }
        }
        char **command, tmp_command;
        
        tmp_command = tokenise(line, " ");

        command = fileNameExpand(tmp_command)
        freeTokens(tmp_command);
        int change_read = 0;
        for (i = 0; command[i] != NULL; i++){
            if (command[i + 1] == NULL && strcmp(command[i], '<') == 0){
                //error
                error = 1;
                break;
            }
            
            if (strcmp(command[i], '<') == 0){
                change_read = 1;
                
                break;
            }
        }

        semaphore p;
        init (p, 0)
        pipe(fd);
        if ((pid = fork()) != 0) {
            

            close(fd[0]);
            dup2(open(command[i + 1], O_RDONLY), fd[1]);
            sem_post(p);
            wait(&stat);
            freeTokens(command);
            print("return %d\n", WEXITSTATUS(stat));
        } else {
            sem_wait(p);
            close(fd[1]);
            //将fd的输入重定向到STDIN
            dup2(fd[0], STDIN_FIFENO);
            // 清除不必要的部分： < 和后面打开的文件名
            execute(command, path, envp);
            freeTokens(command[i]);

        }

        
        if (error) {
            printf("errorMsg\n");
            continue;
        } else {
            tmp =  malloc(MAXSTR);
            strncpy(tmp, line, MAXSTR);
            addToCommandHistory(tmp, getNum() +1);
        }
        prompt();
   }
   saveCommandHistory();
   cleanCommandHistory();
   printf("\n");
   return(EXIT_SUCCESS);
}

// fileNameExpand: expand any wildcards in command-line args
// - returns a possibly larger set of tokens
char **append_char(char **res, char *new){
    for (int i = 0; res[i] != NULL; i++);
    char *tmp = malloc(strlen(new))
    strcpy(tmp, new)
    res[i] = tmp;
    res[i+1] = NULL;
    return res;
}
char **fileNameExpand(char **tokens)
{
    char **tmp = malloc(80);
    tmp[0] = NULL;
    for (int i = 0; tokens[i] != NULL; i++){
        glob_t gbuffer;
        glob(tokens[i], GLOB_NOCHECK|GLOB_TILDE, NULL, gbuffer);
        for (int i = 0; i < gbuffer.g1_pathc, i++){
            tmp = append_char(tmp,  gbuffer.g1_pathv[i]);
        }
        globfree(gbuffer);
    }
    
   return NULL;
}

// execute: run a program, given command-line args, path and envp
void execute(char **args, char **path, char **envp) {
    int found = 0;
    char p[BUFSIZ] = {'\0'};

    switch (args[0][0]) {
        case '.':
        case '/':
            if (isExecutable(args[0])) {
                found = 1;
            }
            break;
        default:
            while (*path) {
                snprintf(p, sizeof(p), "%s/%s", *path, args[0]);
                if (isExecutable(p)) {
                    args[0] = p;
                    found = 1;
                    break;
                }
                path++;
            }
    };

    if (found) {
        printf("Executing %s\n", args[0]);
        execve(args[0], args, envp);
        perror("Exec failed\n");
    } else {
        printf("Command not found\n");
    }

}

// isExecutable: check whether this process can execute a file
int isExecutable(char *cmd) {
    struct stat s;
    // must be accessible
    if (stat(cmd, &s) < 0)
    return 0;
    // must be a regular file
    //if (!(s.st_mode & S_IFREG))
    if (!S_ISREG(s.st_mode))
        return 0;
    // if it's owner executable by us, ok
    if (s.st_uid == getuid() && s.st_mode & S_IXUSR)
        return 1;
    // if it's group executable by us, ok
    if (s.st_gid == getgid() && s.st_mode & S_IXGRP)
        return 1;
    // if it's other executable by us, ok
    if (s.st_mode & S_IXOTH)
        return 1;
    return 0;
}
// findExecutable: look for executable in PATH
char *findExecutable(char *cmd, char **path)
{
      char executable[MAXLINE];
      executable[0] = '\0';
      if (cmd[0] == '/' || cmd[0] == '.') {
         strcpy(executable, cmd);
         if (!isExecutable(executable))
            executable[0] = '\0';
      }
      else {
         int i;
         for (i = 0; path[i] != NULL; i++) {
            sprintf(executable, "%s/%s", path[i], cmd);
            if (isExecutable(executable)) break;
         }
         if (path[i] == NULL) executable[0] = '\0';
      }
      if (executable[0] == '\0')
         return NULL;
      else
         return strdup(executable);
}

// isExecutable: check whether this process can execute a file
int isExecutable(char *cmd)
{
   struct stat s;
   // must be accessible
   if (stat(cmd, &s) < 0)
      return 0;
   // must be a regular file
   //if (!(s.st_mode & S_IFREG))
   if (!S_ISREG(s.st_mode))
      return 0;
   // if it's owner executable by us, ok
   if (s.st_uid == getuid() && s.st_mode & S_IXUSR)
      return 1;
   // if it's group executable by us, ok
   if (s.st_gid == getgid() && s.st_mode & S_IXGRP)
      return 1;
   // if it's other executable by us, ok
   if (s.st_mode & S_IXOTH)
      return 1;
   return 0;
}

// tokenise: split a string around a set of separators
// create an array of separate strings
// final array element contains NULL
char **tokenise(char *str, char *sep)
{
   // temp copy of string, because strtok() mangles it
   char *tmp;
   // count tokens
   tmp = strdup(str);
   int n = 0;
   strtok(tmp, sep); n++;
   while (strtok(NULL, sep) != NULL) n++;
   free(tmp);
   // allocate array for argv strings
   char **strings = malloc((n+1)*sizeof(char *));
   assert(strings != NULL);
   // now tokenise and fill array
   tmp = strdup(str);
   char *next; int i = 0;
   next = strtok(tmp, sep);
   strings[i++] = strdup(next);
   while ((next = strtok(NULL,sep)) != NULL)
      strings[i++] = strdup(next);
   strings[i] = NULL;
   free(tmp);
   return strings;
}

// freeTokens: free memory associated with array of tokens
void freeTokens(char **toks)
{
   for (int i = 0; toks[i] != NULL; i++)
      free(toks[i]);
   free(toks);
}

// trim: remove leading/trailing spaces from a string
void trim(char *str)
{
   int first, last;
   first = 0;
   while (isspace(str[first])) first++;
   last  = strlen(str)-1;
   while (isspace(str[last])) last--;
   int i, j = 0;
   for (i = first; i <= last; i++) str[j++] = str[i];
   str[j] = '\0';
}

// strContains: does the first string contain any char from 2nd string?
int strContains(char *str, char *chars)
{
   for (char *s = str; *s != '\0'; s++) {
      for (char *c = chars; *c != '\0'; c++) {
         if (*s == *c) return 1;
      }
   }
   return 0;
}

// prompt: print a shell prompt
// done as a function to allow switching to $PS1
void prompt(void)
{
   printf("mymysh$ ");
}
