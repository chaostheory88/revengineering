        section .text
        global _start
_start:
        call alter_me
        mov eax, 0x1
        mov ebx, 0x0
        int 0x80
alter_me:
        push ebp
        mov ebp, esp
        mov esp, ebp
        pop ebp
        push 0xAABBCCDD
        ret