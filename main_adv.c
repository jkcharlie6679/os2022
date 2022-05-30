#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <string.h>

#define BUF_SIZE 1024

void copy(const char *src_path, const char *dst_path);
void zero_copy(const char *src_path, const char *dst_path);
void copy_base(const char *src_path, const char *dst_path);

int main (int argc, char** argv){
  if (argc != 4) {
    printf("usage: %s <source> <destination> <mode>\n", argv[0]);
    exit(1);
  }
  if(strcmp(argv[3], "1") == 0) {
    copy_base(argv[1], argv[2]);
  } else if(strcmp(argv[3], "2") == 0){
    copy(argv[1], argv[2]);
  }else {
    zero_copy(argv[1], argv[2]);
  }
  return 0;
}

void zero_copy(const char *src_path, const char *dst_path) {
  int src;
  int dst;
  struct stat stat_buf;
  off_t offset = 0;
  int rc;

  src = open(src_path, O_RDONLY);   // open the source file
  fstat(src, &stat_buf);            // get the source file's information
  dst = open(dst_path, O_WRONLY|O_CREAT, stat_buf.st_mode);   // create the destination file
  rc = sendfile (dst, src, &offset, stat_buf.st_size);
  if (rc != stat_buf.st_size) {
    printf("incomplete transfer from sendfile: %d of %d bytes\n", rc, (int)stat_buf.st_size);
    exit(1);
  }

  close(dst);
  close(src);
}

void copy(const char *src_path, const char *dst_path) {
  FILE *src, *dst;
  char buffer[BUF_SIZE];
  size_t length;

  src = fopen(src_path, "rb");
  dst = fopen(dst_path, "wb");
  
  while(!feof(src)) {
    length = fread(buffer, 1, BUF_SIZE, src);
    fwrite(buffer, 1, length, dst);
  }

  fclose(src);
  fclose(dst);
}

void copy_base(const char *src_path, const char *dst_path) {
  int fd_in = open(src_path, O_RDONLY); 
  if(fd_in == -1){
    perror("readwrite: open"); 
    return;
  }
  int fd_out = creat(dst_path, 0644); 
  if(fd_out == -1){
    perror("readwrite: creat");
    return;
  }
  char buf[BUF_SIZE]; 
  size_t readn; 
  while(1){
    readn = read(fd_in, buf, BUF_SIZE); 
    if(readn <= 0){
      break;
    }
    while(readn){
      size_t writen = write(fd_out, buf, readn); 
      if(writen == -1){
        perror("readwrite: write"); 
        return;
      }
      readn -= writen; 
    }
  }
  if(readn == -1){ 
    perror("readwrite: read"); 
    return;
  }
  close(fd_in); 
  close(fd_out); 
}
