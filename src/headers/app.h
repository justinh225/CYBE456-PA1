#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structures.h"

#define AUDIT_SIZE 200

struct profile_state* init();
int destroy(struct profile_state* state);
void audit_log(char* message);
