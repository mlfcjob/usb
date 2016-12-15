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
    printf("VendorID: %04x,  ", desc.idVendor);
    printf("ProductID: %04x.\n", desc.idProduct);
  

    printf("%04x:%04x (bus %d, device %d).\n",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));
 
    struct libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);    

    const struct libusb_interface *inter;
    const struct libusb_interface *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;

    libusb_device_handle *handle;
    r = libusb_open(dev, &handle);
    if (r < 0){
        fprintf(stdout, "open usb device failed.\n");
        return;
    }
     
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



