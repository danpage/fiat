# Build system

<!--- ==================================================================== --->

## Overview

`${REPO_HOME}/Makefile` can be used to drive the build process, e.g., via

| Command                  | Description                                                    |
|:-------------------------|:---------------------------------------------------------------|
| `make   venv/clean`      | clean                  the Python virtual environment          |
| `make   venv/build`      | build                  the Python virtual environment          |
| `make docker/clean`      | clean [Docker](https://www.docker.com)-based container         |
| `make docker/build`      | build [Docker](https://www.docker.com)-based container         |
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

<!--- -------------------------------------------------------------------- --->

## The `CONTEXT` environment variable
    
The
`CONTEXT`
environment variable specifies the
build context
within which the build process is executed.
The options are

1. `native` is a        native build context,
2. `docker` is a containerised build context,

noting that:

- using the
  `native`
  build context implies
  the tool-chain,
  and
  any software dependencies (e.g., libraries)
  need to be installed manually,

- using the
  `docker`
  build context implies
  the tool-chain,
  and
  any software dependencies (e.g., libraries)
  are installed within the associated container.

<!--- -------------------------------------------------------------------- --->

## The `DRIVER`  environment variable

The
`DRIVER`
environment variable specifies the
[driver type](target.md#target-driver).
The options are

1. `binary` uses a binary protocol,
2. `text`   uses a   text protocol.

<!--- -------------------------------------------------------------------- --->

## The `BOARD`   environment variable

The
`BOARD`
environment variable specifies the
[board  type](target.md#target-board).
The options are

| `BOARD`                | Class      | Vendor                                          | Core                                                          | Model                                                                          | ISA      | Host                                                                                  |
|:-----------------------|:-----------|:------------------------------------------------|:--------------------------------------------------------------|:-------------------------------------------------------------------------------|:---------|:--------------------------------------------------------------------------------------|
| `native`               | Simulation |                                                 |                                                               |                                                                                |          |                                                                                       |
| `cw308/atmega328p`     | ASIC       |          [Microchip](https://www.microchip.com) | [ATmega](https://en.wikipedia.org/wiki/AVR_microcontrollers)  | [ATMEGA328P](https://www.microchip.com/en-us/product/atmega328p)               | AVR      | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-AVR)        |
| `cw308/atxmega128d4`   | ASIC       |          [Microchip](https://www.microchip.com) | [ATxmega](https://en.wikipedia.org/wiki/AVR_microcontrollers) | [ATXMEGA128D4](https://www.microchip.com/en-us/product/atxmega128d4)           | AVR      | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-XMEGA)      |
| `cw308/stm32f071rbt6`  | ASIC       | [STMicroelectronics](https://www.st.com)        | [Cortex-M0](https://en.wikipedia.org/wiki/ARM_Cortex-M)       | [STM32F071RBT6](https://www.st.com/resource/en/datasheet/stm32f071cb.pdf)      | ARMv6-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F)     |
| `cw308/stm32f100rbt6b` | ASIC       | [STMicroelectronics](https://www.st.com)        | [Cortex-M3](https://en.wikipedia.org/wiki/ARM_Cortex-M)       | [STM32F100RBT6B](https://www.st.com/resource/en/datasheet/stm32f100cb.pdf)     | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F)     |
| `cw308/stm32f215ret6`  | ASIC       | [STMicroelectronics](https://www.st.com)        | [Cortex-M3](https://en.wikipedia.org/wiki/ARM_Cortex-M)       | [STM32F215RET6](https://www.st.com/resource/en/datasheet/stm32f215re.pdf)      | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F)     |
| `cw308/stm32f303rct7`  | ASIC       | [STMicroelectronics](https://www.st.com)        | [Cortex-M4](https://en.wikipedia.org/wiki/ARM_Cortex-M)       | [STM32F303RCT7](https://www.st.com/resource/en/datasheet/stm32f303rc.pdf)      | ARMv7-M  | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F)     |
| `cw308/stm32f405rgt6`  | ASIC       | [STMicroelectronics](https://www.st.com)        | [Cortex-M4](https://en.wikipedia.org/wiki/ARM_Cortex-M)       | [STM32F405RGT5](https://www.st.com/resource/en/datasheet/stm32f405rg.pdf)      | ARMv7E-M | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-STM32F)     |
| `cw308/fe310-g002`     | ASIC       |             [SiFive](https://www.sifive.com)    |  E31                                                          | [FE310-G002](https://www.sifive.com/document-file/freedom-e310-g002-datasheet) | RV32IMAC | [ChipWhisperer CW308](https://rtfm.newae.com/Targets/UFO%20Targets/CW308T-FE310-G002) |
| `cw305/ibex`           | FPGA       |            [lowRISC](https://www.lowrisc.org)   | [Ibex](https://github.com/lowRISC/ibex)                       |                                                                                | RV32IMC  | [ChipWhisperer CW305](https://rtfm.newae.com/Targets/CW305%20Artix%20FPGA)            |

<!--- -------------------------------------------------------------------- --->

## Container management

- The
  [Docker](https://www.docker.com)
  image for a given board type is built using

  - board-agnostic content in 
    `${REPO_HOME}/src/docker`,
    plus
  - board-specific content in 
    `${REPO_HOME}/src/target/board/imp/${BOARD}/Dockerfile.in`.

  However,
  *there is no need to do this manually*:
  a pre-built image can (and will) be pulled from
  [Docker Hub](https://hub.docker.com/u/danpage)
  by the build process as needed.


- If you *do* want to build such an image, note that

  ```sh
  CONTEXT="native" make docker/clean
  CONTEXT="native" make docker/build
  ```

  will clean (or remove) and build the image  for *one* board type
  (specified using `${BOARD}`)

  ```sh
  ${REPO_HOME}/bin/docker.sh clean
  ${REPO_HOME}/bin/docker.sh build
  ```

  will clean (or remove) and build the images for *all* board types
  (identified by searching for instances of `Dockerfile.in` in `${REPO_HOME}/src/fiat/target/board/imp`).

<!--- ==================================================================== --->
