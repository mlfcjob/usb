#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define CHAR_MAX_LEN 256

typedef struct usb_st{
    char sn[CHAR_MAX_LEN];
    char productp[CHAR_MAX_LEN];
    uint16_t pid;
    uint16_t vid;
}usb_st;

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

    //buffer
    char buf_serial[CHAR_MAX_LEN] ={'\0'};
    char buf_manufacture[CHAR_MAX_LEN] ={'\0'};
    char buf_product[CHAR_MAX_LEN] ={'\0'};
    char usb_device_name[CHAR_MAX_LEN * 2] = {'\0'};

    const struct libusb_interface *inter;
    const struct libusb_interface *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;

    libusb_device_handle *handle;
    r = libusb_open(dev, &handle);
    if (r < 0){
        fprintf(stdout, "open usb device failed.\n");
        return;
    }

    libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, buf_serial, CHAR_MAX_LEN);
    printf("serialNumber: %s.\n", buf_serial);

    libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, buf_manufacture, CHAR_MAX_LEN);
    libusb_get_string_descriptor_ascii(handle, desc.iProduct, buf_product, CHAR_MAX_LEN);

    strcpy(usb_device_name, buf_manufacture);
    strcat(usb_device_name, buf_product);
    if (strncmp(usb_device_name, "Linux", 5) != 0)
        printf("usb device name: %s.\n", usb_device_name);
}

char *get_usb_device_info_by_pipe()
{
    FILE *fp;
    char buffer[1024] = {'\0'};
    fp = popen("lsusb", "r");
    fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), fp);
    printf("%s", buffer);
    pclose(fp);
    return buffer;
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
      
    //get_usb_device_info_by_pipe(); 
    return 0;
}



