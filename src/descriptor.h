/**
 * Descriptors sent by the calculator to the host.
 * See:
 *  - https://wiki.osdev.org/Universal_Serial_Bus
 *  - https://beyondlogic.org/usbnutshell/usb5.shtml
 */

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "hid_spec.h"

#include <string.h>
#include <usbdrvce.h>

#define DEFAULT_LANG_ID 0x0409

static const usb_string_descriptor_t product_name = {
    .bLength = sizeof(product_name) + sizeof("CEyboard"),
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"CEyboard",
};

static const usb_string_descriptor_t manufacturer = {
    .bLength = sizeof(manufacturer) + sizeof("fredchan.org"),
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"fredchan.org",
};

static usb_string_descriptor_t serial = {
    .bLength = sizeof(serial) + 32,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"0000000000000000",
};

static const usb_string_descriptor_t *strings[] = { &product_name, &manufacturer, &serial };
static const usb_string_descriptor_t langids = {
    .bLength = sizeof(langids),
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = {
        [0] = DEFAULT_LANG_ID,
    },
};

static struct {
    usb_configuration_descriptor_t configuration;
    usb_interface_descriptor_t interface;
    hid_descriptor_t hid;
    usb_endpoint_descriptor_t endpoint;
    uint8_t * report_descriptor;
} configuration = {
    .configuration = {
        .bLength = sizeof(configuration.configuration),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 1,
        .bmAttributes = USB_CONFIGURATION_ATTRIBUTES | USB_BUS_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500 / 2,
    },
    .interface = {
        .bLength = sizeof(configuration.interface),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_HID_CLASS,
        .bInterfaceSubClass = 0, // No subclass
        .bInterfaceProtocol = 1, // Keyboard
        .iInterface = 0, // (Optional) Index of String Descriptor Describing this interface
    },
    .hid = {
        .bLength = sizeof(configuration.hid),
        .bDescriptorType = USB_HID_DESCRIPTOR,
        .bcdHID = 0x101, // HID Class Specification v1.11
        .bCountryCode = 0, // Not supported
        .bNumDescriptors = 1,
        .bReportDescriptorType = USB_HID_REPORT_DESCRIPTOR,
        .wDescriptorLength = sizeof(configuration.report_descriptor),
    },
    .endpoint = {
        .bLength = sizeof(configuration.endpoint),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_DEVICE_TO_HOST | 1,
        .bmAttributes = USB_INTERRUPT_TRANSFER,
        .wMaxPacketSize = sizeof(hid_kb_input_report_t),
        .bInterval = 10, // 10ms polling interval
    },
    .report_descriptor = (uint8_t[]){
        0x05, 0x01, //Usage Page (Generic Desktop)
        0x09, 0x06, //Usage (Keyboard)
        0xA1, 0x01, //Collection (Application)
        0x05, 0x07, //  Usage Page (Key Codes)
        0x19, 0xE0, //  Usage Minimum (E0)
        0x29, 0xE7, //  Usage Minimum (E7)
        0x15, 0x00, //  Logical Minimum (0)
        0x25, 0x01, //  Logical Maximum (1)
        0x75, 0x01, //  Report Size (1)
        0x95, 0x08, //  Report Count (8) 
        0x81, 0x02, //  Input (Data, Variable, Absolute), ;Modifier byte 
        0x95, 0x01, //  Report Count (1), 
        0x75, 0x08, //  Report Size (8), 
        0x81, 0x01, //  Input (Constant), ;Reserved byte 
        0x95, 0x06, //  Report Count (6), 
        0x75, 0x08, //  Report Size (8), 
        0x15, 0x00, //  Logical Minimum (0), 
        0x25, 0x65, //  Logical Maximum(101), 
        0x19, 0x00, //  Usage Minimum (0), 
        0x29, 0x65, //  Usage Maximum (101), 
        0x81, 0x00, //  Input (Data, Array), ;Key arrays (6 bytes)
        0xC0,       //End Collection
    },
};

static const usb_configuration_descriptor_t *configurations[] = {
    &configuration.configuration,
};

static const usb_device_descriptor_t device = {
    .bLength = sizeof(device),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 0x40,
    .idVendor = 0x0451, // Texas Instruments
    .idProduct = 0x1337,
    .bcdDevice = 0x0100,
    .iManufacturer = 2,
    .iProduct = 1,
    .iSerialNumber = 3,
    .bNumConfigurations = sizeof(configurations) / sizeof(*configurations),
};

static const usb_standard_descriptors_t standard = {
    .device = &device,
    .configurations = configurations,
    .langids = &langids,
    .numStrings = sizeof(strings) / sizeof(*strings),
    .strings = strings,
};

#endif
