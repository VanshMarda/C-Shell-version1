// Bash made by Vansh Marda
// 2021101089
// IIIT H - CSE

#include "headers.h"

char initialDirector[buffer];
char workingDirectory[buffer];
char showingDirectory[buffer];

void pinfo_handler(char *tok[])
{
    char add[buffer];
    char exe[buffer];
    strcpy(exe, "defunct :");
    sprintf(add, "/proc/%s/stat", tok[1]);
    FILE *ptr = fopen(add, "r");
    if (ptr == NULL)
    {
        printf("Cannot access the process \n");
        return;
    }
    // char hostbuffer[256];
    // char *IPbuffer;
    // struct hostent *host_entry;
    // int hostname;

    // // To retrieve hostname
    // hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    // if (hostentry == NULL)
    // {
    //     perror("gethostbyname");
    //     exit(1);
    // }
    int pid;
    char exec_name[1000];
    char st;
    fscanf(ptr, "%d ", &pid);
    fscanf(ptr, "%s ", exec_name);
    fscanf(ptr, "%c ", &st);
    sprintf(add, "/proc/%s/exe", tok[1]);
    int t = readlink(add, exe, buffer);
    if (t != -1)
    {
        exe[t] = '\0';
    }
    else
    {
        strcat(exe, exec_name);
    }
    sprintf(add, "/proc/%s/status", tok[1]);
    fclose(ptr);
    // // To retrieve host information
    // host_entry = gethostbyname(hostbuffer);
    // if (hostentry == NULL)
    // {
    //     perror("gethostbyname");
    //     exit(1);
    // };

    // // To convert an Internet network
    // // address into ASCII string
    // IPbuffer = inet_ntoa(*((struct in_addr*)
    //                        host_entry->h_addr_list[0]));

    // printf("Hostname: %s\n", hostbuffer);
    // printf("Host IP: %s", IPbuffer);
    ptr = fopen(add, "r");
    if (ptr == NULL)
    {
        printf("Cannot access the process \n");
        return;
    }
    char word[buffer];
    while (fscanf(ptr, " %499s", word) == 1)
    {
        if (strcmp(word, "VmSize:") == 0)
        {
            fscanf(ptr, " %499s", word);
            break;
        }
    }
    fclose(ptr);
    printf("pid : %d\n", pid);
    printf("Process Status : %c\n", st);
    printf("memory : %s\n", word);
    printf("Executable Path : %s\n", exe);
    printf("\n");
}


void TransformDir()
{
    int n1 = strlen(initialDirector);
    if (strlen(workingDirectory) < n1)
    {
        strcpy(showingDirectory, workingDirectory);
        if (showingDirectory[strlen(showingDirectory) - 1] == '/')
        {
            showingDirectory[strlen(showingDirectory) - 1] = '\0';
        }
        return;
    }
    int i = 0;
    while (i < n1)
    {
        if (initialDirector[i] != workingDirectory[i])
        {
            strcpy(showingDirectory, workingDirectory);
            return;
        }
        i++;
    }
    strcpy(showingDirectory, "~/");
    strcat(showingDirectory, workingDirectory + n1);
    int n2 = strlen(showingDirectory);
    if (showingDirectory[n2 - 1] == '/')
    {
        showingDirectory[n2 - 1] = '\0';
    }
}

// handles cd
void cd(char *token[])
{
    char cd_location[buffer];
    strcpy(cd_location, token[1]);
    char new_address[buffer];
    absolute_relativer_addressing(new_address, cd_location, workingDirectory, initialDirector);
    struct stat stats_dir;
    if (stat(new_address, &stats_dir) == 0 && (S_IFDIR & stats_dir.st_mode))
    {
        if (chdir(new_address) == -1 || getcwd(workingDirectory, buffer) == NULL)
        {
            printf("Directory does not exist  or getcwd failed\n");
        }
        strcat(workingDirectory, "/");
        TransformDir();
    }
    else
    {
        printf("Directory does not exist%s\n", token[1]);
    }
}

void processInput(char *input, int bg)
{
    char *tok[2000];
    int num_tok = 0;
    tok[0] = strtok(input, " \t\n");
    while (tok[num_tok] != NULL)
    {
        tok[++num_tok] = strtok(NULL, " \t");
    }
    if (num_tok == 0)
        return;
    if (strcmp(tok[0], "cd") == 0)
    {
        if (num_tok == 1)
        {
            tok[1] = malloc(4);
            strcpy(tok[1], "~");
        }
        cd(tok);
    }
    else if (strcmp(tok[0], "pwd") == 0)
    {
        printf("%s\n", workingDirectory);
    }
    else if (strcmp(tok[0], "echo") == 0)
    {
        echo(tok, num_tok);
    }
    else if (strcmp(tok[0], "clear") == 0)
    {
        printf("\e[1;1H\e[2J");
    }
    else if (strcmp(tok[0], "history") == 0)
    {
        if (num_tok == 1)
        {
            show_history(10);
        }
        else
        {
            if (strtol(tok[1], NULL, 10) <= 0)
            {
                if (strtol(tok[1], NULL, 10) > 20)
                {
                    printf("history <int n> \n n > 0 && n <= 20\n");
                    return;
                }
            }
            show_history(atoi(tok[1]));
        }
    }
    else if (strcmp(tok[0], "pinfo") == 0)
    {
        if (num_tok == 1)
        {
            tok[1] = malloc(10);
            sprintf(tok[1], "%d", getpid());
        }
        pinfo_handler(tok);
    }
    else if (strcmp(tok[0], "ls") == 0)
    {
        ls_handler(tok, num_tok, workingDirectory, initialDirector);
    }
    else if (strcmp(tok[0], "discover") == 0)
    {
        discover_handler(tok, num_tok, workingDirectory, initialDirector);
    }
    else if (strcmp(tok[0], "exit") == 0)
    {
        kill_background();
        printf("bye bye \n");
        _exit(0);
    }
    else
        process_adding(tok, num_tok, bg);
}

// void printInitials(char* shellName , char* showDir){
//     printf("%s", "\033[1m\033[36m"); //print cyan color
//     printf("%s", shellName);
//     printf("%s", "\033[1m\033[32m"); //print green color
//     printf("%s ", showDir);
//     printf("%s", "\033[1m\033[33m"); //print yellow color
//     printf("$ ");
//     printf("%s", "\033[1m\033[0m"); //reset color
// }

// separates commands by ;
void trim_whitespaceinput_commands(char *arr)
{
    char *command;
    command = strtok(arr, ";&");
    char arr2[buffer];
    char arr3[buffer];
    strcpy(arr2, arr);
    strcpy(arr3, arr);
    int c = 0;
    int x = strlen(command);
    int z=0;
    for(int i=0;i<x;i++){
        z++;
    }
    while (command != NULL)
    {
        c++;
        command = strtok(NULL, ";&");
    }
    if (c <= 0){
        return;
    }
    char *input_commands[c + 1];
    int i = 0;
    char *beg = arr2;
    input_commands[0] = strtok(arr2, ";&");

    while (input_commands[i] != NULL && strcmp(input_commands[i], "") != 0)
    {
        i++;
        input_commands[i] = strtok(NULL, ";&");
    }
    int j = 0;
    while (j < c)
    {
        bool bg = false;
        if (arr3[strlen(input_commands[j]) + (input_commands[j] - beg)] == '&')
        {
            bg = true;
        }
        int backup_stdout = dup(STDOUT_FILENO);
        int backup_stdin = dup(STDIN_FILENO);
        pipeChecker(input_commands[j], bg);
        // processInput(input_commands[j], bg);
        j++;
    }
}

void rip_child(int signum)
{
    if (signum == SIGCHLD)
        background_process_killer();
}

void exit_2(int signum)
{
    if (signum == SIGINT)
    {
        kill_background();
        _exit(0);
    }
}

int main()
{
    signal(SIGCHLD, rip_child);
    signal(SIGINT, exit_2);
    printf("\e[1;1H\e[2J");
    printf(" ---------------\n");
    printf("|   Welcome     |\n");
    printf("|     to        |\n");
    printf("| Vansh Marda's |\n");
    printf("|    Shell  ;)  |\n");
    printf(" ---------------\n");

    // vansh@vansh-X550CL:
    char *shellName;
    shellName = username();

    // Get the pathname of the current working directory
    //  home dir ---> /home/vansh/Desktop/osn/assignment/assignment2/
    if (getcwd(initialDirector, buffer) == NULL)
    {
        perror("Cannot get path to home dir failed");
    }
    strcat(initialDirector, "/");
    strcpy(workingDirectory, initialDirector);
    TransformDir();
    while (1)
    {
        printInitials(shellName, showingDirectory);
        char *arr = malloc(buffer);
        fgets(arr, buffer, stdin);
        int ln = strlen(arr) - 1;
        if (*arr)
        {
            if (arr[ln] == '\n')
            {
                arr[ln] = '\0';
            }
        }
        arr = remove_whitespace(arr);
        add_history(arr);
        input_commands(arr);
    }
}
