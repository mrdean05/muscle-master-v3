#ifndef __USB_DRIVER_H__
#define __USB_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

void USBDriver_initTask( void *arg );
void USBDriver_HIDHostQueue( void *args );

#ifdef __cplusplus
}
#endif
#endif