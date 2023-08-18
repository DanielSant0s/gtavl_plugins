/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Pedro J. Cabrera (rc5stint@yahoo.com)
  ------------------------------------------------------------------------
  usbd.h		   USB Driver function prototypes and constants.

  Modeled after the Linux kernel USB APIs.
*/

#if !defined(_USBD_H)
#define _USBD_H

/* typedefs from psx2lib */
#include <tamtypes.h>

/* 
 * RC5Stint Note: This section of comments and #defines were copied 
 * verbatim from the file include/linux/usb.h in the PS2 Linux Kit. 
 */

/* USB constants */

/*
 * Device and/or Interface Class codes
 */
#define USB_CLASS_PER_INTERFACE		0	/* for DeviceClass */
#define USB_CLASS_AUDIO			1
#define USB_CLASS_COMM			2
#define USB_CLASS_HID			3
#define USB_CLASS_PHYSICAL		5
#define USB_CLASS_PRINTER		7
#define USB_CLASS_MASS_STORAGE		8
#define USB_CLASS_HUB			9
#define USB_CLASS_DATA			10
#define USB_CLASS_APP_SPEC		0xfe
#define USB_CLASS_VENDOR_SPEC		0xff

/*
 * USB types
 */
#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

/*
 * USB recipients
 */
#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03

/*
 * USB directions
 */
#define USB_DIR_OUT			0
#define USB_DIR_IN			0x80

/*
 * Descriptor types
 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05

#define USB_DT_HID			(USB_TYPE_CLASS | 0x01)
#define USB_DT_REPORT			(USB_TYPE_CLASS | 0x02)
#define USB_DT_PHYSICAL			(USB_TYPE_CLASS | 0x03)
#define USB_DT_HUB			(USB_TYPE_CLASS | 0x09)

/*
 * Descriptor sizes per descriptor type
 */
#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9
#define USB_DT_INTERFACE_SIZE		9
#define USB_DT_ENDPOINT_SIZE		7
#define USB_DT_ENDPOINT_AUDIO_SIZE	9	/* Audio extension */
#define USB_DT_HUB_NONVAR_SIZE		7
#define USB_DT_HID_SIZE			9

/*
 * Endpoints
 */
#define USB_ENDPOINT_NUMBER_MASK	0x0f	/* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK		0x80

#define USB_ENDPOINT_XFERTYPE_MASK	0x03	/* in bmAttributes */
#define USB_ENDPOINT_XFER_CONTROL	0
#define USB_ENDPOINT_XFER_ISOC		1
#define USB_ENDPOINT_XFER_BULK		2
#define USB_ENDPOINT_XFER_INT		3

/*
 * USB Packet IDs (PIDs)
 */
/* 
 * RC5Stint Note: some of these PIDs are for USB 2.0.
 * As far as I know, they may not apply to USBD.IRX.
 */
#define USB_PID_UNDEF_0                        0xf0
#define USB_PID_OUT                            0xe1
#define USB_PID_ACK                            0xd2
#define USB_PID_DATA0                          0xc3

#define USB_PID_PING                           0xb4	/* USB 2.0 */

#define USB_PID_SOF                            0xa5

#define USB_PID_NYET                           0x96	/* USB 2.0 */
#define USB_PID_DATA2                          0x87	/* USB 2.0 */
#define USB_PID_SPLIT                          0x78	/* USB 2.0 */

#define USB_PID_IN                             0x69
#define USB_PID_NAK                            0x5a
#define USB_PID_DATA1                          0x4b
#define USB_PID_PREAMBLE                       0x3c	/* Token mode */

#define USB_PID_ERR                            0x3c	/* USB 2.0: handshake mode */

#define USB_PID_SETUP                          0x2d
#define USB_PID_STALL                          0x1e

#define USB_PID_MDATA                          0x0f	/* USB 2.0 */

/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

/*
 * HID requests
 */
#define USB_REQ_GET_REPORT		0x01
#define USB_REQ_GET_IDLE		0x02
#define USB_REQ_GET_PROTOCOL		0x03
#define USB_REQ_SET_REPORT		0x09
#define USB_REQ_SET_IDLE		0x0A
#define USB_REQ_SET_PROTOCOL		0x0B

// Note: end of verbatim copy from PS2 Linux Kit

/* 
 * RC5Stint Note: These structs are modified versions of the structs 
 * in the file include/linux/usb.h in the PS2 Linux Kit.  They were 
 * modified to fit the structures used by USBD.IRX, and to use the 
 * typedefs from Gustavo Scotti's psx2lib.
 */

typedef struct _UsbDeviceRequest {
	u8 requesttype;
	u8 request;
	u16 value;
	u16 index;
	u16 length;
} UsbDeviceRequest __attribute__ ((packed));

/* Device descriptor */
typedef struct _UsbDeviceDescriptor {
	u8  bLength;
	u8  bDescriptorType;
	u16 bcdUSB;
	u8  bDeviceClass;
	u8  bDeviceSubClass;
	u8  bDeviceProtocol;
	u8  bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8  iManufacturer;
	u8  iProduct;
	u8  iSerialNumber;
	u8  bNumConfigurations;
} UsbDeviceDescriptor __attribute__ ((packed));

/* Configuration descriptor information.. */
typedef struct _UsbConfigDescriptor {
	u8  bLength;
	u8  bDescriptorType;
	u16 wTotalLength;
	u8  bNumInterfaces;
	u8  bConfigurationValue;
	u8  iConfiguration;
	u8  bmAttributes;
	u8  MaxPower;
/*
 * RC5Stint Note: These have been commented out because USBD.IRX
 * does not have them in its structures.
 */
/*	struct usb_interface *interface; */

/*	unsigned char *extra; */	/* Extra descriptors */
/*	int extralen; */
} UsbConfigDescriptor __attribute__ ((packed));

/* Interface descriptor */
typedef struct _UsbInterfaceDescriptor {
	u8  bLength;
	u8  bDescriptorType;
	u8  bInterfaceNumber;
	u8  bAlternateSetting;
	u8  bNumEndpoints;
	u8  bInterfaceClass;
	u8  bInterfaceSubClass;
	u8  bInterfaceProtocol;
	u8  iInterface;
/*
 * RC5Stint Note: These have been commented out because USBD.IRX
 * does not have them in its structures.
 */
/*	UsbEndpointDescriptor *endpoint; */

/*	unsigned char *extra; */	/* Extra descriptors */
/*	int extralen; */
} UsbInterfaceDescriptor __attribute__ ((packed));

typedef struct _UsbInterface {
/*
 * RC5Stint Note: These have been commented out because USBD.IRX
 * does not have them in its structures.
 */
/*	UsbInterfaceDescriptor *altsetting; */

	int act_altsetting;		/* active alternate setting */
	int num_altsetting;		/* number of alternate settings */
	int max_altsetting;		/* total memory allocated */
 
/*
 * RC5Stint Note: These have been commented out because USBD.IRX
 * does not have them in its structures.
 */
/*	UsbDriver *driver; */
/*	void *private_data; */
} UsbInterface __attribute__ ((packed));

/* Endpoint descriptor */
typedef struct _UsbEndpointDescriptor {
	u8  bLength;
	u8  bDescriptorType;
	u8  bEndpointAddress;
	u8  bmAttributes;
	u8 wMaxPacketSizeLB;
	u8 wMaxPacketSizeHB;
	u8  bInterval;
/*
 * RC5Stint Note: These have been commented out because USBD.IRX
 * does not have them in its structures.
 */
/*	u8  bRefresh; */
/*	u8  bSynchAddress; */

/*	unsigned char *extra; */   /* Extra descriptors */
/*	int extralen; */
} UsbEndpointDescriptor __attribute__ ((packed));

/* String descriptor */
typedef struct _UsbStringDescriptor {
	u8  bLength;
	u8  bDescriptorType;
	u16 wData[1];
} UsbStringDescriptor __attribute__ ((packed));

/* RC5Stint Note: end of modified copy from PS2 Linux Kit */

/* 
 * RC5Stint Note: Here are the function prototypes and 
 * typedefs specific to USBD.IRX.
 */

/* USB driver bus event listener structure */
typedef struct _UsbDriver {
	/*
	 * These pointers are used internally by USBD.IRX to keep a 
	 * doubly linked list of drivers.  They should be set to NULL 
	 * before the driver is registered.  USBD.IRX will set them as 
	 * necessary.
	 *
	 * Note: leave them alone after your driver has been registered!
	 */
	struct _UsbDriver *next, *prev;

	/* short sweet name for your driver, like "usbmouse" or "pl2301" */
	char *name;

	/* 
	 * This function pointer is called when a device is added to the bus.
	 * The probe function should examine the device's static descriptors, 
	 * to see if it can support the device.
	 *
	 * It should return a 1 if it can support the device, 0 if it cannot.
	 */
	int (*probe)(int devID);

	/*
	 * This function pointer is called after the probe function returns a 1
	 * for a particular device ID.  The connect function should allocate any 
	 * private data needed to keep configuration for this device ID, set the 
	 * private data pointer for the device, and open any endpoints necessary 
	 * to communicate with this device.
	 *
	 * It should return 0 if successful, or -1 if not successful.
	 */
	int (*connect)(int devID);

	/*
	 * This function pointer is called after a connected device is removed
	 * from the bus.  The disconnect function should deallocate any private 
	 * data needed to keep configuration for this device ID, and close any 
	 * endpoints that were left open to communicate with this device.
	 *
	 * It should return 0 if successful, or -1 if not successful.
	 */
	int (*disconnect)(int devID);

	/*
	 * None of the drivers I've reverse-engineered touch the next 20 bytes.
	 * They don't seem to be used for anything.
	 */
	u8 reserved[20];

	/*
	 * It is used, but some drivers leave it unset.  For this reason, 
	 * it doesn't seem to be all that important.
	 *
	 * Any help figuring out what it is for would be greatly appreciated.
	 */
	u32 unknown;
} UsbDriver __attribute__ ((packed));

/* 
 * initialize USBD.IRX
 * Note: UsbInit is automatically called first whenever USBD.IRX is loaded.
 * There should never be a need to reinitialize the driver.  In fact, it may 
 * not even work.  But I'm providing the function hook anyhow. <shrug>
 */
int UsbInit(void);

/*
 * These two functions are used to register and unregister USB device drivers to
 * listen for USB bus events.  The events are device probe, connect, and disconnect.
 * They return USB_RC_OK if successful.
 */
int UsbRegisterDriver(UsbDriver *driver);
int UsbUnregisterDriver(UsbDriver *driver);

/*
 * This function is used to get the static descriptors for the specific USB 
 * device.  These descriptors identify the device uniquely and help determine 
 * what type of device we are dealing with, and what its capabilities and 
 * features are.
 */
void *UsbGetDeviceStaticDescriptor(int devID, void *data, u8 type);

/*
 * These two functions are used to assign relevant data to a specific device.  
 * The type of data is entirely up to the caller.  For example, a particular 
 * USB device driver may store configuration data for each specific device 
 * under its control.
 */
int UsbSetDevicePrivateData(int devID, void *data);
void *UsbGetDevicePrivateData(int devID);

/* 
 * This function returns an endpoint ID for the device ID and endpoint descriptor 
 * passed in.  This endpoint ID is then used when transfering data to the device, 
 * and to close the endpoint.
 */
int UsbOpenEndpoint(int devID, UsbEndpointDescriptor *epDesc);
int UsbCloseEndpoint(int epID);

/*
 * The data transfer function uses a callback of this type to notify the caller 
 * that the transfer is done, whether or not it was successful, and how many 
 * bytes were actually transferred.
 */
typedef	void (*UsbTransferDoneCallBack)(int resultCode, int bytes, void *arg);

/*
 * This function is used for all types of USB data transfers.  Which type of 
 * transfer is determined by the parameters that are passed in.  The types are:
 * control, isochronous, interrupt, and bulk transfers.
 */
int UsbTransfer(int epID, void *data, int length, void *optionalData, UsbTransferDoneCallBack doneCB, void *arg);
	
/* 
 * Note: The values of these #defines were found by reverse engineering 
 * the file PS2USBC.IRX in the Unreal Tournament for PS2 CD, and the files 
 * USBKEYBD.IRX and USBMOUSE.IRX in other demo and game CDs.
 */

/* control, isochronous, bulk, and interrupt transfers */

#define UsbControlTransfer(epID, reqtyp, req, val, index, len, dataptr, doneCB, arg) ({ \
		UsbDeviceRequest devreq; \
		devreq.requesttype = (reqtyp); \
		devreq.request = (req); \
		devreq.value = (val); \
		devreq.index = (index); \
		devreq.length = (len); \
		UsbTransfer((epID), (dataptr), devreq.length, &devreq, (doneCB), (arg)); \
	})

#define UsbIsochronousTransfer(epID, dataptr, len, delta, doneCB, arg) \
	UsbTransfer((epID), (dataptr), (len), (void *)(delta), (doneCB), (arg))

#define UsbBulkTransfer(epID, dataptr, len, doneCB, arg) \
	UsbTransfer((epID), (dataptr), (len), NULL, (doneCB), (arg))

#define UsbInterruptTransfer(epID, dataptr, len, doneCB, arg) \
	UsbTransfer((epID), (dataptr), (len), NULL, (doneCB), (arg))

/* standard control transfers */

#define UsbClearDeviceFeature(epID, feature, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_DEVICE, USB_REQ_CLEAR_FEATURE, \
	(feature), 0, 0, NULL, (doneCB), (arg))

#define UsbSetDeviceFeature(epID, feature, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_DEVICE, USB_REQ_SET_FEATURE, \
	(feature), 0, 0, NULL, (doneCB), (arg))

#define UsbGetDeviceConfiguration(epID, dataptr, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_DEVICE, USB_REQ_GET_CONFIGURATION, \
	0, 0, 1, (dataptr), (doneCB), (arg))

#define UsbSetDeviceConfiguration(epID, config, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_DEVICE, USB_REQ_SET_CONFIGURATION, \
	(config), 0, 0, NULL, (doneCB), (arg))

#define UsbGetDeviceDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_DEVICE, USB_REQ_GET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbSetDeviceDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_DEVICE, USB_REQ_SET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbGetDeviceStatus(epID, dataptr, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_DEVICE, USB_REQ_GET_STATUS, \
	0, 0, 2, (dataptr), (doneCB), (arg))

#define UsbSetDeviceAddress(epID, address, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_DEVICE, USB_REQ_SET_ADDRESS, \
	(address), 0, 0, NULL, (doneCB), (arg))

#define UsbClearInterfaceFeature(epID, feature, interface, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_INTERFACE, USB_REQ_CLEAR_FEATURE, \
	(feature), (interface), 0, NULL, (doneCB), (arg))

#define UsbSetInterfaceFeature(epID, feature, interface, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_INTERFACE, USB_REQ_SET_FEATURE, \
	(feature), (interface), 0, NULL, (doneCB), (arg))

#define UsbGetInterface(epID, interface, dataptr, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_INTERFACE, USB_REQ_GET_INTERFACE, \
	0, (interface), 1, (dataptr), (doneCB), (arg))

#define UsbSetInterface(epID, interface, alt_setting, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_INTERFACE, USB_REQ_SET_INTERFACE, \
	(alt_setting), (interface), 0, NULL, (doneCB), (arg))

#define UsbGetInterfaceDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_INTERFACE, USB_REQ_GET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbSetInterfaceDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_INTERFACE, USB_REQ_SET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbGetInterfaceStatus(epID, interface, dataptr, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_INTERFACE, USB_REQ_GET_STATUS, \
	0, (interface), 2, (dataptr), (doneCB), (arg))

#define UsbClearEndpointFeature(epID, feature, endpoint, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_ENDPOINT, USB_REQ_CLEAR_FEATURE, \
	(feature), (endpoint), 0, NULL, (doneCB), (arg))

#define UsbSetEndpointFeature(epID, feature, endpoint, doneCB, arg)	\
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_ENDPOINT, USB_REQ_SET_FEATURE, \
	(feature), (endpoint), 0, NULL, (doneCB), (arg))

#define UsbGetEndpointStatus(epID, endpoint, dataptr, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_ENDPOINT, USB_REQ_GET_STATUS, \
	0, (endpoint), 2, (dataptr), (doneCB), (arg))

#define UsbGetEndpointDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_IN & USB_RECIP_ENDPOINT, USB_REQ_GET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbSetEndpointDescriptor(epID, type, index, language, dataptr, len, doneCB, arg) \
	UsbControlTransfer((epID), USB_DIR_OUT & USB_RECIP_ENDPOINT, USB_REQ_SET_DESCRIPTOR, \
	((type) << 8) | (index), (language), (len), (dataptr), (doneCB), (arg))

#define UsbSynchEndpointFrame(epID, endpoint, pfn, doneCB, arg)	\
	UsbControlTransfer((pid), USB_DIR_IN & USB_RECIP_ENDPOINT, USB_REQ_SYNCH_FRAME, \
	0, (endpoint), 2, (pfn), (doneCB), (arg))

/* result codes for transfer done callback */

#define USB_RC_OK			0x000	// No Error
#define USB_RC_CRC			0x001	// Bad CRC
#define USB_RC_BITSTUFF		0x002	// Bit Stuffing
#define USB_RC_TOGGLE		0x003	// Bad Direction Toggle
#define USB_RC_STALL		0x004	// Endpoint Stalled
#define USB_RC_NORESPONSE	0x005	// Device Is Not Responding
#define USB_RC_BADPID		0x006	// PID Check Failed
#define USB_RC_WRONGPID		0x007	// Unexpected PID 
#define USB_RC_DATAOVER		0x008	// Data Overrun
#define USB_RC_DATAUNDER	0x009	// Data Underrun

#define USB_RC_BUFFOVER		0x00c	// Buffer Overrun
#define USB_RC_BUFFUNDER	0x00d	// Buffer Underrun

#define USB_RC_BADDEV		0x101	// Invalid device ID
#define USB_RC_BADPIPE		0x102	// Invalid pipe ID
#define USB_RC_BADLENGTH	0x103	// Invalid length
#define USB_RC_BADDRIVER	0x104	// Invalid driver
#define USB_RC_BADCONTEXT	0x105	// Invalid context

#define USB_RC_ED			0x111	// No space for Endpoint Descriptor
#define USB_RC_IOREQ		0x112	// No space for Input/Output Request
#define USB_RC_BADOPTION	0x113	// Bad Option

#define USB_RC_BUSY			0x121	// Device or Bus Busy
#define USB_RC_ABORTED		0x122	// Operation Aborted

#define USB_RC_NOSUPPORT	0x131	// Unsupported Operation (not implemented)
#define USB_RC_UNKNOWN		0x132	// Unknown Error (USBD.IRX doesn't know what went wrong)

/* Unicode Language ID for English (United States) */
#define USB_LANG_ID_ENGLISH_USA 0x0409

/* mask for configuration attributes */
#define USB_CONFIG_ATTRIBUTE_MASK 0x60

#endif	/* !_USBD_H */
