#include<headers.h>

void changeInput(char *token, char *file) {
    if (strcmp(token, ">") == 0) {
        // change stdout
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
            perror("cannot redirect output");
        } else {
            close(STDOUT_FILENO);
            dup(new_fd);
            close(new_fd);
            //printf("redirecting %d to another file", t);
        }
    } else if (strcmp(token, ">>") == 0) {
        // change stdout
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1) {
            perror("cannot redire1ct output");
        } else {
            close(STDOUT_FILENO);
            dup(new_fd);
            close(new_fd);
            //printf("redirecting %d to another file", t);
        }
    } else if (strcmp(token, "<") == 0) {
        // change in
        int new_fd;
        //printf("redirecting stdin to another file");
        if ((new_fd = open(file, O_RDONLY)) == -1) {
            perror("cannot redirect input");
        } else {
            close(STDIN_FILENO);
            dup(new_fd);
            close(new_fd);
            //printf("redirecting %d to another file", t);
        }
    }

}

void redirectionHandler(char* input , int bg ){
char *tokens[1000];
    int num_tokens = 0;
    char input2[buffer];
    strcpy(input2, input);
    tokens[0] = strtok(input, " \t\n");
    while (tokens[num_tokens] != NULL) {
        tokens[++num_tokens] = strtok(NULL, " \t");
    }
    if (num_tokens == 0) {
        return;
    }
    char tokens_append[100][1000];
    int n = 0;
    for (int i = 0; i < num_tokens; i++) {
        char *new_tokens[100];
        int c = tokenize(">>", tokens[i], new_tokens);
        for (int j = 0; j < c; j++) {
            if (strcmp(new_tokens[j], "") != 0)
                strcpy(tokens_append[n++], new_tokens[j]);
            free(new_tokens[j]);
        }
    }
    // >
    char tokens_append_out[100][1000];
    num_tokens = n;
    n = 0;
    for (int i = 0; i < num_tokens; i++) {
        //printf("-token : %s-\n", tokens_append[i]);
        char *new_tokens[100];
        if (strcmp(tokens_append[i], ">>") == 0) {
            strcpy(tokens_append_out[n++], tokens_append[i]);
            continue;
        }
        int c = tokenize(">", tokens_append[i], new_tokens);
        for (int j = 0; j < c; j++) {
            if (strcmp(new_tokens[j], "") != 0) {
                strcpy(tokens_append_out[n++], new_tokens[j]);
            }
            free(new_tokens[j]);

        }
    }
    // <
    char tokens_final[100][1000];
    num_tokens = n;
    n = 0;
    for (int i = 0; i < num_tokens; i++) {
        //printf("-token : %s-\n", tokens_append_out[i]);
        char *new_tokens[100];
        int c = tokenize("<", tokens_append_out[i], new_tokens);
        for (int j = 0; j < c; j++) {
            if (strcmp(new_tokens[j], "") != 0)
                strcpy(tokens_final[n++], new_tokens[j]);
            free(new_tokens[j]);
        }

    }
    //now we need to do the redirection
    //int backup_stdout = dup(STDOUT_FILENO);
    //int backup_stdin = dup(STDIN_FILENO);
    char *command_tokens[1000];
    int num_word_command = 0;
    for (int i = 0; i < n; i++) {
        char *word = tokens_final[i];
        //fprintf(stderr, "--%s--\n", word);
        if (strcmp(word, ">") == 0 || strcmp(word, ">>") == 0 || strcmp(word, "<") == 0) {
            if (i + 1 == n || tokens_final[i + 1] == NULL) {
                fprintf(stderr, "unexpected token after %s \n", word);
                return;
            }
            changeInput(word, tokens_final[i + 1]);
            i++;
        } else {
            command_tokens[num_word_command] = malloc(buffer);
            strcpy(command_tokens[num_word_command], tokens_final[i]);
            num_word_command++;
        }
    }
    run_command(command_tokens, num_word_command, bg, input2);
    for (int i = 0; i < num_word_command; i++)
        free(command_tokens[i]);
}

void pipeChecker(char* cmd , int bg){
    int pipee = 0;
    for (int i = 0; i < strlen(cmd); i++)
        if (cmd[i] == '|')
            pipee++;
    if (pipee == 0) {
        redirectionHandler(cmd, bg);
        return;
    } else if (cmd[0] == '|' || cmd[strlen(cmd) - 1] == '|') {
        fprintf(stderr, "Pipe does not have both ends \n");
        return;
    }
    char *commands[1000];
    int n = 0;
    char *t = strtok(cmd, "|");
    while (t != NULL) {
        commands[n] = malloc(buffer);
        strcpy(commands[n], t);
        t = strtok(NULL, "|");
        //printf("-%s-\n", commands[n]);
        n++;
    }


    int out = dup(1);
    int in = dup(0);
    int prev_open = -1;
    for (int i = 0; i < n - 1; i++) {
        int pipes[2];
        if (pipe(pipes) == -1) {
            //perror("cannot open pipe");
            return;
        }
        if (prev_open != -1) {
            dup2(prev_open, 0);
            //perror("connected prev input pipe");
            close(prev_open);
            //perror("closed prev input pipe");
        }
        dup2(pipes[1], 1);
        //perror("connected new output pipe");
        close(pipes[1]);
        //perror("connected output pipe old fd");
        redirectionHandler(commands[i], bg);
        prev_open = pipes[0];
        free(commands[i]);
    }
    dup2(out, 1);
    //perror("connected output to stdout");
    close(out);
    if (prev_open != -1) {
        dup2(prev_open, 0);
        //perror("connected prev input pipe");
        close(prev_open);
    }
    redirectionHandler(commands[n - 1], bg);
    dup2(in, 0);
    close(in);
}
