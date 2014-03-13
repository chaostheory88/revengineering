struct PRKDPC {
  short field_1;
  char byte1;
  char byte2;
  char bytes_pad_1[7]; //because we don't need about that
  PKDEFERRED_ROUTINE DeferredRoutine;
  PVOID DeferredContext;
  char bytes_pad_2[8];
  int mask;
  
} 

VOID KeInitializeDpc(
		     _Out_ PRKDPC Dpc,
		     _In_ PKDEFERRED_ROUTINE DeferredRoutine,
		     _In_opt_ PVOID DeferredContext
		     )
{

  memset(Dpc+28,0,4);
  Dpc->DeferredRoutine = DeferredRoutine
   
  Dpc->field_1 = 0x13;
  Dpc->byte_1 = 0x0;
  Dpc->byte_2 = 0x1;
  Dpc->DeferredContext = DeferredContext;
  
}
