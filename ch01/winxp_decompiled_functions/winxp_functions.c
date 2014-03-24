typedef struct _KDPC
{
  UCHAR Type;
  WORD Number;
  UCHAR Importance;
  LIST_ENTRY DpcListEntry;
  PVOID DeferredRoutine;
  PVOID DeferredContext;
  PVOID SystemArgument1;
  PVOID SystemArgument2;
  PVOID Lock
} KDPC, *PKDPC;

/*
  kd> dt nt!_KDPC
   +0x000 Type             : Int2B
   +0x002 Number           : UChar
   +0x003 Importance       : UChar
   +0x004 DpcListEntry     : _LIST_ENTRY
   +0x00c DeferredRoutine  : Ptr32     void
   +0x010 DeferredContext  : Ptr32 Void
   +0x014 SystemArgument1  : Ptr32 Void
   +0x018 SystemArgument2  : Ptr32 Void
   +0x01c Lock             : Ptr32 Uint4B
 */

VOID KeInitializeDpc(
		     _Out_ PRKDPC Dpc,
		     _In_ PKDEFERRED_ROUTINE DeferredRoutine,
		     _In_opt_ PVOID DeferredContext
		     )
{

  Dpc->Lock = NULL;
  Dpc->DeferredRoutine = DeferredRoutine
   
  Dpc->Type = 0x13;
  Dpc->Number = 0x0;
  Dpc->Importance = 0x1;
  Dpc->DeferredContext = DeferredContext;
  
}


/*
  lkd> dt nt!_KAPC
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
*/

typedef struct _KAPC
{
  UCHAR Type;
  UCHAR Size;
  UCHAR SizeByte0;
  PKTHREAD Thread;
  LIST_ENTRY ApcListEntry;
  PVOID KernelRoutine;
  PVOID RoundownRoutine;
  PVOID NormalRoutine;
  PVOID NormalContext;
  PVOID SystemArgument1;
  PVOID SystemArgument2;
  CHAR ApcStateIndex;
  CHAR ApcMode;
  UCHAR Inserted;
} KAPC, *PRKAPC;

//  void KeInitializeApc(A* a, c,int d, c2, c3, int c4) {
VOID
KeInitializeApc (
    __out PRKAPC Apc,
    __in PRKTHREAD Thread,
    __in KAPC_ENVIRONMENT Environment,
    __in PKKERNEL_ROUTINE KernelRoutine,
    __in_opt PKRUNDOWN_ROUTINE RundownRoutine,
    __in_opt PKNORMAL_ROUTINE NormalRoutine,
    __in_opt KPROCESSOR_MODE ApcMode,
    __in_opt PVOID NormalContext
		 ) {
   
  Apc->Size = 0x30;
  Apc->Type = 0x12;
  
  if (Environment == CurrentApcEnvironment) {
    Apc->ApcStateIndex = Thread->ApcStateIndex;
  } else {
    Apc->ApcStateIndex = Environment;
  }

  Apc->Thread = Thread;
  Apc->KernelRoutine = KernelRoutine;
  Apc->RundownRoutine = RundownRoutine;
  Apc->NormalRoutine = NormalRoutine; 
  
  if (NormalRoutine == NULL) {
    Apc->ApcMode = NULL;
    Apc->NormalContext = NULL;
  } else {
    Apc->Inserted = NULL;
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

VOID
FASTCALL
ObjFastDereferenceObject(
			 IN PEX_FAST_REF FastRef,
				  IN PVOID Object)
{

      if ((arg_cx->arg_0 ^ arg_dx) < 7) {
	ATOMIC (arg_cx->arg_0++);
      }


      /* Not sure how object is passed */
      ObjDereferenceObject(object);

      return;
}

/*
.text:00429C2A ; =============== S U B R O U T I N E =======================================
.text:00429C2A
.text:00429C2A ; Attributes: bp-based frame
.text:00429C2A
.text:00429C2A                 public KeInitializeQueue
.text:00429C2A KeInitializeQueue proc near             ; CODE XREF: sub_4B7590+6Fp
.text:00429C2A                                         ; sub_5D0EB9+B3p ...
.text:00429C2A
.text:00429C2A arg_0           = dword ptr  8
.text:00429C2A arg_4           = dword ptr  0Ch
.text:00429C2A
 1 .text:00429C2A                 mov     edi, edi
 2 .text:00429C2C                 push    ebp
 3 .text:00429C2D                 mov     ebp, esp
 4 .text:00429C2F                 mov     eax, [ebp+arg_0]
 5 .text:00429C32                 and     dword ptr [eax+4], 0
 6 .text:00429C36                 mov     byte ptr [eax], 4
 7 .text:00429C39                 mov     byte ptr [eax+2], 0Ah
 8 .text:00429C3D                 lea     ecx, [eax+8]
 9 .text:00429C40                 mov     [ecx+4], ecx
10 .text:00429C43                 mov     [ecx], ecx
11 .text:00429C45                 lea     ecx, [eax+10h]
12 .text:00429C48                 mov     [ecx+4], ecx
13 .text:00429C4B                 mov     [ecx], ecx
14 .text:00429C4D                 lea     ecx, [eax+20h]
15 .text:00429C50                 mov     [ecx+4], ecx
16 .text:00429C53                 mov     [ecx], ecx
17 .text:00429C55                 mov     ecx, [ebp+arg_4]
18 .text:00429C58                 and     dword ptr [eax+18h], 0
19 .text:00429C5C                 test    ecx, ecx
20 .text:00429C5E                 jz      short loc_429C67
21 .text:00429C60
22 .text:00429C60 loc_429C60:                             ; CODE XREF: KeInitializeQueue+44j
23 .text:00429C60                 mov     [eax+1Ch], ecx
24 .text:00429C63                 pop     ebp
25 .text:00429C64                 retn    8
26 .text:00429C67 ; --------------------------------------------------------------------------
27 .text:00429C67                 movsx   ecx, ds:KeNumberProcessors
28 .text:00429C6E                 jmp     short loc_429C60


lines 3/4 function prologue.
lines 4/5 set to zero the second 4 bytes pointed by arg_0
lines 5/6 set byte pointed from arg_0 to 4
lines 6/7 set bye pointed by arg_0+2 to 16

lines 8/17 Initialize three structure at eax+0x8, eax+0x10, eax+0x20 respectly.
           The two fields in the structures are inizialized at their very same address.

line 18 eax+0x18 are zeroed
line 19/28 set eax+0x1C to args_4 if not null,  to ds:KeNumberProcessor if arg_4 is null

So arg_0 should be a struct like

struct Q {
     char[2] q_0;
     char[2] q_2;
     INT32 q_4;
     PTRLIST ql_8;
     char unk[4];
     PTRLIST ql_10;
     char unk[4];
     INT32 q_18;
     INT32/PTR32 q_1c;
     PTRLIST ql_20;
} *PTRQ;

and

typdef struct LIST {
    PTRLIST l_0;
    PTRLIST l_4;
} *PTRLIST;


 */

void KeInitializeQueue(PTRQ arg_0,INT32 arg_4) {
  arg_0->q_0[0] = 4;
  arg_0->q_0[2] = 16;

  arg_0->q_4 = 0;

  PTRLIST l = arg_0->ql_8;
  l->l_0 = l;
  l->l_1 = l;

  l = arg_0->ql_10;
  l->l_0 = l;
  l->l_1 = l;

  l = arg_0->ql_20;
  l->l_0 = l;
  l->l_1 = l;

  q->q_18 = arg_4 ? arg_4 : ds:KeNumberProcessors;

  return;

}

