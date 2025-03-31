#ifndef STRUCTURES_H
#define STRUCTURES_H

#define COMM_SIZE 100
#define NAME_SIZE 50
#define PERM_SIZE 3
#define MAX_CMMD_SIZE 

struct profile_state {
	char owner_name[NAME_SIZE];
	char current_viewer[NAME_SIZE];
	struct friend_lists_list* friends_lists;
	struct image* images;
};

struct friend_lists_list {
	struct friend_lists_list* next;
	struct friend_list_node* friend_list;
	char name[NAME_SIZE];
};

struct friend_list_node {
	struct friend_list_node* next;
	char username[NAME_SIZE];
};

struct image {
	char filename[NAME_SIZE];
	char owner_name[NAME_SIZE];
	char owner_perm[PERM_SIZE];
	char list_perm[PERM_SIZE];
	char general_perm[PERM_SIZE];
	char list_name[NAME_SIZE];
	struct image* next;
};

#endif

