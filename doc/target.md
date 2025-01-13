# Target support

<!--- ==================================================================== --->

(target-build)=
## Build system

### Overview

`${REPO_HOME}/Makefile` can be used to drive the build process, e.g., via

| Command                  | Description                                                    |
|:-------------------------|:---------------------------------------------------------------|
| `make   venv/clean`      | clean                  the Python virtual environment          |
| `make   venv/build`      | build                  the Python virtual environment          |
| `make    doc/clean`      | clean [Sphinx](https://www.sphinx-doc.org)-based documentation |
| `make    doc/build`      | build [Sphinx](https://www.sphinx-doc.org)-based documentation |
| `make target/clean`      | clean                  the target implementation               |
| `make target/build`      | build                  the target implementation               |
| `make target/fetch-deps` | fetch (i.e., download) the target implementation dependencies  |
| `make target/patch-deps` | patch                  the target implementation dependencies  |
| `make target/build-deps` | build                  the target implementation dependencies  |
| `make target/clean-deps` | clean                  the target implementation dependencies  |
| `make clean`             |  selectively remove content built in `${REPO_HOME}/build`      |
| `make spotless`          | aggressively remove content built in `${REPO_HOME}/build`      |

with configuration options for it outlined in the following.

### The `CONTEXT` environment variable
    
The
`CONTEXT`
environment variable specifies the
build context
within which the build process is executed.
The options are

1. `native` is a        native build context,
2. `docker` is a containerised build context,

noting that:

- Using the
  `native`
  build context implies
  the tool-chain,
  and
  any software dependencies (e.g., libraries)
  need to be installed manually.

- Using the
  `docker`
  build context implies
  the tool-chain,
  and
  any software dependencies (e.g., libraries)
  are installed within the associated container.
  Note that each platform-specific 
  [Docker](https://www.docker.com)
  image is built using the content housed in
  `${REPO_HOME}/src/docker`.
  However,
  *there is no need to do this manually*:
  a pre-built image can (and will) be pulled from
  [Docker Hub](https://hub.docker.com/u/danpage)
  by the build process as needed.

### The `DRIVER`  environment variable

The
`DRIVER`
environment variable specifies the
[driver type](#target-driver).
The options are

1. `binary` uses a binary protocol,
2. `text`   uses a   text protocol.

### The `BOARD`   environment variable

The
`BOARD`
environment variable specifies the
[board  type](#target-board).
The options are

| `BOARD`                | Class      | Family                          | Core                                                    | Model                                                                          | ISA      | Host                                                                              |
|:-----------------------|:-----------|:--------------------------------|:--------------------------------------------------------|:-------------------------------------------------------------------------------|:---------|:----------------------------------------------------------------------------------|
| `native`               | Simulation |                                 |                                                         |                                                                                |          |                                                                                   |
| `cw308/stm32f071rbt6`  | ASIC       |    [ARM](https://www.arm.com)   | [Cortex-M0](https://en.wikipedia.org/wiki/ARM_Cortex-M) | [STM32F071RBT6](https://www.st.com/resource/en/datasheet/stm32f071cb.pdf)      | ARMv6-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |
| `cw308/stm32f100rbt6b` | ASIC       |    [ARM](https://www.arm.com)   | [Cortex-M3](https://en.wikipedia.org/wiki/ARM_Cortex-M) | [STM32F100RBT6B](https://www.st.com/resource/en/datasheet/stm32f100cb.pdf)     | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |
| `cw308/stm32f215ret6`  | ASIC       |    [ARM](https://www.arm.com)   | [Cortex-M3](https://en.wikipedia.org/wiki/ARM_Cortex-M) | [STM32F215RET6](https://www.st.com/resource/en/datasheet/stm32f215re.pdf)      | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |
| `cw308/stm32f303rct7`  | ASIC       |    [ARM](https://www.arm.com)   | [Cortex-M4](https://en.wikipedia.org/wiki/ARM_Cortex-M) | [STM32F303RCT7](https://www.st.com/resource/en/datasheet/stm32f303rc.pdf)      | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |
| `cw308/stm32f405rgt6`  | ASIC       |    [ARM](https://www.arm.com)   | [Cortex-M4](https://en.wikipedia.org/wiki/ARM_Cortex-M) | [STM32F405RGT5](https://www.st.com/resource/en/datasheet/stm32f405rg.pdf)      | ARMv7E-M | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |
| `cw308/fe310-g002`     | ASIC       | [RISC-V](https://www.riscv.org) |  E31                                                    | [FE310-G002](https://www.sifive.com/document-file/freedom-e310-g002-datasheet) | RV32IMAC | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F) |

<!--- -------------------------------------------------------------------- --->

(target-kernel)=
## Kernel layer

### Overview

The kernel layer 
is comprised of 

1. an abstract interface and associated support
   captured by

   ```sh
   ${REPO_HOME}/src/fiat/target/kernel/kernel.h
   ${REPO_HOME}/src/fiat/target/kernel/kernel.c
   ```

2. a  concrete implementation
   captured by

   ```sh
   ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.h
   ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.c
   ```

   plus associated configuration

   ```sh
   ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.conf
   ```

noting that:

- The kernel state is reflected by a set of registers, each identified
  by an 8-bit index

  ```
   8 7 6 5 4 3 2 1
  +-+-+-+-+-+-+-+-+
  | | | | | | | | |
  +-+-+-+-+-+-+-+-+
   ^ \___________/
   |       |
   |       |
   |       +--------- address
   +-----------------   class : 0 = GPR, 1 = SPR
   ```

  meaning the set is divided into two classes (using the MSB):
  General-Purpose Registers (GPRs)
  are defined by the kernel,
  whereas
  Special-Purpose Registers (SPRs)
  are defined by the driver.

- Each register has an associated 8-bit type, i.e., some meta-data

   ```
   8 7 6 5 4 3 2 1
  +-+-+-+-+-+-+-+-+
  | | | | | | | | |
  +-+-+-+-+-+-+-+-+
             ^ ^ ^ 
             | | | 
             | | | 
             | | +---    read access : 0 = false, 1 = true
             | +-----   write access : 0 = false, 1 = true
             +------- content length : 0 = fixed, 1 = variable
   ```

  noting that the terms 
   Read-Only (RO), 
  Write-Only (WO), 
  and 
   Read-Write (RW),
  are used as a short-hand to describe read and/or write access cases.

- The standard SPRs defined are as follows:

  - index `0x80 = 128`, i.e., SPR address `0x00 = 0`
    is called  `id`: this is an RO register used for an identifier,
  - index `0x80 = 129`, i.e., SPR address `0x00 = 1`
    is called `ret`: this is an RO register used for a  return code,
  - index `0x81 = 130`, i.e., SPR address `0x01 = 2`
    is called `tsc`: this is an RO register used for a  time-stamp counter,
  - index `0x82 = 131`, i.e., SPR address `0x02 = 3`
    is called `rnd`: this is an WO register used for    externally supplied randomness.

### API

```{eval-rst}
.. c:autofunction:: kernel
  :file: target/kernel/kernel.h
.. c:autofunction:: kernel_prologue_major
  :file: target/kernel/kernel.h
.. c:autofunction:: kernel_epilogue_major
  :file: target/kernel/kernel.h
.. c:autofunction:: kernel_prologue_minor
  :file: target/kernel/kernel.h
.. c:autofunction:: kernel_epilogue_minor
  :file: target/kernel/kernel.h
```

<!--- -------------------------------------------------------------------- --->

(target-driver)=
## Driver layer

### Overview

The driver layer
essentially implements the interface between
the client (i.e., the user)
and
the kernel (i.e., the functionality being used, as part of the target implementation),
using features in and so support from the
the board layer.  For example, it is responsible for

- management of
  [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)-based
  [communication protocol](https://en.wikipedia.org/wiki/Communication_protocol), 
  that e.g., allows
  transfer of data into and from the kernel register set
  and
  invocation of kernel functionality,
  and
- management of
  [GPIO](https://en.wikipedia.org/wiki/General-purpose_input/output)
  pins to, e.g., act as a trigger signal.

Irrespective of driver type, the generic usage model is captured by

```
Client                                     Target
======================================================================
:
wr index, data           ------ cmd -----> kernel_gprs[ index ] = data
:                        <----- ack -----
:
kernel_prologue op       ------ cmd -----> kernel_prologue_major( op )
:                        <----- ack -----
:
kernel          op, rep  ------ cmd -----> kernel_prologue_minor( op ) -+
:                                          trigger = 1                  |   
:                                          kernel               ( op )  |-- repetition     0
:                                          trigger = 0                  |   
:                                          kernel_epilogue_minor( op ) -+
:
:                                          kernel_prologue_minor( op ) -+
:                                          trigger = 1                  |   
:                                          kernel               ( op )  |-- repetition     1
:                                          trigger = 0                  |   
:                                          kernel_epilogue_minor( op ) -+
:
:                                          :
:
:                                          kernel_prologue_minor( op ) -+
:                                          trigger = 1                  |   
:                                          kernel               ( op )  |-- repetition rep-1
:                                          trigger = 0                  |   
:                                          kernel_epilogue_minor( op ) -+
:                        <----- ack -----
:
kernel_epilogue op       ------ cmd -----> kernel_epilogue_major( op )
:                        <----- ack -----
:
rd index                 ------ cmd -----> data = kernel_gprs[ index ]
:                        <----- ack -----
:
======================================================================
```

where, read top-to-bottom, the idea is that

- the client
  issues the `wr`              command
  to write any  input data,
- the client (optionally)
  issues the `kernel_prologue` command
  to perform "major" initialisation,
- the client 
  issues the `kernel`          command
  to invoke the kernel some number of times; in each repetition, it

  - performs some "minor" initialisation,
  - sets the GPIO trigger pin to 1,
  - executes the the kernel itself,
  - sets the GPIO trigger pin to 0,
  - performs some "minor"   finalisation,

- the client (optionally)
  the `kernel_epilogue` command
  to perform "major"   finalisation,
- the client
  issues the `rd`              command
  to  read any output data.

### Notation

The following notation is used to specify a given protocol:

- communicated messages are structured using the following types:

  1. a `char`, i.e., an 8-bit word representing an unsigned integer,
  2. a `byte`, i.e., an 8-bit word representing an [ASCII](https://en.wikipedia.org/wiki/ASCII) character,
  3. a `n`-element `byte` sequence, with elements communicated in little-endian order,
  4. a variable-length unsigned integer `vint` (or [varint](https://en.wikipedia.org/wiki/Variable-length_quantity), which is itself a `byte` sequence,
  
- `<T:X>` denotes a field `X` of type `T`, 
- `[T:X]^n` denotes a field `X` which is a sequence whose `i`-th element `X[i]` is of type `T`, 
- if a field has a fixed value `Y`, it is specified as `<T:X>=Y`.
- `||` denotes concatenation.

### The `binary` driver

This driver implementation uses a 
[binary](https://en.wikipedia.org/wiki/Binary_protocol)
protocol, which prioritises efficiency.
Communication is
stream-based,
with request and acknowledge messages are represented using a
sequence of raw bytes.
Based on this, the following command set is supported:

- Ping.
  - `req` syntax: `<byte:req>=0x00`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`

- Reset.
  - `req` syntax: `<byte:req>=0x01`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`

- Query allocated size, or "`sizeof`" a register (measured in bytes).
  - `req` syntax: `<byte:req>=0x02 || <byte:index>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00 || <vint:size>`

- Query used      size, or "`usedof`" a register (measured in bytes).
  - `req` syntax: `<byte:req>=0x03 || <byte:index>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00 || <vint:size>`

- Query           type, or "`typeof`" a register.
  - `req` syntax: `<byte:req>=0x04 || <byte:index>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00 || <byte:type>`

- Write content into a register.
  - `req` syntax: `<byte:req>=0x05 || <byte:index> || <vint:size> || [byte:data]^size`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`

- Read  content from a register.
  - `req` syntax: `<byte:req>=0x06 || <byte:index>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00 || <vint:size> || [byte:data]^size`

- Execute the kernel.
  - `req` syntax: `<byte:req>=0x07 || <byte:op> || <vint:rep>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`
  - note: `op` is an operation identifier passed to the implementation, and `rep` is a repeat count

- Execute the kernel prologue (or "major" initialisation).
  - `req` syntax: `<byte:req>=0x08 || <byte:op>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`
  - note: `op` is an operation identifier passed to the implementation

- Execute the kernel epilogue (or "major"   finalisation).
  - `req` syntax: `<byte:req>=0x09 || <byte:op>`
  - `ack` syntax: 
    - on failure, `<byte:ack>=0x01` 
    - on success, `<byte:ack>=0x00`
  - note: `op` is an operation identifier passed to the implementation

### The `text`   driver

This driver implementation uses a 
[text](https://en.wikipedia.org/wiki/Text-based_protocol)
protocol, which prioritises simplicity (and maybe usability therefore):
in concept at least, it is *similar* to the ChipWhisperer 
[SimpleSerial](https://chipwhisperer.readthedocs.io/en/latest/simpleserial.html) 
protocol.
Communication is
  line-based,
with request and acknowledge messages are represented using a
sequence of 
[ASCII](https://en.wikipedia.org/wiki/ASCII)
characters terminated with a 
[Carriage Return (CR)](https://en.wikipedia.org/wiki/Newline)
only; a `byte` is represented by 2 hexadecimal characters
(per the [`printf`](https://en.wikipedia.org/wiki/Printf) format specifier `%02X`).
Based on this, the following command set is supported:

- Ping.
  - `req` syntax: `<char:req>='!'`
  - `ack` syntax: 
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

- Reset.
  - `req` syntax: `<char:req>='*'`
  - `ack` syntax: 
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

- Query allocated size, or "`sizeof`" a register (measured in bytes).
  - `req` syntax: `<char:req>='|' <byte:index>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+' <vint:size>`

- Query used      size, or "`usedof`" a register (measured in bytes).
  - `req` syntax: `<char:req>='#' <byte:index>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+' <vint:size>`

- Query           type, or "`typeof`" a register.
  - `req` syntax: `<char:req>='?' <byte:index>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+' <byte:type>`

- Write content into a register.
  - `req` syntax: `<char:req>='>' <byte:index> <vint:size> [byte:data]^size`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

- Read content from a register.
  - `req` syntax: `<char:req>='<' <byte:index>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+' <vint:size> [byte:data]^size`

- Execute the kernel.
  - `req` syntax: `<char:req>='=' <byte:op> <vint:rep>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

- Execute the kernel prologue (or "major" initialisation).
  - `req` syntax: `<char:req>='[' <byte:op>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

- Execute the kernel epilogue (or "major"   finalisation).
  - `req` syntax: `<char:req>=']' <byte:op>`
  - `ack` syntax:
    - on failure, `<char:ack>='-'`
    - on success, `<char:ack>='+'`

<!--- -------------------------------------------------------------------- --->

(target-board)=
## Board  layer

### Overview

The board  layer 
is comprised of 

1. an abstract interface and associated support
   captured by

   ```sh
   ${REPO_HOME}/src/fiat/target/board/board.h
   ${REPO_HOME}/src/fiat/target/board/board.c
   ```

2. a  concrete implementation
   captured by

   ```sh
   ${REPO_HOME}/src/fiat/target/board/imp/board_imp.h
   ${REPO_HOME}/src/fiat/target/board/imp/board_imp.c
   ```

   plus associated configuration

   ```sh
   ${REPO_HOME}/src/fiat/target/board/imp/Dockerfile.in
   ${REPO_HOME}/src/fiat/target/board/imp/Makefile.in
   ```

noting that
although it essentially acts as a 
[Hardware Abstraction Layer (HAL)](https://en.wikipedia.org/wiki/Hardware_abstraction),
it is potentially as simple as a shim layer over a platform-specific HAL.

### API

```{eval-rst}
.. c:autofunction:: board_init
  :file: target/board/board.h
.. c:autofunction:: board_uart_wr
  :file: target/board/board.h
.. c:autofunction:: board_uart_rd
  :file: target/board/board.h
.. c:autofunction:: board_gpio_wr
  :file: target/board/board.h
.. c:autofunction:: board_gpio_rd
  :file: target/board/board.h
.. c:autofunction:: board_tsc_rd
  :file: target/board/board.h
.. c:autofunction:: board_rng_rd
  :file: target/board/board.h
```

<!--- -------------------------------------------------------------------- --->

## Shared

```{eval-rst}
.. c:autoenum::  req_t
  :file: target/share/util.h
.. c:autoenum::  ack_t
  :file: target/share/util.h
.. c:autoenum:: gpio_t
  :file: target/share/util.h
.. c:autotype::  ret_t
  :file: target/share/util.h
.. c:autotype::  tsc_t
  :file: target/share/util.h
```

<!--- ==================================================================== --->
