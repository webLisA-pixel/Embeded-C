#include "chanel_3.h"

// Add a command to history
void add_history(char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(command);
    }
}

// Print command history
void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

// Read input command
char *read_input() {
    char *input = malloc(BUFFER_SIZE * sizeof(char));
    if (!input) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }
    fgets(input, BUFFER_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';
    return input;
}

// Parse input command into arguments
char **parse_input(char *input) {
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(input, " ");
    while (token != NULL) {
        tokens[position++] = token;
        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;
    return tokens;
}

// Execute 'cd' command
int cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("Error shell");
        }
    }
    return 1;
}

// Execute other command
void execute(char **args) {
    pid_t pid;
	int sep=0;
	int count=0;
	while(args[count]!=NULL)
	{
		if(strcmp(args[count], "|")==0)
		{
			if(sep==0)
				sep=count;
			else
			{
				perror("too few more separator");
				abort;
			}
		}
		count++;
	}
	if (sep!=0)
	{
		char **tmp_args_1=malloc(bufsize * sizeof(char*));
		char **tmp_args_2=malloc(bufsize * sizeof(char*));
		pid_t pid_2;
		int pipe_buf[2];
		for (int i=0; i<sep; i++)
			tmp_args_1[i]=args[i];
		for (int i=sep+1;i<count;i++)
			tmp_args_2[i]=args[i];
		pipe(pipe_buf);
		pid=fork();
		if (pid == 0)
		{
			// First child process
			dup2(pipe_buf[1], STDOUT_FILENO); // Redirect stdout to pipe write
			close(pipe_buf[0]); // Close unused read
			close(pipe_buf[1]);
	
			execvp(tmp_args_1[0], tmp_args_1); // Execute first command
			perror("Pipe cmd1 failed");
			exit(1);
		}
		pid_2 = fork();
		if (pid_2 == 0) 
		{
			// Second child process
			dup2(pipe_buf[0], STDIN_FILENO); // Redirect stdin to pipe read
			close(pipe_buf[1]); // Close unused write
			close(pipe_buf[0]);
	
			execvp(tmp_args_2[0], tmp_args_2); // Execute second command
			perror("Pipe cmd2 failed");
			exit(1);
		}
		close(pipe_buf[READ]);
		close(pipe_buf[WRITE]);
		wait(NULL);
		wait(NULL);
		free(tmp_args_1);
		free(tmp_args_2);
	}
	else
	{
		pid = fork();
		if (pid == 0) 
		{
			// Child process
			if (execvp(args[0], args) == -1) {
				perror("process_3");
				exit(1);
			}
			exit(0);
		} 
		else if(pid < 0) 
			perror("Don't create process");
		wait(0);
		exit(0);
	}
}