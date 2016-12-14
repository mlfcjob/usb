#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

void print_dev(libusb_device *dev)
{
    ssize_t i;
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0){
        fprintf(stdout, "failed to get descriptor.\n");
        return;
    }

    printf("\n*********************\n");
    printf("Number of possible configurations:%d, ", (int)desc.bNumConfigurations);
    printf("Device class: %d, ", (int)desc.bDeviceClass);
    printf("VendorID: %d,  ", desc.idVendor);
    printf("ProductID: %d.\n", desc.idProduct);
   
    struct libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);    

    const struct libusb_interface *inter;
    const struct libusb_interface *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;

}

int main(int argc, char *argv[])
{
    ssize_t device_num;

    libusb_context *usb_context;  
    libusb_device  **devs;       /*pointer to device, used to retrieve a list of devices*/

    if (libusb_init(&usb_context) != 0)
        fprintf(stdout, "libusb init failed.\n");

    device_num = libusb_get_device_list(usb_context, &devs);

    fprintf(stdout, "usb device nums: %d.\n", device_num);

    ssize_t i;

    for (i = 0; i < device_num; i++){
        print_dev(devs[i]);
    }
    
    libusb_free_device_list(devs, 1);
    libusb_exit(usb_context);
       
    return 0;
}



