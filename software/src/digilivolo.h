/* Part of the DigiLivolo control software.
 * https://github.com/N-Storm/DigiLivolo/ */

#ifndef __digilivolo_h__
#define __digilivolo_h__

#include <wchar.h>

#define DIGILIVOLO_VID 0x16c0
#define DIGILIVOLO_PID 0x05df
#define DIGILIVOLO_MANUFACTURER_STRING L"digilivolo@yandex.com"
#define DIGILIVOLO_PRODUCT_STRING L"DigiLivolo"

#define REPORT_ID 0x4c

#define CMD_SWITCH 0x01 // IN,OUT send Livolo keycode command or send ACK to the host
#define CMD_RDY 0x10 // OUT, device ready command

#endif // __digilivolo_h__