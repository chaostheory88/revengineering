        section .text
        global _start
_start:
        push 0xAABBCCDD
        call alter_me
alter_me:
        push ebp
        mov ebp,esp
        mov esp,ebp
        pop ebp
        pop ebx
        ret
