%macro PROLOGUE 1
        push ebp
        mov ebp, esp
        sub esp, %1
%endmacro

%macro EPILOGUE
        pop ebp
        leave
        ret
%endmacro

        section .text
        global strlen,strchr,memcpy,memset,strcmp,strset

        ;; size_t strlen(const char *s)
_strlen:
        
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
        