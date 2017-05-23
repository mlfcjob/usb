#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

char enp_num[8], buf8[8], is_change = 0;

void get_interface_class(libusb_device *dev, uint8_t *interface_class)
{
    int i, j;
    struct libusb_config_descriptor *config;
    struct libusb_interface *interface;
    struct libusb_interface_descriptor *altsetting;

    int r = libusb_get_config_descriptor(dev, 0, &config);
    if (r != 0 ) {
        printf("failed to get config descriptor.\n");
        return;
    }

    for( i = 0; i < config->bNumInterfaces; i++) {
        *interface_class = config->interface[i].altsetting->bInterfaceClass;
    }

    libusb_free_config_descriptor(config);
}

void get_vid_pid_and_bcd_device(libusb_device *dev, uint16_t *vid, uint16_t *pid, uint16_t *bcd_device)
{
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);

    if (r < 0) {
        printf("get device descriptor failed.\n");
        return;
    }

    *vid = desc.idVendor;
    *pid = desc.idProduct;
    *bcd_device = desc.bcdDevice;
}

int main(int argc, char *argv[])
{
    struct usb_bus *bus;

    libusb_device *dev;

    libusb_device **devs;
    libusb_context *ctx = NULL;

    int r, dev_count;
    int i;

    uint8_t address;
    uint8_t bus_num;
    uint16_t vid = -1;
    uint16_t pid = -1;
    uint16_t bcd_device = -1;
    uint8_t interface_class = -1;

    r = libusb_init(&ctx);
    if (r < 0)
        printf("Init error.\n");
    else
        printf("Succed init.\n");


    dev_count = libusb_get_device_list(ctx, &devs);
    if (dev_count < 0 )
        printf("get devices count failed.\n");
    else
        printf("There are %d usb devices on this machine.\n\n", dev_count);

    for (i = 0;  i < dev_count; i++) {
        bus_num  = libusb_get_bus_number(devs[i]);
        address  = libusb_get_device_address(devs[i]);
        get_vid_pid_and_bcd_device(devs[i], &vid, &pid, &bcd_device);
        get_interface_class(devs[i], &interface_class);
        printf("InterfaceClass: %d, %d-%d, %04x:%04x--%04x.\n", interface_class, bus_num, address, vid, pid, bcd_device);
    }
    
    return 0;

}


