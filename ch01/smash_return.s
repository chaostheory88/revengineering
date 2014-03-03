        section .text
        global _start
_start:
        call smash_return
        mov ecx, eax
        mov eax, 0x1
        mov ebx, 0x0
        int 0x80
smash_return:
        push ebp
        mov ebp, esp
        mov eax, 0xFFFFFFFF
        add eax, 0x3
        mov esp, ebp
        pop ebp
        ret