/* Part of the DigiLivolo control software.
 * https://github.com/N-Storm/DigiLivolo/ */

#ifndef __args_h__
#define __args_h__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "digilivolo.h"

#include "git_version.h"
#include <argp.h>

// [argp] A description of the arguments we accept.
extern char args_doc[];

// [argp] The options we understand.
extern struct argp_option options[];

// [argp] Program documentation.
// const char* argp_program_version = GIT_VERSION;
extern const char prognamever[];
extern const char doc[];
extern const char* argp_program_bug_address;

// [argp] Command-line arguments.
typedef struct arguments {
    uint16_t remote_id;
    uint8_t key_id;
    bool verbose;
} arguments_t;

extern arguments_t arguments;

extern error_t parse_opt(int key, char* arg, struct argp_state* state);

#endif // __args_h__