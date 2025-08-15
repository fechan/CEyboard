/**
 * @brief HID descriptor types as defined by the USB HID specs.
 * See:
 *  - https://www.usb.org/sites/default/files/hid1_11.pdf
 *  - https://www.usb.org/sites/default/files/hut1_21.pdf
 */

#ifndef HID_SPEC_H
#define HID_SPEC_H

#include <stdint.h>
#include <usbdrvce.h>

typedef enum hid_descriptor_class {
    USB_HID_INPUT_ENDPOINT_DESCRIPTOR = 5,
    USB_HID_DESCRIPTOR = 33,
    USB_HID_REPORT_DESCRIPTOR = 34,
} hid_descriptor_class;

/**
 * @brief The HID descriptor identifies the length and type of subordinate
 * descriptors for a device. (NOTE: I did not implement the optional fields)
 * @cite Section 6.2.1
 */
typedef struct hid_descriptor_t {
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint16_t    bcdHID;
    uint8_t     bCountryCode;
    uint8_t     bNumDescriptors;
    uint8_t     bReportDescriptorType;
    uint16_t    wDescriptorLength;
} hid_descriptor_t;

typedef struct hid_kb_input_report_t {
    uint8_t modifierKeys;
    uint8_t reserved;
    uint8_t keycodes[6];
} hid_kb_input_report_t;

#endif
