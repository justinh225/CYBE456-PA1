#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/commands.h"
#include "headers/structures.h"

char* friends_path = "friends.txt";

/* Main Program */
int main(int argc, char** argv) 
{
	/* Setup files */
	FILE* friends_file = fopen(friends_path, "w+");
	fclose(friends_file);
	FILE* lists_file = fopen("lists.txt", "w+");
	FILE* pictures_file = fopen("pictures.txt", "w+");
	FILE* audit_file = fopen("audit.txt", "w+");

	struct profile_state* profile_state = malloc(sizeof(struct profile_state));
	profile_state->owner_name = NULL;
	profile_state->current_viewer = NULL;

	/* Execute Commands */
	char* filename = argv[1]; 
	printf("Reading commands from file: \'%s\'\n", filename);

	FILE* comm_file = fopen(filename, "r");
	if(comm_file == NULL) {
		printf("Failed to read file.\n");
		return 1;
	}

	char command[COMM_SIZE] = { 0 };
	
	while(fgets(command, COMM_SIZE, comm_file) != NULL) {

		command[strlen(command) - 1] = '\0';
		
		char* cmmd = strtok(command, " ");
		char* value = strtok(NULL, " ");

		printf("Executing \'%s\'\n", command);

		if(!strcmp(cmmd, "friendadd")) {
			friendadd(friends_path, value, profile_state);
		} else if (!strcmp(cmmd, "viewby")) {
			
			if(viewby(friends_path, value, profile_state)) {
				printf("ERROR: Failed to authenticate friend. Use the \'friendadd\' command to add a new friend.\n");
				return 1;
			}
			printf("Current viewer is \'%s\'.\n", profile_state->current_viewer);
		} else if (profile_state->current_viewer != NULL) {

		} else {
			printf("ERROR: A user must be viewing the profile in order to execute a command.");
		}

		memset(command, 0, COMM_SIZE * sizeof(command[0]));
		printf("|%s|\n", profile_state->owner_name);
	}

	printf("Successfully read commands file.\n");
	
	/* Cleanup Files */
	fclose(lists_file);
	fclose(pictures_file);
	fclose(audit_file);

	return 0;
}
