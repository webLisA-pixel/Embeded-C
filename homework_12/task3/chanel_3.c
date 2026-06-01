#include "chanel_3.h"

int main() {
    char *input;
	char *tmp_input;
    char **args;
    int background;

    while (1) {
        // Get current working directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        // Display the prompt
        printf("%s@process_3:%s> ", getenv("USER"), cwd);

        // Read input
        input = read_input();
        if (strlen(input) == 0) continue;

        // Add command to history
        add_history(input);

        // Parse input
		args = parse_input(input);

        // Handle "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }
		
        // Handle "cd" command
        if (strcmp(args[0], "cd") == 0) {
            cd(args);
        }

        // Handle "history" command
        else if (strcmp(args[0], "history") == 0) {
            print_history();
        }

        // Execute other commands
        else {
            execute(args);
        }

        // Free memory
        free(input);
        free(args);
    }

    return 0;
}