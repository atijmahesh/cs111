# Hey! I'm Filing Here

In this lab, I successfully implemented the following: creation of the superblock, block group descriptor table, block bitmap, inode bitmap, and inode table. Additionally, I created and wrote the root directory block, the lost+found directory block, and the hello_world file block. I ensured that the ext2 filesystem image (`cs111-base.img`) was correctly initialized and populated with the required structures and data, adhering to the specified permissions and ownership settings. Finally, I verified the integrity of the filesystem using `fsck.ext2` to ensure there were no errors.

## Building
To build, run
```shell
make
```

## Running

To setup cs111-base.img, run
```shell
./ext2-create
```
To check file system, run
```shell
dumpe2fs cs111-base.img
fsck.ext2 cs111-base.img
```
To mount file system, run
```shell
mkdir mnt
sudo mount -o loop cs111-base.img mnt
```

## Cleaning up
To remove directory and unmount file system, run
```shell
sudo umount mnt
rmdir mnt
```
To clean up binary files, run
```shell
make clean
```
