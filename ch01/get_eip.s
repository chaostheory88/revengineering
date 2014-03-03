        section .text
        global _start
_start:
        call get_eip
        mov eax, 0x1
        mov ebx, 0x0
        int 0x80
get_eip:
        push ebp
        mov ebp, esp
        mov ebx, [esp+4]
        leave
        ret

        