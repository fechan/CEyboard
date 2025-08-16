#include "descriptor.h"
#include "hid_spec.h"

#include <keypadc.h>
#include <string.h>
#include <usbdrvce.h>
#include <ti/screen.h>

static usb_error_t handleUsbEvent(usb_event_t event, void *eventData, usb_callback_data_t *callbackData) {
    usb_error_t error = USB_SUCCESS;

    if (event == USB_DEFAULT_SETUP_EVENT) {
        if (((usb_control_setup_t *)eventData)->bRequest == USB_GET_DESCRIPTOR_REQUEST &&
            ((usb_control_setup_t *)eventData)->wValue == 0x0200 &&
            ((usb_control_setup_t *)eventData)->wLength == sizeof(configuration)) {

            usb_Transfer(
                usb_GetDeviceEndpoint(
                    usb_FindDevice(NULL, NULL, USB_SKIP_HUBS),
                    0
                ),
                &configuration,
                sizeof(configuration),
                5,
                NULL
            );
            error = USB_IGNORE;
        }
    }

    return error;
}

int main(void) {
    usb_error_t error;
    static hid_kb_input_report_t hidKbInputReport = { 0 };

    os_ClrHome();
    os_PutStrFull("Macropad initiated.");

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        
        // This is the main loop; if you press `clear` then the program ends
        while (!kb_IsDown(kb_KeyClear)) {
            kb_Scan();
            usb_HandleEvents();

            if (kb_Data[1] == kb_2nd) {
                hidKbInputReport.keycodes[0] = 0x04;
                os_PutStrFull("Button pressed.");
            } else {
                hidKbInputReport.keycodes[0] = 0x0;
            }

            usb_InterruptTransfer(
                usb_GetDeviceEndpoint(
                    usb_FindDevice(NULL, NULL, USB_SKIP_HUBS),
                    USB_DEVICE_TO_HOST | 1
                ),
                &hidKbInputReport,
                sizeof(hid_kb_input_report_t),
                5,
                NULL
            );
        }
    }

    usb_Cleanup();
    return 0;
}