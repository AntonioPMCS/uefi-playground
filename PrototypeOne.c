#include <Uefi.h>
//#include <HttpProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/Http.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/Tcp4.h>
#include <Protocol/ServiceBinding.h>

#define BUFFER_SIZE  0x10000

EFI_STATUS
EFIAPI
SendHttpRequest (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
) {
    EFI_STATUS Status;
    EFI_SERVICE_BINDING_PROTOCOL  *ServiceBinding;
    EFI_HANDLE                    *Handle;
    EFI_HTTP_PROTOCOL             *HttpProtocol;   
    UINT8                         *Buffer;       
    //EFI_SIMPLE_NETWORK_PROTOCOL   *SimpleNetwork;

    // most returns ignore memory allocation...
    Buffer = AllocateZeroPool (BUFFER_SIZE);
    if (Buffer == NULL) {
      DEBUG ((DEBUG_ERROR, "[PostRequest] AllocatePool failed with %r\n", Status));
      return Status;
    }
    if (Buffer == NULL) {
      DEBUG ((DEBUG_ERROR, "[PostRequest] AllocatePool failed with %r\n", Status));
      return Status;
    }

    Status = gBS->LocateProtocol(&gEfiHttpServiceBindingProtocolGuid,NULL, (VOID **)&ServiceBinding);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "[PrototypeOne - SendHttpRequest] LocateProtocol failed with %r\n", Status));
      return Status;
    }

    Status = ServiceBinding->CreateChild(ServiceBinding, (VOID **)&Handle);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "[PrototypeOne - SendHttpRequest] CreateChild failed with %r\n", Status));
      return Status;
    }

    Status = gBS->HandleProtocol(Handle, &gEfiHttpProtocolGuid, (VOID **)&HttpProtocol);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "[PrototypeOne - SendHttpRequest] HandleProtocol failed with %r\n", Status));
      return Status;
    }


    // // Locate the HTTP protocol
    // Status = gBS->LocateProtocol(&gEfiHttpProtocolGuid, NULL, (VOID**)&Http);
    // if (EFI_ERROR(Status)) {
    //     DEBUG((DEBUG_ERROR, "[PrototypeOne] HTTP Protocol not found. %a %d  \n", __FUNCTION__, __LINE__));
    //     return Status;
    // }

    // // Locate the Simple Network Protocol
    // Status = gBS->LocateProtocol(&gEfiSimpleNetworkProtocolGuid, NULL, (VOID**)&SimpleNetwork);
    // if (EFI_ERROR(Status)) {
    //     DEBUG((DEBUG_ERROR, "Simple Network Protocol not found. %a %d  \n", __FUNCTION__, __LINE__));
    //     return Status;
    // }

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
PrototypeOneEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;

    // Optionally, initialize network if needed (if Simple Network Protocol is not initialized)
    //Status = InitializeNetwork(SystemTable);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    DEBUG((DEBUG_ERROR, "[PrototypeOne] Hello! %a %d  \n", __FUNCTION__, __LINE__));

      // Send HTTP request
    Status = SendHttpRequest(ImageHandle, SystemTable);

    return Status;
}