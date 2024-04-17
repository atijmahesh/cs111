## UID: 905954423

## Pipe Up

This program creates a series of processes to execute command-line arguments as piped commands, managing input and output redirection between them, and handling errors related to process creation and command execution.

## Building
To Build:
```shell
make
```

## Running

Example Run:
```shell
./pipe ls sort uniq wc
```
Output:
Lists the unique sorted file and directory names in the current directory, counting how many unique items exist.
In the case of running it in this folder, the output is: 7 7 63

## Cleaning up
To Clean Up:
```shell
make clean
```
