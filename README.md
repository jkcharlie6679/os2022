# NYCU 2022-Spring Operating System Homework - Fast File Duplication

## Overhead of File Reading or Writing

- Switching from the user mode to the kernel mode
- Copying data from the storage to kernel pages
- Copying data from the kernel pages to user buffer
- Switching from the kernel mode to the user mode

## Testing Procedure

- Pre-conditioning

  ```bash
  mkdir /tmp/ramdisk # make the mountpoint
  fallocate -l 1G /tmp/ramdisk/test.img # create the large file for duplicate
  sudo mount -t tmpfs -o size=3G tmpfs /tmp/ramdisk
  sudo swapoff -a && sudo swapon -a # clear swap
  sudo sh -c "echo 3 > /proc/sys/vm/drop_caches" # clear pagecache, dentries, and inodes
  ```

- Run program

- Cleaning-up

  ```bash
  diff <source> <destination>
  ```

## Help

- Show mount point
  
  ```bash
  cat /proc/mounts
  ```

- Calculate the time

  ```bash
  time <cmd>
  ```

- Unmount

  ```bash
  umount <dir>
  ```
