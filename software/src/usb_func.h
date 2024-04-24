/* Part of the DigiLivolo control software.
 * https://github.com/N-Storm/DigiLivolo/ */

#ifndef __usb_func_h__
#define __usb_func_h__

#include <wchar.h>
#include <hidapi.h>

extern const char* hid_bus_name(hid_bus_type bus_type);
extern void print_device_details(struct hid_device_info* cur_dev);
extern void print_device(struct hid_device_info* cur_dev);
extern void print_devices(struct hid_device_info* cur_dev);
extern struct hid_device_info* find_digilivolo(struct hid_device_info* cur_dev);

#endif // __usb_func_h__
