// Bash made by Vansh Marda
// 2021101089
// IIIT H - CSE

#include "headers.h"

char *username()
{
    char login_name[buffer];
    char host_name[buffer];
    // vansh@vansh-X550CL:~ $
    //  getlogin_r -> vansh
    if (getlogin_r(login_name, buffer) != 0)
    {
        perror("Cant get login name");
    }
    if (gethostname(host_name, buffer) != 0)
    {
        perror("Cant get hostname");
    }
    char *final_name = (char *)malloc(500);
    strcpy(final_name, login_name);
    strcat(final_name, "@");
    strcat(final_name, host_name);
    strcat(final_name, ":");
    return final_name;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int min(int a, int b)
{
    if (b > a)
    {
        return b;
    }
    else
    {
        return a;
    }
}

void process_adding(char *tokens[], int num, int background)
{
    char *command = strdup(tokens[0]);
    char *arr[num + 1];
    for (int i = 0; i < num; i++)
    {
        arr[i] = strdup(tokens[i]);
    }
    arr[num] = NULL;
    int forking = fork();
    if (forking < 0)
    {
        perror("creating child process failed\n");
    }
    else if (forking == 0)
    {
        if (background)
        {

            setpgid(0, 0);
        }
        if (execvp(command, arr) == -1)
        {
            printf("invalid command\n");
            exit(1);
        }
    }
    else if (forking > 0)
    {
        if (!background)
        {
            waitpid(forking, NULL, 0);
        }
        else
        {
            printf("child with pid [%d] sent to background\n", forking);
        }
    }
}

void absolute_relativer_addressing(char *new_address, char *cd_location, const char *curr_dir, const char *home_dir)
{
    if (cd_location[0] == '/')
    {
        strcpy(new_address, cd_location);
        // printf("1 ... %s\n",new_address);
    }
    else if (cd_location[0] == '~')
    {
        strcpy(new_address, home_dir);
        strcat(new_address, cd_location + 1);
        // printf("2 ... %s \n",new_address);
    }
    else
    {
        strcpy(new_address, curr_dir);
        if (new_address[strlen(new_address) - 1] != '/')
            strcat(new_address, "/");
        strcat(new_address, cd_location);
        // printf("3 ... %s \n",new_address);
    }
}

void printInitials(char *shellName, char *showDir)
{
    printf("%s", "\033[1m\033[36m"); // print cyan color
    printf("%s", shellName);
    printf("%s", "\033[1m\033[32m"); // print green color
    printf("%s ", showDir);
    printf("%s", "\033[1m\033[33m"); // print yellow color
    printf("$ ");
    printf("%s", "\033[1m\033[0m"); // reset color
}

char *remove_whitespace(char *line)
{
    // Calculating length of the array
    //  len = sizeof(str)/sizeof(str[0]);

    // Checks for space character in array if its there then ignores it and swap str[i] to str[i+1];
    int t = 0;
    int i = 0;
    // for(i = 0; i < len; i++){
    //     if(str[i] == ' '){
    //         for(j=i;j<len;j++)
    //     {
    //         str[j]=str[j+1];
    //     }
    //     len--;
    //     }
    // }

    while (i < strlen(line))
    {
        if (line[i] == '\n' || line[i] == '\t' || line[i] == ' ')
        {
            t++;
        }
        else
        {
            break;
        }
        i++;
    }
    i = 0;
    while (i < t)
    {
        line++;
        i++;
    }
    i = strlen(line) - 1;

    while (i >= 0)
    {
        if (line[i] == '\n' || line[i] == '\t' || line[i] == ' ')
        {
            line[i] = '\0';
        }
        else
        {
            break;
        }
        i--;
    }
    // printf(%s", line);
    return line;
}