#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "structures.h"
#include "acl_actions.h"
#include "app.h"

int friendadd(char* friends_path, char* friend_name, struct profile_state* state);
int viewby(char* friends_path, char* friend_name, struct profile_state* state);
int logout(struct profile_state* state);
int listadd(struct profile_state* state, char* list_name);
int friendlist(struct profile_state* state, char* username, char* list_name);
int postpicture(struct profile_state* state, char* filename);
int chlst(struct profile_state* state, char* file_name, char* list_name);
int chmod(struct profile_state* state, char* filename, char* owner_perm, char* group_perm, char* general_perm);
int chown(struct profile_state* state, char* filename, char* friendname);
void readcomments(struct profile_state* state, char* filename);
int writecomments(struct profile_state* state, char* filename, char* text);
int end(struct profile_state* state);
