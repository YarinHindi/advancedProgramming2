

# Threadpool Mechanism for Encryption Algorithm

This program implements a threadpool mechanism to parallelize an encryption algorithm, which is not very fast and can only encrypt/decrypt up to 1024 bytes of data. The aim is to improve performance by utilizing multi-cores.

## Usage

The program is run from the command line and takes input from `stdin`. The encrypted/decrypted output is then sent to `stdout`. The following command line arguments are used:

- `-e`: used for encryption
- `-d`: used for decryption

Usage examples:
```
coder key -e < my_original_file > encrypted_file
coder key -d < encrypted_file > my_original_file
```

## Building the Program

The program requires the following libraries to be installed:

- pthread
- priorityQ

To build the program, use the following command:
```
run make it will build your program.
right after that tester exe file will show on your lib and this is the file you shold run.
```

## How it Works

The program uses a threadpool to parallelize the encryption/decryption process. The `ThreadPool` struct is defined to hold a buffer of `Data` structs, which contain the input data, the encryption/decryption key, and a unique number for each data element. The `thread_pool_init()` function initializes the threadpool and its mutexes and condition variables. The `thread_pool_enqueue()` function adds a `Data` element to the threadpool buffer, and the `thread_pool_dequeue()` function retrieves a `Data` element from the buffer. The `worker_thread()` function is executed by each thread in the threadpool, and it retrieves data from the buffer, encrypts/decrypts it, and then adds it to the `PriorityQueue` buffer using the `enqueue()` function.

## Notes

- The program assumes that the encryption and decryption functions are implemented in the `codec.h` library, which is compiled for x86.
- The program assumes that the input data is in `stdin` and the output data is in `stdout`.
- The program assumes that the number of cores available on the machine is sufficient to maximize performance.