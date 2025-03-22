#include <stdio.h>
#include <string.h>

/* Main Program */
int main(int argc, char** argv) 
{
	/* Setup files */
	FILE* friends_file = fopen("friends.txt", "w+");
	FILE* lists_file = fopen("lists.txt", "w+");
	FILE* pictures_file = fopen("pictures.txt", "w+");
	FILE* audit_file = fopen("audit.txt", "w+");

	/* Execute Commands */
	char* filename = argv[1]; 
	printf("Reading commands from filename: %s\n", filename);

	FILE* comm_file = fopen(filename, "r");
	if(comm_file == NULL) {
		printf("Failed to read file.\n");
		return 1;
	}

	while(1) {
		char command[100] = { 0 };
		char* result = fgets(command, 100, comm_file);
		if(result == NULL)
			break;
		command[strlen(command) - 1] = '\0';

		printf("Executing \'%s\'\n", command);
	}

	printf("Successfully read commands file.\n");
	
	/* Cleanup Files */
	fclose(friends_file);
	fclose(lists_file);
	fclose(pictures_file);
	fclose(audit_file);

	return 0;
}
