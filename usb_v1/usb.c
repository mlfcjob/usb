#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

char enp_num[8], buf8[8], is_change = 0;


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

    r = libusb_init(&ctx);
    if (r < 0)
        printf("Init error.\n");
    else
        printf("Succed init.\n");


    dev_count = libusb_get_device_list(ctx, &devs);
    if (dev_count < 0 )
        printf("get devices count failed.\n");
    else
        printf("There are %d usb devices on this machine.\n", dev_count);

    for (i = 0;  i < dev_count; i++) {
        bus_num  = libusb_get_bus_number(devs[i]);
        address  = libusb_get_device_address(devs[i]);
        get_vid_pid_and_bcd_device(devs[i], &vid, &pid, &bcd_device);
        printf("BusId:%d, DeviceId: %d, %04x:%04x--%04x.\n", bus_num, address, vid, pid, bcd_device);
    }
    
    return 0;

}


