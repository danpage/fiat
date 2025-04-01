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
board type:
this allows the target implementation to be built *and* executed on
the development platform itself with no real dependencies
(e.g., with an installation of [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) already available),
and thus offers a way to prototype and debug both target and client 
implementations.

<!--- ==================================================================== --->

## Step 3: develop target implementation

1. We
   configure the target implementation
   by editing

   ```sh
   ${FIAT_PATH_REPO}/src/fiat/target/kernel/imp/kernel_imp.conf
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
   ${FIAT_PATH_REPO}/src/fiat/target/kernel/imp/kernel_imp.c
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
   `c` will be computed as `k ^ m` (i.e., XOR'ing `k` and `m` together).

<!--- -------------------------------------------------------------------- --->

## Step 4: build   target implementation

- *Either*
  execute

  ```sh
  FIAT_CONTEXT="native" FIAT_DRIVER="binary" FIAT_BOARD="native" make target/clean target/build
  ```
  
  to use the `binary` driver,
  *or*

  ```sh
  FIAT_CONTEXT="native" FIAT_DRIVER="text"   FIAT_BOARD="native" make target/clean target/build
  ```

  to use the `text`   driver.

<!--- -------------------------------------------------------------------- --->

## Step 5: use     target implementation

To drive interaction with the target implementation, we use an example
client implementation located in

```sh
${FIAT_PATH_REPO}/src/fiat/client/script/example.py
```

whose core functionality is captured by the following fragment:

```{eval-rst}
.. literalinclude:: ../src/fiat/client/script/example.py
   :language: Python
   :linenos:
   :lines: 7-43
   :lineno-start: 7
```

Note that each line which issues a request does not, but ideally would
check the associated acknowledgement: execution simply continues under 
the assumption that each acknowledgement indicates success.
A line-by-line overview reads as follows:

- line  14
   opens the client (or connection),
- line  16
  issues a           `reset`  request,
- lines 18 to  20
  issue  a series of `nameof` requests
  to query the identifier     of `c`, `k`, and `m` registers,
- lines 22 to  24
  issue  a series of `sizeof` requests
  to query the allocated size of `c`, `k`, and `m` registers,
- lines 26 to 28
  issue  a series of `typeof` requests
  to query the           type of `c`, `k`, and `m` registers (including, e.g., the read- and write-access),
- lines 30 and 31
  generate random, 16-byte values for `k` and `m`,
- lines 33 and 34
  issue  a series of `wr`     requests
  to write (i.e., transfer from client to target) `k` and `m`,
- line  36
  issues a           `kernel` request
  to invoke the kernel function which computes `c` from `k` and `m`,
- line  38
  issues a           `rd`     request
  to read  (i.e., transfer from client to target)         `c`,
- lines 40 and 41
  print each byte of `c` and `k ^ m` to check whether the value read matches that expected,
- line  43
  closes the client (or connection).

Now we can execute the target and client, and interaction between them:

- In terminal 1, 
  execute

  ```sh
  ncat --verbose --listen --source-port 1234 --hex-dump ${FIAT_PATH_REPO}/build/target/native/target.ncat --exec ${FIAT_PATH_REPO}/build/target/native/target.elf
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
  `${FIAT_PATH_REPO}/build/target/native/target.ncat`

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
  python3 ${FIAT_PATH_REPO}/src/fiat/client/script/example.py --libfiat-device='socket' --libfiat-driver='binary' --libfiat-host='127.0.0.1' --libfiat-port='1234'
  ```

  to use the `binary` driver,
  *or*

  ```sh
  python3 ${FIAT_PATH_REPO}/src/fiat/client/script/example.py --libfiat-device='socket' --libfiat-driver='text'   --libfiat-host='127.0.0.1' --libfiat-port='1234'
  ```

  to use the `text`   driver.

Using either driver, we expect an output similar to

```sh
nameof( 0x00 ) = c
nameof( 0x01 ) = k
nameof( 0x02 ) = m
sizeof( 0x00 ) = 16
sizeof( 0x01 ) = 16
sizeof( 0x02 ) = 16
typeof( 0x00 ) = 0x02 => wr=0, rd=1, length=0
typeof( 0x01 ) = 0x01 => wr=1, rd=0, length=0
typeof( 0x02 ) = 0x01 => wr=1, rd=0, length=0
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

noting that despite use of
`random.randbytes( 16 )`
to generate `k` and `m`, they, and thus `c`, should match the above due 
to the fixed seed provided via
`random.seed( 0 )`.
Either way, the important feature is that each
`c[i]`
received from the target implementation matches the
`k[i] ^ m[i]`
we compute locally: this demonstrates it is operating as expected.

- From use of the `binary` driver,
  `${FIAT_PATH_REPO}/build/target/native/target.ncat`
  reads as follows:

  ```

  ```

- From use of the `text`   driver,
  `${FIAT_PATH_REPO}/build/target/native/target.ncat`
  reads as follows:

  ```

  ```

<!--- ==================================================================== --->
