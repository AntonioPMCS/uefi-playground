#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/ServiceBinding.h>
#include <Protocol/Http.h>

#define BUFFER_SIZE 0x100000

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
HttpExampleMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  EFI_STATUS                   Status;
  UINT8                        *Buffer;
  EFI_SERVICE_BINDING_PROTOCOL *ServiceBinding;
  EFI_HANDLE                   Handle;
  EFI_HTTP_PROTOCOL            *HttpProtocol;

  Status = SystemTable->BootServices->AllocatePool(
    EfiBootServicesData,
    BUFFER_SIZE,
    (VOID **)&Buffer
  );
  if (Buffer == NULL) {
    Print(L"Allocate Pool Failed\n");
    return Status;
  }


  Status = gBS->LocateProtocol(
    &gEfiHttpServiceBindingProtocolGuid,
    NULL,
    (VOID **)&ServiceBinding
  );
  if (EFI_ERROR (Status)) {
    Print(L"Locate Protocol Failed\n");
    return Status;
  }

  Status = ServiceBinding->CreateChild(ServiceBinding, &Handle);
    if (EFI_ERROR (Status)) {
    Print(L"CreateChild failed");
    return Status;
  }

  Status = gBS->HandleProtocol(Handle, &gEfiHttpProtocolGuid, (VOID **)&HttpProtocol);
    if (EFI_ERROR (Status)) {
    Print(L"HandleProtocol failed");
    ServiceBinding->DestroyChild (ServiceBinding, Handle);
    return Status;
  }



  Print(L"Welcome to Http Example!\n");

  return EFI_SUCCESS;
}
