#include <stdio.h>
#include <string.h>

#include "structures.h"

int friendadd(char* friends_path, char* friend_name, struct profile_state* state);
int viewby(char* friends_path, char* friend_name, struct profile_state* state);
int logout(struct profile_state* state);
