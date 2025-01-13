# Example

Consider a typical 128-bit 
[block cipher](https://en.wikipedia.org/wiki/Block_cipher),
which produces a
a 128-bit ciphertext `c`
by encrypting
a 128-bit plaintext  `m`
using
a 128-bit cipher key `k`.
This example demonstrates how a FIAT-supported target implementation
reflecting such a block cipher, or at least a trivial proxy for one,
could be developed and used.  To do so, we use the `native` 
build context 
and 
board type
this allows the target implementation to be built *and* executed on
the development platform itself with no real dependencies
(e.g., with an already available [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)),
and thus offers a way to prototype and debug both target and client 
implementations.

<!--- ==================================================================== --->

## Step 3: develop target implementation

1. We
   configure the target implementation
   by editing

   ```sh
   ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.conf
   ```

   In particular,
   we replace the existing register declarations with

   ```c
   DECLARE_GPR(c, 16, {}, .wr = false, .rd =  true, .length = KERNEL_REG_LENGTH_FIX)
   DECLARE_GPR(k, 16, {}, .wr =  true, .rd = false, .length = KERNEL_REG_LENGTH_FIX)
   DECLARE_GPR(m, 16, {}, .wr =  true, .rd = false, .length = KERNEL_REG_LENGTH_FIX)
   ```

   Doing so means

   - `c` is a fixed-length, 16-byte RO (or output only) GPR with address `0x00 = 0`,
   - `k` is a fixed-length, 16-byte WO (or  input only) GPR with address `0x01 = 1`,
     and
   - `m` is a fixed-length, 16-byte WO (or  input only) GPR with address `0x02 = 2`.

2. We
   complete the target implementation
   by editing

   ```sh
   ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.c
   ```

   In particular,
   we replace the kernel function with

   ```c
   ret_t kernel( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
     byte* c = gpr[ 0 ].data;
     byte* k = gpr[ 1 ].data;
     byte* m = gpr[ 2 ].data;
   
     for( int i = 0; i < 16; i++ ) {
       c[ i ] = k[ i ] ^ m[ i ];
     }
   
     return EXIT_SUCCESS;
   }
   ```

   Doing so means
   `c` will be computed `k ^ m` (i.e., XOR'ing `k` and `m` together).

<!--- -------------------------------------------------------------------- --->

## Step 4: build   target implementation

- *Either*
  execute

  ```sh
  CONTEXT="native" BOARD="native" DRIVER="binary" make target/clean target/build
  ```
  
  to use the `binary` driver,
  *or*

  ```sh
  CONTEXT="native" BOARD="native" DRIVER="text"   make target/clean target/build
  ```

  to use the `text`   driver.

<!--- -------------------------------------------------------------------- --->

## Step 5: use     target implementation

To drive interaction with the target implementation, we'll use a client 
implementation located in

```sh
${REPO_HOME}/src/fiat/client/example/example.py
```

whose core functionality is captured by the following fragment:

```{eval-rst}
.. literalinclude:: ../src/fiat/client/example/example.py
   :language: Python
   :linenos:
   :lines: 33-60
   :lineno-start: 33
```

Note that each line which issues a request does not, but ideally would
check the associated acknowledgement: execution simply continues under 
the assumption that each acknowledgement indicates success.
A line-by-line overview reads as follows:

- line  33
  issues a           `reset`  request,
- lines 35 to  40
  issue  a series of `sizeof` requests
  to check the allocated size of the `c`, `k`, and `m` registers,
- lines 42 to  47
  issue  a series of `typeof` requests
  to check the           type of the `c`, `k`, and `m` registers (including, e.g., the read- and write-access),
- lines 49 and 50
  generate random, 16-byte values for `k` and `m`,
- lines 52 and 53
  issue  a series of `wr`     requests
  to write (i.e., transfer from client to target) `k` and `m`,
- line  55
  issues a           `kernel` request
  to invoke the kernel function which computes `c` from `k` and `m`,
- line  57
  issues a           `rd`     request
  to read  (i.e., transfer from client to target)         `c`,
- lines 59 and 60
  print each byte of `c` and `k ^ m` to check whether the value read matches that expected.

Now we can execute the target and client, and interaction between them:

- In terminal 1, 
  execute

  ```sh
  ncat --verbose --listen --source-port 1234 --hex-dump ${REPO_HOME}/build/target/native/target.ncat --exec ${REPO_HOME}/build/target/native/target.elf
  ```

  Doing so uses 
  [ncat](https://en.wikipedia.org/wiki/Netcat)
  to execute the target implementation, and connect
  the associated `stdin` and `stdout` 
  [standard streams](https://en.wikipedia.org/wiki/Standard_streams)
  to a (local)
  [network socket](https://en.wikipedia.org/wiki/Network_socket)
  we can connect to.  Notice that use of 
  `--hex-dump`
  means that all communicated data will be captured in the file
  `${REPO_HOME}/build/target/native/target.ncat`

- In terminal 2, 
  install
  [`libfiat`](https://pypi.org/project/libfiat)

  ```sh
  pip3 install libfiat
  ```

  then
  *either*
  execute

  ```sh
  python3 ${REPO_HOME}/src/fiat/client/example/example.py --device='socket' --driver='binary' --host='127.0.0.1' --port='1234' --seed='0'
  ```

  to use the `binary` driver,
  *or*

  ```sh
  python3 ${REPO_HOME}/src/fiat/client/example/example.py --device='socket' --driver='text'   --host='127.0.0.1' --port='1234' --seed='0'
  ```

  to use the `text`   driver.

Using either driver, we expect an output similar to
  
```sh
sizeof( GPR_C ) = 16
sizeof( GPR_K ) = 16
sizeof( GPR_M ) = 16
typeof( GPR_C ) = 02 => wr=0, rd=1, length=0
typeof( GPR_K ) = 01 => wr=1, rd=0, length=0
typeof( GPR_M ) = 01 => wr=1, rd=0, length=0
i =  0 : c[i] = 0x98, k[i] ^ m[i] = 0x98
i =  1 : c[i] = 0x93, k[i] ^ m[i] = 0x93
i =  2 : c[i] = 0x86, k[i] ^ m[i] = 0x86
i =  3 : c[i] = 0xB3, k[i] ^ m[i] = 0xB3
i =  4 : c[i] = 0x8A, k[i] ^ m[i] = 0x8A
i =  5 : c[i] = 0x40, k[i] ^ m[i] = 0x40
i =  6 : c[i] = 0xC2, k[i] ^ m[i] = 0xC2
i =  7 : c[i] = 0x68, k[i] ^ m[i] = 0x68
i =  8 : c[i] = 0x96, k[i] ^ m[i] = 0x96
i =  9 : c[i] = 0x12, k[i] ^ m[i] = 0x12
i = 10 : c[i] = 0x41, k[i] ^ m[i] = 0x41
i = 11 : c[i] = 0x80, k[i] ^ m[i] = 0x80
i = 12 : c[i] = 0x78, k[i] ^ m[i] = 0x78
i = 13 : c[i] = 0xB2, k[i] ^ m[i] = 0xB2
i = 14 : c[i] = 0xCF, k[i] ^ m[i] = 0xCF
i = 15 : c[i] = 0x14, k[i] ^ m[i] = 0x14
```
  
noting that `k` and `m` and thus `c` are randomised per execution, but
should match the above due to use of the `--seed` option.  Either way,
the important feature is that each
`c[i]`
received from the target implementation matches the
`k[i] ^ m[i]`
we compute locally: this demonstrates it is operating as expected.

- From use of the `binary` driver,
  `${REPO_HOME}/build/target/native/target.ncat`
  reads as follows:

  ```
  [0000]   01                                                  .
  [0000]   00                                                  .
  [0000]   02 00                                               ..
  [0000]   00 10                                               ..
  [0000]   02 01                                               ..
  [0000]   00 10                                               ..
  [0000]   02 02                                               ..
  [0000]   00 10                                               ..
  [0000]   04 00                                               ..
  [0000]   00 02                                               ..
  [0000]   04 01                                               ..
  [0000]   00 01                                               ..
  [0000]   04 02                                               ..
  [0000]   00 01                                               ..
  [0000]   05 01 10 CD 07 2C D8 BE   6F 9F 62 AC 4C 09 C2 82   .....,.. o.b.L...
  [0010]   06 E7 E3                                            ...
  [0000]   00                                                  .
  [0000]   05 02 10 55 94 AA 6B 34   2F 5D 0A 3A 5E 48 42 FA   ...U..k4 /].:^HB.
  [0010]   B4 28 F7                                            .(.
  [0000]   00                                                  .
  [0000]   07 00 01                                            ...
  [0000]   00                                                  .
  [0000]   06 00                                               ..
  [0000]   00 10 98 93 86 B3 8A                                .......
  [0000]   40 C2 68 96 12 41 80 78   B2 CF 14                  @.h..A.x ...
  ```

- From use of the `text`   driver,
  `${REPO_HOME}/build/target/native/target.ncat`
  reads as follows:

  ```
  [0000]   2A 0D                                               *.
  [0000]   2B 0D                                               +.
  [0000]   7C                                                  |
  [0000]   20 30 30 0D                                          00.
  [0000]   2B 20 31 30 0D                                      + 10.
  [0000]   7C                                                  |
  [0000]   20 30 31 0D                                          01.
  [0000]   2B                                                  +
  [0000]   20                                                   
  [0000]   31                                                  1
  [0000]   30                                                  0
  [0000]   0D                                                  .
  [0000]   7C                                                  |
  [0000]   20 30 32 0D                                          02.
  [0000]   2B 20 31 30 0D                                      + 10.
  [0000]   3F                                                  ?
  [0000]   20 30 30 0D                                          00.
  [0000]   2B 20 30 32 0D                                      + 02.
  [0000]   3F                                                  ?
  [0000]   20 30 31 0D                                          01.
  [0000]   2B 20 30 31 0D                                      + 01.
  [0000]   3F                                                  ?
  [0000]   20 30 32 0D                                          02.
  [0000]   2B 20 30 31 0D                                      + 01.
  [0000]   3E                                                  >
  [0000]   20 30 31 20 31 30 20 63   64 30 37 32 63 64 38 62    01 10 c d072cd8b
  [0010]   65 36 66 39 66 36 32 61   63 34 63 30 39 63 32 38   e6f9f62a c4c09c28
  [0020]   32 30 36 65 37 65 33 0D                             206e7e3. 
  [0000]   2B 0D                                               +.
  [0000]   3E                                                  >
  [0000]   20 30 32 20 31 30 20 35   35 39 34 61 61 36 62 33    02 10 5 594aa6b3
  [0010]   34 32 66 35 64 30 61 33   61 35 65 34 38 34 32 66   42f5d0a3 a5e4842f
  [0020]   61 62 34 32 38 66 37 0D                             ab428f7. 
  [0000]   2B 0D                                               +.
  [0000]   3D                                                  =
  [0000]   20 30 30 20 30 31 0D                                 00 01.
  [0000]   2B 0D                                               +.
  [0000]   3C                                                  <
  [0000]   20 30 30 0D                                          00.
  [0000]   2B 20 31 30 20 39 38                                + 10 98
  [0000]   39 33 38 36 42 33 38 41   34 30 43 32 36 38 39 36   9386B38A 40C26896
  [0010]   31 32 34 31 38 30 37 38   42 32 43 46 31 34 0D      12418078 B2CF14.
  ```

<!--- ==================================================================== --->
