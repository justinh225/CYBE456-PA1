#include "headers/app.h"

char* friends_path = "friends.txt";
char* lists_path = "lists.txt";
char* pictures_path = "pictures.txt";
char* audit_path = "audit.txt";

struct profile_state* init() {

	 /* Setup files */
        FILE* friends_file = fopen("friends.txt", "w+");
        fclose(friends_file);
        FILE* lists_file = fopen("lists.txt", "w+");
	fclose(lists_file);
        FILE* pictures_file = fopen("pictures.txt", "w+");
	fclose(pictures_file);
        FILE* audit_file = fopen("audit.txt", "w+");
	fclose(audit_file);

        /* Setup profile state */
        struct profile_state* profile_state = malloc(sizeof(struct profile_state));
        memset(profile_state->owner_name, 0, NAME_SIZE * sizeof(char));
        memset(profile_state->current_viewer, 0, NAME_SIZE * sizeof(char));
        profile_state->friends_lists = NULL;
        profile_state->images = NULL;

	return profile_state;
}

int destroy(struct profile_state* state) {
	
	/* Log and remove lists */
	FILE* lists_file = fopen(lists_path, "a");
	struct friend_lists_list* curr_list = state->friends_lists;
	while(curr_list != NULL) {
		fprintf(lists_file, "%s:", curr_list->name);

		struct friend_list_node* curr_node = curr_list->friend_list;
		while(curr_node != NULL) {
			fprintf(lists_file, " %s", curr_node->username);

			struct friend_list_node* temp_node = curr_node;
			curr_node = curr_node->next;
			free(temp_node);
		}
		fprintf(lists_file, "\n");

		struct friend_lists_list* temp_list = curr_list;
		curr_list = curr_list->next;
		free(temp_list);
	}

	/* Log and remove pictures */
	FILE* pictures_file = fopen(pictures_path, "a");
	struct image* curr = state->images;
	while(curr != NULL) {
		fprintf(pictures_file, "%s: %s %s %s %s %s\n", curr->filename, curr->owner_name, curr->list_name, curr->owner_perm, curr->list_perm, curr->general_perm);

		struct image* temp = curr->next;
		free(curr);
		curr = temp;
	}

	free(state);

	return 0;
}

void audit_log(char* message) {
	FILE* audit_file = fopen(audit_path, "a");
	
	printf("%s\n", message);
	fprintf(audit_file, "%s\n", message);

	fclose(audit_file);
}
