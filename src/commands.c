#include "headers/commands.h"

int friendadd(char* friends_path, char* friend_name, struct profile_state* state) {

	if(state->owner_name[0] != '\0' && state->current_viewer[0] != '\0' && strcmp(state->owner_name, state->current_viewer)) {
		 audit_log("Error: only profile owner may issue friendadd command");
		return 1;
	}
	FILE* file_ptr = fopen(friends_path, "r");
	if(file_ptr == NULL) {
		audit_log("ERROR: Failed to open friends file.");
		return 1;
	}

	char name[50] = { 0 };
	while(fgets(name, 50, file_ptr) != NULL) {
		name[strlen(name) - 1] = '\0';
		if(!strcmp(friend_name, name)) {
			char error_message[AUDIT_SIZE] = { 0 };
			sprintf(error_message, "Error: Friend \"%s\" already exists", friend_name);
			audit_log(error_message);
			fclose(file_ptr);
			return 1;
		}
	}
	fclose(file_ptr);

	file_ptr = fopen(friends_path, "a");
	fprintf(file_ptr, "%s\n", friend_name);
	fclose(file_ptr);

	if(state->owner_name[0] == '\0') {
		memset(state->owner_name, 0, NAME_SIZE * sizeof(char));
		strcpy(state->owner_name, friend_name);
	}
	
	char message[AUDIT_SIZE] = { '\0' };
	sprintf(message, "Friend %s added", friend_name);

	audit_log(message);
	return 0;
}


int viewby(char* friends_path, char* friend_name, struct profile_state* state) {

	if(state->current_viewer[0] != '\0') {
		audit_log("View failed: Concurrent friends not supported.");
		return 1;
	}

	FILE* file_ptr = fopen(friends_path, "r");
	if(file_ptr == NULL) {
		audit_log("ERROR: Failed to open friends file\n");
		return 1;
	}
	
	int valid = 1;
	char name[50] = { 0 };
	while(fgets(name, 50, file_ptr) != NULL) {
		name[strlen(name) - 1] = '\0';
		if(!strcmp(friend_name, name)) {
			memset(state->current_viewer, 0, NAME_SIZE * sizeof(char));
			strcpy(state-> current_viewer, friend_name);
			
			char message[AUDIT_SIZE] = { 0 };
			sprintf(message, "User %s views the profile", state->current_viewer);
			audit_log(message);
			valid = 0;
		}			
	}
	fclose(file_ptr);
	
	return valid;
}

int logout(struct profile_state* state) {
	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "Friend %s logged out", state->current_viewer);
	
	memset(state->current_viewer, 0, NAME_SIZE * sizeof(char));

	audit_log(message);
}

int listadd(struct profile_state* state, char* list_name) {

	if(strcmp(state->owner_name, state->current_viewer)) {
		audit_log("Error: only profile owner may issue listadd command");
		return 1;
	}

	if(!strcmp(list_name, "nil")) {
		audit_log("ERROR: Friends list cannot have a name equal to \"nil\".");
		return 1;
	}

	struct friend_lists_list* new_list = malloc(sizeof(struct friend_lists_list));
	new_list->next =  NULL;
	new_list->friend_list = NULL;
	strcpy(new_list->name, list_name);

	char message[AUDIT_SIZE] = { 0 };
        sprintf(message, "List %s created", list_name);

	if(state->friends_lists == NULL) {
		state->friends_lists = new_list;
		audit_log(message);
		return 0;	
	}
	
	struct friend_lists_list* curr = state->friends_lists;
	struct friend_lists_list* prev = NULL;
	while(curr != NULL) {
		if(!strcmp(curr->name, list_name)) {
			char error_message[AUDIT_SIZE] = { 0 };
                	sprintf(error_message, "Error: list %s already exists", list_name);
			free(new_list);
			return 1;
		}

		prev = curr;
		curr = curr->next;
	}

	prev = new_list;

	audit_log(message);
	return 0;
}

int friendlist(struct profile_state* state, char* username, char* list_name) {
	if(strcmp(state->owner_name, state->current_viewer)) {
                audit_log("Error: only profile owner may issue friendlist command");
                return 1;
        }

	
	struct friend_lists_list* friends_list = state->friends_lists;
	while(friends_list != NULL) {
		if(!strcmp(friends_list->name, list_name))
			break;
		friends_list = friends_list->next;
	}
	if(friends_list == NULL) {
		audit_log("ERROR: Friends list with the name does not exist.");
		return 1;
	}	


	struct friend_list_node* new_node = malloc(sizeof(struct friend_list_node));
        new_node->next = NULL;
        strcpy(new_node->username, username);

	char message[AUDIT_SIZE] = { '\0' };
	sprintf(message, "Friend %s added to list %s", new_node->username, list_name);

	if(friends_list->friend_list == NULL) {
		friends_list->friend_list = new_node;
		audit_log(message);
		return 0;
	}

	struct friend_list_node* curr = friends_list->friend_list;
	struct friend_list_node* prev = NULL;
	while(curr != NULL) {
		if(!strcmp(curr->username, username)) {
			audit_log("Friend has already been added to the list.");
			return 1;
		}

		prev = curr;
		curr = curr->next;
	}
	prev->next = new_node;
	audit_log(message);
	return 0;
}

int postpicture(struct profile_state* state, char* filename) {
	
	if(state->current_viewer[0] == '\0') {
		audit_log("ERROR: User must be viewing this profile to post a picture.");
		return 1;
	}

	struct image* curr = state->images;
	while(curr != NULL) {
		if(!strcmp(curr->filename, filename)) {
			audit_log("Picture with filename already exists.");
			return 1;
		}
		curr = curr->next;
	}

	FILE* image_file = fopen(filename, "w");
	if(image_file == NULL) {
		audit_log("ERROR: Failed to create a new image file.");
		return 1;
	}
	fprintf(image_file, "%s\n", filename);
	fclose(image_file);

	struct image* new_image = malloc(sizeof(struct image));
	strcpy(new_image->filename, filename);
	strcpy(new_image->owner_name, state->current_viewer);
	strcpy(new_image->owner_perm, "rw");
	strcpy(new_image->list_perm, "--");
	strcpy(new_image->general_perm, "--");
	strcpy(new_image->list_name, "nil");
	new_image->next = NULL;

	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "Picture %s with owner %s and default permissions created.", new_image->filename, new_image->owner_name);

	curr = state->images;
	if(curr == NULL) {
		state->images = new_image;
		audit_log(message);
		return 0;
	}

	while(curr->next != NULL) 
		curr = curr->next;
	curr->next = new_image;
	
	audit_log(message);
	return 0;
}

int chlst(struct profile_state* state, char* file_name, char* list_name) {
	
	/* Check if the image exists */
	struct image* image = state->images;
	while(image != NULL) {
		if(!strcmp(image->filename, file_name))
			break;
		image = image->next;
	}
	if(image == NULL) {
		audit_log("ERROR: Failed to find image.");
		return 1;
	}

	/* Check if the user is authorized */
	if(strcmp(state->owner_name, state->current_viewer) && strcmp(image->owner_name, state->current_viewer)) {
		audit_log("ERROR: An images list can only be modified by the image or profile owner.");
		return 1;
	}

	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "List for %s set to %s by %s", image->filename, list_name, state->current_viewer);


	/* If nil, done */
	if(!strcmp(list_name, "nil")) {
		strcpy(image->list_name, list_name);
		audit_log(message);
		return 0;
	}

	/* Find the friends list and verify the viewer is in it */
	struct friend_lists_list* friend_list = state->friends_lists;
	while(friend_list != NULL) {
		if(!strcmp(friend_list->name, list_name))
			break;
		friend_list = friend_list->next;
	}
	if(friend_list == NULL) {
		audit_log("ERROR: Friend list was not found.");
		return 1;
	} else if(strcmp(state->owner_name, state->current_viewer)) {
		struct friend_list_node* curr = friend_list->friend_list;
		while(curr != NULL) {
			if(!strcmp(state->current_viewer, curr->username))
				break;
			curr = curr->next;
		}
		if(curr == NULL) 
		{
			char error_message[AUDIT_SIZE] = { 0 };
			sprintf(error_message, "Error with chlist: user %s is not a member in list %s", state->current_viewer, friend_list->name);
			audit_log(error_message);
			return 1;
		}
	}
	
	strcpy(image->list_name, list_name);
	audit_log(message);
	return 0;

}

int chmod(struct profile_state* state, char* filename, char* owner_perm, char* group_perm, char* general_perm) {

	/* Check if the image exists */
	struct image* image = state->images;
	while(image != NULL) {
		if(!strcmp(image->filename, filename))
			break;
		image = image->next;
	}                                                                               
	if(image == NULL) {
		char error_message[AUDIT_SIZE] = { 0 };	
		sprintf(error_message, "Error with chmod: picture %s not found", filename);

		audit_log(error_message);		
		return 1;
	}

	/* Check if the user is authorized */                             
	if(strcmp(state->owner_name, state->current_viewer) && strcmp(image->owner_name, state->current_viewer)) {
		char message[AUDIT_SIZE] = { 0 };
		sprintf(message, "Friend %s denied access to %s", state->current_viewer, filename);
                audit_log(message);
		return 1;
        }
	
	strcpy(image->owner_perm, owner_perm);
	strcpy(image->list_perm, group_perm);
	strcpy(image->general_perm, general_perm);

	char message_success[AUDIT_SIZE] = { 0 };
	sprintf(message_success, "Permissions for file %s set to %s %s %s by %s", filename, image->owner_perm, image->list_perm, image->general_perm, state->current_viewer);

	audit_log(message_success);
	return 0;
}

int chown(struct profile_state* state, char* filename, char* friendname) {

	if(strcmp(state->owner_name, state->current_viewer)) {
		audit_log("Error with chown: user unauthorized.");
		return 1;                                                	 
	 }

	/* Check if the image exists */
        struct image* image = state->images;
        while(image != NULL) {
                if(!strcmp(image->filename, filename))
                        break;
                image = image->next;
        }
        if(image == NULL) {
                char error_message[AUDIT_SIZE] = { 0 };                                                       sprintf(error_message, "Error with chmown: file %s not found", filename);
                
		audit_log(error_message);
		return 1;
	}

	strcpy(image->owner_name, friendname);
	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "Owner of %s changed to %s", filename, friendname);

	audit_log(message);
	return 0;
}

void readcomments(struct profile_state* state, char* filename) {
	
	/* Check if the image exists */
        struct image* image = state->images;
        while(image != NULL) {
                if(!strcmp(image->filename, filename))
                        break;
                image = image->next;
        }
	
	if(!verify_read(state->current_viewer, image, state->friends_lists)) {
		char error_message[AUDIT_SIZE] = { 0 };
		sprintf(error_message, "Friend %s denied read access to %s", state->current_viewer, filename);
		
		audit_log(error_message);	
		return;
	}

	FILE* image_file = fopen(filename, "r");
	if(image_file == NULL) { 
		audit_log("ERROR: Failed to open image file.");
		return;
	}

	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "Friend %s reads %s as:", state->current_viewer, filename);
	audit_log(message);

	char line[COMM_SIZE] = { 0 };
	while(fgets(line, COMM_SIZE, image_file) != NULL) {
		line[strlen(line) - 1] = '\0';
		audit_log(line);
	}

	fclose(image_file);
}

int writecomments(struct profile_state* state, char* filename, char* text) {

	/* Check if the image exists */
        struct image* image = state->images;
        while(image != NULL) {
                if(!strcmp(image->filename, filename))
                        break;
                image = image->next;
        }

        if(!verify_write(state->current_viewer, image, state->friends_lists)) {
        	char error_message[AUDIT_SIZE] = { 0 }; 
		sprintf(error_message, "Friend %s denied write access to %s", state->current_viewer, filename);
                audit_log(error_message);
		
		return 1;                                                                        }

	FILE* image_file = fopen(filename, "a");
        if(image_file == NULL) {            
		audit_log("Failed to open image file.");
                return 1;
        }

        fprintf(image_file, "%s\n", text);

	char message[AUDIT_SIZE] = { 0 };
	sprintf(message, "Friend %s wrote to %s: %s", state->current_viewer, filename, text);

	audit_log(message);
        fclose(image_file);
}

int end(struct profile_state* state) {
	return destroy(state);
}










