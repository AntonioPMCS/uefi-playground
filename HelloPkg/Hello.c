#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
HelloMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  Print(L"Hello SoftI9 :)\n");
  return EFI_SUCCESS;
}