# NYCU 2022-Spring Operating System Homework - Fast File Duplication

## Overhead of File Reading or Writing

- Switching from the user mode to the kernel mode
- Copying data from the storage to kernel pages
- Copying data from the kernel pages to user buffer
- Switching from the kernel mode to the user mode

## Testing Procedure

- Pre-conditioning

  ```bash
  mount -t tmpfs -o size=3G tmpfs <mountpoint>
  swapoff -a && swapon -a
  echo 3 > /proc/sys/vm/drop_caches
  ```

- Run program

- Cleaning-up

  ```bash
  diff source destination
  ```
