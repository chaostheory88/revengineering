%macro PROLOGUE 1
        push ebp
        mov ebp, esp
        sub esp, %1
%endmacro

%macro EPILOGUE 0
        leave
        ret
%endmacro

        section .text
        global _strlen,_strchr,_memcpy,_memset,_strcmp,_strset

        ;; size_t strlen(const char *s)
_strlen:
        PROLOGUE 0
        push ecx
        push edi
        xor ecx, ecx
        mov edi, [ebp+8]
        mov al, 0x0 
__start_strlen_loop:
        scasb
        jz __end_strlen_loop
        inc ecx
        jmp __start_strlen_loop
__end_strlen_loop:
        mov eax, ecx
        pop edi
        pop ecx
        EPILOGUE
        
        ;; char *strchr(const char *s, int c)
_strchr:

        ;; void *memcpy(void *dest, const void *src, size_t n)
_memcpy:

        ;; void *memset(void *s, int c, size_t n)
_memset:	
        
        ;; int strcmp(const char *s1, const char *s2)
_strcmp:

        ;; char *strset(const char *str, char c)
_strset:
        