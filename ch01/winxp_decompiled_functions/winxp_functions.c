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


 lkd> dt nt!_KAPC */
   +0x000 Type             : Int2B
   +0x002 Size             : Int2B
   +0x004 Spare0           : Uint4B
   +0x008 Thread           : Ptr32 _KTHREAD
   +0x00c ApcListEntry     : _LIST_ENTRY
   +0x014 KernelRoutine    : Ptr32     void
   +0x018 RundownRoutine   : Ptr32     void
   +0x01c NormalRoutine    : Ptr32     void
   +0x020 NormalContext    : Ptr32 Void
   +0x024 SystemArgument1  : Ptr32 Void
   +0x028 SystemArgument2  : Ptr32 Void
   +0x02c ApcStateIndex    : Char
   +0x02d ApcMode          : Char
   +0x02e Inserted         : UChar

typedef struct _KAPC {
     Int2B	   Type,                +0x000
       Int2B	   Size,		+0x002
       Uint4B	   Spare0,           	+0x004
       Ptr32 _KTHREAD Thread,      	+0x008
       _LIST_ENTRY ApcListEntry,	+0x00c
       Ptr32     void KernelRoutine,	+0x014
       Ptr32     void RundownRoutine,	+0x018
       Ptr32     void NormalRoutine ,	+0x01c
       Ptr32 Void NormalContext ,	+0x020
       Ptr32 Void SystemArgument1,	+0x024
       Ptr32 Void SystemArgument2  ,	+0x028
       Char ApcStateIndex,    		+0x02c
       Char ApcMode          ,		+0x02d
       UChar Inserted         ,		+0x02e
       } *PKAPC;


//  void KeInitializeApc(A* a, c,int d, c2, c3, int c4) {
  void KeInitializeApc(
		       *PKAPC arg_0,
		       Ptr32 _KTHREAD arg_4,
		       dword arg_8,
		       PTR32 arg_C,
		       PTR32 arg_10,
		       PTR32 arg_14,
		       char arg_18,
		       dword arg_1C,
		       ) {
    
    a->size = 0x30;
    a->type = 0x12;

    a->ApcStateIndex = (arg_8 == 2)? (char)2 : *((char *)arg_4+0x165);

    a->Thread = arg_4;
    a->kernelRoutine = arg_c;
    a->rundownRoutine = arg_10;
    a->normalRoutine = arg_14; 
  
  if (arg_14 != 0) {
    a->ApcStateIndex = arg_18;
    a->ApcMode = arg_18;
    a->NormalContext = arg_1C;
  }
  else {

    a->ApcStateIndex = 0;
    a->ApcMode = 0;
    a->NormalContext = 0;
  } 


}




/*
obsFastDereferenceObject 

 1 05bbf10 8bff            mov     edi,edi
 2 805bbf12 55              push    ebp
 3 805bbf13 8bec            mov     ebp,esp
 4 805bbf15 83ec0c          sub     esp,0Ch
 5 805bbf18 53              push    ebx
 6 805bbf19 56              push    esi
 7 805bbf1a 57              push    edi
 8 805bbf1b 8bf9            mov     edi,ecx
 9 805bbf1d 8bda            mov     ebx,edx
10 805bbf1f 897df8          mov     dword ptr [ebp-8],edi
11 805bbf22 8b37            mov     esi,dword ptr [edi]
12 805bbf24 8bc6            mov     eax,esi
13 805bbf26 33c3            xor     eax,ebx
14 805bbf28 83f807          cmp     eax,7
15 805bbf2b 8975f4          mov     dword ptr [ebp-0Ch],esi
16 805bbf2e 7318            jae     nt!ObFastDereferenceObject+0x38 (805bbf48)
17 805bbf30 8d4601          lea     eax,[esi+1]
18 805bbf33 8945fc          mov     dword ptr [ebp-4],eax
19 805bbf36 8b45f4          mov     eax,dword ptr [ebp-0Ch]
20 805bbf39 8b4df8          mov     ecx,dword ptr [ebp-8]
21 805bbf3c 8b55fc          mov     edx,dword ptr [ebp-4]
22 805bbf3f 0fb111          cmpxchg dword ptr [ecx],edx
23 805bbf42 3bc6            cmp     eax,esi
24 805bbf44 75dc            jne     nt!ObFastDereferenceObject+0x12 (805bbf22)
25 805bbf48 8bcb            mov     ecx,ebx
26 805bbf4a e8636cf6ff      call    nt!ObfDereferenceObject (80522bb2)
27 805bbf4f 5f              pop     edi
28 805bbf50 5e              pop     esi
29 805bbf51 5b              pop     ebx
30 805bbf52 c9              leave
31 805bbf53 c3              ret





line 4 reserves 12 bytes on stack 
lines 5 to 7  save ebx, esi and edi 

The parameters here are passed in register ecx and edx. 
We will refer them as arg_cx and arg_dx.

lines 8 to 15 move the first field pointed by ecx in ebp -12,
             move ecx to ebp - 8
	     then xor the first field pointed by ecx(arg_cx) with arg_dx and compare the result with 7 (lines 12 to 14)
	     if the results is greater or equal to 7 it jumps at line
             25, calling objDerefereneObject and returning.
lines 18 to 23 
             In this lines first field pointed by arg_cx is incremented in an atomic/thread safe  way trought the use of the cmpxchg instruction
	     First the incremented value is moved at ebp - 4 (lines 17/18)
	     Then the following registers are setted (lines 18/21):
	     eax = *arg_cx
	     ecx = first_argument
	     edx = incremented value (*(arg_cx) + 1)
	     Finally in line 22 cmpxchg is called. 

	     lines 23 check if the increment is succesful comparing
	              eax if not loop (start of the loop is line 15)
	              until the incement succedded or the check at 15 failed.

	      then on line 25 ebx is moved in ecx (why?) and then objDereferenceObject is called 
	      lines 27/31 are register restoration and function epilogue

	      Obscure points (to me):
	      -> why increment in objDEREFERENCE? (should be decrement
	      counter ref) 

	      -> here[0] I found the implementationo of reactos. Here
	      it semas obfDereferenceObject is alwayes called, in
	      reactos version is called only when "fastDerefence"
	      fails.

	      -> line 25 (is for passing argument to
	      ObjDereferenceObject? but ObjDereferenceObject isn't
	      fastcall!)

	      -> the xor and check on line 15 (can't figure out from reactos comments neither)
	     
stack

ebp-4 = (arg_cx->arg_0) + 1
ebp-8 = arg_cx
ebp-12 = arg_cx->arg_0

[0]http://doxygen.reactos.org/d0/d72/ex_8h_a1c4d9a1c94cde2bf267e32b41015c8bc.html#a1c4d9a1c94cde2bf267e32b41015c8bc

PSEUDO C decompilation following:

*/

    void ObjFastDereferenceObject(PTR32 fastReference,
				  PTR32 object){

      if ((arg_cx->arg_0 ^ arg_dx) < 7) {
	ATOMIC (arg_cx->arg_0++);
      }


      /* Not sure how object is passed */
      ObjDereferenceObject(object);

      return;
}

