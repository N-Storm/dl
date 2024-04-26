/* Part of the DigiLivolo control software.
 * https://github.com/N-Storm/DigiLivolo/ */

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "digilivolo.h"

#include <hidapi.h>
#include "usb_func.h"

#include "git_version.h"
#include "args.h"

// [argp] Program documentation.
// const char* argp_program_version = GIT_VERSION;
const char prognamever[] = "digilivolo " GIT_VERSION;
const char doc[] = "\nSoftware to control DigiLivolo devices.\n";
const char* argp_program_bug_address = "https://github.com/N-Storm/DigiLivolo/";

// [argp] A description of the arguments we accept.
char args_doc[] = "REMOTE_ID KEY_ID";

// [argp] The options we understand.
struct argp_option options[] = {
  {0,             0,   0,                            0, "Positional arguments:"      },
  {"REMOTE_ID",   0,   0, OPTION_DOC | OPTION_NO_USAGE, "Livilo Remote ID (1-65535)" },
  {"KEY_ID",      0,   0, OPTION_DOC | OPTION_NO_USAGE, "Livilo Key ID (1-255)"      },
  {0,             0,   0,                            0, "Options:"                   },
  {"verbose",   'v',   0,                            0, "Produce verbose output"     },
  { 0 }
};

// [argp] Command-line arguments.
arguments_t arguments;

// [argp] Parse a single option.
error_t parse_opt(int key, char* arg, struct argp_state* state)
{
	/* Get the input argument from argp_parse, which we
	 * know is a pointer to our arguments structure. */
	struct arguments* arguments = state->input;

	switch (key) {
	case 'v':
		arguments->verbose = true;
		break;

	case ARGP_KEY_ARG:
		if (state->arg_num >= 2)
			// Too many arguments.
			argp_usage(state);

		char* endptr;
		// Convert argument to long
		long value = strtol(arg, &endptr, 0);
		// Check if it was valid long value
		if (*endptr == '\0') {
			switch (state->arg_num) {
			case 0:
				// Out of range
				if (value > 65535 || value <= 0)
					argp_usage(state);
				else
					arguments->remote_id = (uint16_t)value;
				break;

			case 1:
				// Out of range
				if (value > 255 || value <= 0)
					argp_usage(state);
				else
					arguments->key_id = (uint8_t)value;
				break;

			default:
				return ARGP_ERR_UNKNOWN;
			}
		}
		else
			// REMOTE_ID or KEY_ID not an unsigned integer
			argp_usage(state);

		break;

	case ARGP_KEY_END:
		if (state->arg_num < 2)
			// Not enough arguments.
			argp_usage(state);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
