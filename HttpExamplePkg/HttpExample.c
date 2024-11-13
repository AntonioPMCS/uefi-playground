#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/ServiceBinding.h>
#include <Protocol/Http.h>

#define BUFFER_SIZE 0x100000

// Includes for networking
#include <Protocol/Ip4Config2.h>
/////////////////////////////////////

// Typedefs for networking
typedef struct _IFCONFIG_INTERFACE_CB {
  EFI_HANDLE                        NicHandle;
  LIST_ENTRY                        Link;
  EFI_IP4_CONFIG2_PROTOCOL          *IfCfg;
  EFI_IP4_CONFIG2_INTERFACE_INFO    *IfInfo;
  EFI_IP4_CONFIG2_POLICY            Policy;
  UINT32                            DnsCnt;
  EFI_IPv4_ADDRESS                  DnsAddr[1];
} IFCONFIG_INTERFACE_CB;

//////////////////////////////////////////////////////////

/**
  The get current status of all handles.

  @param[in]   IfName         The pointer of IfName(interface name).
  @param[in]   IfList         The pointer of IfList(interface list).

  @retval EFI_SUCCESS    The get status processed successfully.
  @retval others         The get status process failed.

**/
EFI_STATUS
IfConfigGetInterfaceInfo (
  IN CHAR16      *IfName,
  IN LIST_ENTRY  *IfList
  )
{
  EFI_STATUS                      Status;
  UINTN                           HandleIndex;
  UINTN                           HandleNum;
  EFI_HANDLE                      *HandleBuffer;
  EFI_IP4_CONFIG2_PROTOCOL        *Ip4Cfg2;
  EFI_IP4_CONFIG2_INTERFACE_INFO  *IfInfo;
  IFCONFIG_INTERFACE_CB           *IfCb;
  UINTN                           DataSize;

   //
  // Locate all the handles with ip4 service binding protocol.
  //
  Status = gBS->LocateHandleBuffer (
                                    ByProtocol,
                                    &gEfiIp4ServiceBindingProtocolGuid,
                                    NULL,
                                    &HandleNum,
                                    &HandleBuffer
                                    );
    if (EFI_ERROR (Status) || (HandleNum == 0)) {
      Print(L"[IfConfigGetInterfaceInfo] LocateHandleBuffer: Unable to LocateHandleBuffer\n");
      return Status;
  }

  return Status;
}
  //////////////////////////////////////////////////////////////

/**
  Configuring the network interface devices. We will see why.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/

EFI_STATUS
IfConfig (
  )
{
  EFI_STATUS              Status;
  IFCONFIG_INTERFACE_CB   *IfCb;
  EFI_IP4_CONFIG2_POLICY  Policy;
  LIST_ENTRY              *IfList;
  LIST_ENTRY              *Entry;
  LIST_ENTRY              *NextEntry;

  IfList = AllocatePool (sizeof (LIST_ENTRY));
  InitializeListHead (IfList);
  Status = IfConfigGetInterfaceInfo (
                                     L"eth0",
                                     IfList
                                     );
  if (EFI_ERROR (Status)) {
    Print(L"[IfConfig] Failed to get interface info\n");
    return Status;
  }

  // IfCb = NET_LIST_USER_STRUCT (IfList->ForwardLink, IFCONFIG_INTERFACE_CB, Link);

  // Policy = Ip4Config2PolicyDhcp;
  // Status = IfCb->IfCfg->SetData (
  //                                IfCb->IfCfg,
  //                                Ip4Config2DataTypePolicy,
  //                                sizeof (EFI_IP4_CONFIG2_POLICY),
  //                                &Policy
  //                                );
  // if (EFI_ERROR (Status)) {
  //   DEBUG ((DEBUG_ERROR, "[IfConfig] Failed to set Ip4Config data: %r\n", Status));
  //   return Status;
  // }

  // // Clean the IFCONFIG_INTERFACE_CB list.
  // NET_LIST_FOR_EACH_SAFE (Entry, NextEntry, IfList) {
  //   IfCb = NET_LIST_USER_STRUCT (Entry, IFCONFIG_INTERFACE_CB, Link);

  //   RemoveEntryList (&IfCb->Link);

  //   if (IfCb->IfInfo != NULL) {
  //     FreePool (IfCb->IfInfo);
  //   }

  //   FreePool (IfCb);
  // }
  // FreePool (IfList);
  return EFI_SUCCESS;
}




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


  /************ Configure the Network Interface ************/
  Status = IfConfig ();





  ////////////////////////////////////////////////////////////////////////////////////////////

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
