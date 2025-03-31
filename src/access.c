#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/commands.h"
#include "headers/structures.h"
#include "headers/app.h"

int main(int argc, char** argv) 
{
	struct profile_state* profile_state = init();

	/* Begin Reading commands */
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
		char text_value[100] = { '\0' };
		char* values[4] = { NULL };
		if(!strcmp(cmmd, "writecomments")) {
			values[0] = strtok(NULL, " ");
			char* token = strtok(NULL, " ");
			strcat(text_value, token);
			token = strtok(NULL, " ");
			while(token != NULL) {
				strcat(text_value, " ");
				strcat(text_value, token);
				
				token = strtok(NULL, " ");
			}
		} else {
			for(int i = 0; i < 4; i++) {
				values[i] = strtok(NULL, " ");
			}
		}

		/* Execute command */
		if(!strcmp(cmmd, "friendadd")) {
			friendadd("friends.txt", values[0], profile_state);
		} else if (!strcmp(cmmd, "viewby")) {
			viewby("friends.txt", values[0], profile_state);
		} else if(!strcmp(cmmd, "end")) {
			return end(profile_state);
		} else if (profile_state->current_viewer != NULL) {
			if(!strcmp(cmmd, "logout")) {
				logout(profile_state);
			} else if(!strcmp(cmmd, "listadd")) {
				listadd(profile_state, values[0]);
			} else if(!strcmp(cmmd, "friendlist")) {
				friendlist(profile_state, values[0], values[1]);
			} else if(!strcmp(cmmd, "postpicture")) {
				postpicture(profile_state, values[0]);
			} else if(!strcmp(cmmd, "chlst")) {
				chlst(profile_state, values[0], values[1]);
			} else if(!strcmp(cmmd, "chmod")) { 
				chmod(profile_state, values[0], values[1], values[2], values[3]); 
			} else if(!strcmp(cmmd, "chown")) {
				chown(profile_state, values[0], values[1]);
			} else if (!strcmp(cmmd, "readcomments")) {
				readcomments(profile_state, values[0]);
			} else if (!strcmp(cmmd, "writecomments")) {
				writecomments(profile_state, values[0], text_value);
			}
		} else {
			printf("ERROR: A user must be viewing the profile in order to execute a command.");
		}

		memset(command, 0, COMM_SIZE * sizeof(command[0]));
	}

	return 1;
}
