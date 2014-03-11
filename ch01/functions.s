%macro PROLOGUE 0
        push ebp
        mov ebp, esp
%endmacro

%macro EPILOGUE 0
        leave
        ret
%endmacro

%macro ALLOCATE 1
        sub esp, %1
%endmacro

%macro DEALLOCATE 1
        add esp, %1
%endmacro

        section .text
        global _strlen,_strchr,_memcpy,_memset,_strcmp,_strset

        ;; size_t strlen(const char *s)
_strlen:
        PROLOGUE
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
        PROLOGUE
        push edi
        mov eax, [ebp+12]
        mov edi, [ebp+8]
__start_strchr_loop:
        scasb
        jz __end_strchr_loop
        jmp __start_strchr_loop
__end_strchr_loop:
        lea eax, [edi-0x1]
        pop edi
        EPILOGUE

        ;; void *memcpy(void *dest, const void *src, size_t n)
_memcpy:
        PROLOGUE
        push edi
        push esi
        push ecx
        mov ecx, [ebp+16]
        mov esi, [ebp+12]
        mov edi, [ebp+8]
__start_memcpy_loop:
        lodsb
        stosb
        loop __start_memcpy_loop
__end_memcpy_loop:
        lea eax, [ebp+8]
        pop ecx
        pop esi
        pop edi
        EPILOGUE

        ;; void *memset(void *s, int c, size_t n)
_memset:	
        
        ;; int strcmp(const char *s1, const char *s2)
_strcmp:

        ;; char *strset(const char *str, char c)
_strset:
        