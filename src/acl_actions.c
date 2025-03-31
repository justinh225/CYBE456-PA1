#include "headers/acl_actions.h"

int verify_read(char* username, struct image* image, struct friend_lists_list* lists) {
	
	int is_owner = !strcmp(username, image->owner_name);
 	int is_group = 0;

	if(strcmp(image->list_name, "nil")) {
		while(lists != NULL) {
			if(!strcmp(image->list_name, lists->name))
				break;
			lists = lists->next;
		}

		struct friend_list_node* list_entry = lists->friend_list;
		while(list_entry != NULL) {
			if(!strcmp(list_entry->username, username)) {
				is_group = 1;
				break;
			}
			list_entry = list_entry->next;
		}
	}

	if(is_owner)
		return image->owner_perm[0] == 'r';
	else if(is_group)
		return image->list_perm[0] == 'r';

	return image->general_perm[0] == 'r';
}

int verify_write(char* username, struct image* image, struct friend_lists_list* lists) {
	int is_owner = !strcmp(username, image->owner_name);
        int is_group = 0;

        if(strcmp(image->list_name, "nil")) {
                while(lists != NULL) {
                        if(!strcmp(image->list_name, lists->name))
                                break;
                        lists = lists->next;
                }

                struct friend_list_node* list_entry = lists->friend_list;
                while(list_entry != NULL) {
                        if(!strcmp(list_entry->username, username)) {
                                is_group = 1;
                                break;
                        }
                        list_entry = list_entry->next;
                }
        }

        if(is_owner)
                return image->owner_perm[1] == 'w';
        else if(is_group)
                return image->list_perm[1] == 'w';

        return image->general_perm[1] == 'w';
}
