/* Part of the DigiLivolo control software.
 * https://github.com/N-Storm/DigiLivolo/ */

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "digilivolo.h"
#include "args.h"

#include <hidapi.h>
#include "usb_func.h"

// [argp] Our argp parser.
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[9];
	hid_device* handle = NULL;
	int i;

	struct hid_device_info *devices, *dl_dev;

	// [argp] Default values.
	arguments.remote_id = 0;
	arguments.key_id = 0;
	arguments.verbose = false;

	// Print program name & version
	printf("%s\n", prognamever);

	/* Parse our arguments; every option seen by parse_opt will
	 * be reflected in arguments. */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		printf("Compiled with hidapi version %s, runtime version %s.\n", HID_API_VERSION_STR, hid_version_str());
		printf("Arguments: REMOTE_ID = %d, KEY_ID = %d\n", arguments.remote_id, arguments.key_id);
	}

	if (hid_init())
		return -1;

	#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
	// To work properly needs to be called before hid_open/hid_open_path after hid_init.
	// Best/recommended option - call it right after hid_init.
	hid_darwin_set_open_exclusive(0);
	#endif

	devices = hid_enumerate(DIGILIVOLO_VID, DIGILIVOLO_PID);
	dl_dev = find_digilivolo(devices);
	if (!dl_dev) {
		printf("ERROR: unable to find device\n");
    	if (arguments.verbose) {
			printf("All devices with matching VID & PID:\n");
			print_devices(devices);
		}
	}
	else {
		if (arguments.verbose) {
			printf("Device found: ");
			print_device(dl_dev);
			printf("Opening device path: %s\n", dl_dev->path);
		}
		handle = hid_open_path(dl_dev->path);
	}

	hid_free_enumeration(devices);

	// Set up the command buffer.
	memset(buf, 0x00, sizeof(buf));

	// Open the device using VID & PID
	if (!handle) {
		printf("ERROR: unable to open device\n");
		hid_exit();
		return 1;
	}

	struct hid_device_info* info = hid_get_device_info(handle);
	if (info == NULL) {
		printf("ERROR: Unable to get device info\n");
		hid_close(handle);
		hid_exit();
		return 1;
	}
	else {
		// print_device(info);
	}

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	// Send a Feature Report to the device
	buf[0] = REPORT_ID;
	buf[1] = CMD_SWITCH;
	buf[2] = arguments.remote_id & 0xFF;
	buf[3] = (arguments.remote_id >> 8) & 0xFF;
	buf[4] = arguments.key_id;
	res = hid_send_feature_report(handle, buf, 8);
	if (res < 0) {
		printf("ERROR: Unable to send a feature report.\n");
		hid_close(handle);
		hid_exit();
		return 1;
	}
	else
		printf("Codes sent to device.\n");

	memset(buf, 0, sizeof(buf));

	// Read a Feature Report from the device
	buf[0] = REPORT_ID;
	res = hid_get_feature_report(handle, buf, 8);
	if (res < 0) {
		printf("WARN: Unable to get a feature report: %ls\n", hid_error(handle));
	}
	else if (arguments.verbose) {
		// Print out the returned buffer.
		printf("Feature Report: ");
		for (i = 0; i < res; i++)
			printf("%02x ", (unsigned int)buf[i]);
		printf("\n");
	}

	memset(buf, 0, sizeof(buf));

	hid_close(handle);

	/* Free static HIDAPI objects. */
	hid_exit();

	return 0;
}
