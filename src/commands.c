#include "headers/commands.h"

int friendadd(char* friends_path, char* friend_name, struct profile_state* state) {

	if(state->owner_name != NULL && state->current_viewer != NULL && strcmp(state->owner_name, state->current_viewer)) {
		printf("ERROR: Only a profile owner can add friends\n");
	}

	FILE* file_ptr = fopen(friends_path, "a");
	if(file_ptr == NULL) {
		printf("ERROR: Failed to open friends file.\n");
		return 1;
	}

	char name[50] = { 0 };
	while(fgets(name, 50, file_ptr) != NULL) {
		if(!strcmp(friend_name, name)) { 
			printf("ERROR: Friend with name \"friend_name\" already exists.\n");
			return 1;
		}
	}

	fprintf(file_ptr, "%s\n", friend_name);
	fclose(file_ptr);

	if(state->owner_name == NULL) {
		memset(state->owner_name, 0, NAME_SIZE * sizeof(char));
		strcpy(state->owner_name, friend_name);
	}

	return 0;
}


int viewby(char* friends_path, char* friend_name, struct profile_state* state) {

	if(state->current_viewer != NULL) {
		printf("ERROR: Cannot view profile, another user is already viewing this profile.\n");
		return 1;
	}

	FILE* file_ptr = fopen(friends_path, "r");
	if(file_ptr == NULL) {
		printf("ERROR: Failed to open friends file\n");
		return 1;
	}
	
	int valid = 1;
	char name[50] = { 0 };
	while(fgets(name, 50, file_ptr) != NULL) {
		name[strlen(name) - 1] = '\0';
		if(!strcmp(friend_name, name)) {
			state->current_viewer = friend_name;
			valid = 0;
		}			
	}

	fclose(file_ptr);
	
	return valid;
}

int logout(struct profile_state* state) {
	state->current_viewer = NULL;
}
