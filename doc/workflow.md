# Workflow

<!--- ==================================================================== --->

## Step 1: install pre-requisites

- *Either*

  1. provide a native build context by installing 
     associated pre-requisites, e.g.,
     a suitable
     compiler 
     and 
     programming 
     tool-chain,
     e.g., board-specific versions of
     [GCC](https://gcc.gnu.org)
     and
     [OpenOCD](http://openocd.org),
     *or*

  2. provide a Docker build context by installing 
     the 
     [Docker](https://www.docker.com)
     container platform.

<!--- -------------------------------------------------------------------- --->

## Step 2: clone repository

- *Either* use

  1. the 
     [GitHub CLI](https://github.com/cli/cli), 
     e.g.,

     ```sh
     gh repo clone https://github.com/danpage/fiat.git ./fiat
     ```

     *or*
  
  2. [git](https://git-scm.com>)
     itself, 
     e.g.,
  
     ```sh
     git clone https://github.com/danpage/fiat.git ./fiat
     ```

- Execute

  ```sh
  cd ./fiat
  source ./bin/conf.sh
  ```
  
  to configure the environment.
  For example, you should find that the
  `REPO_HOME`
  and
  `REPO_VERSION`
  environment variables are set appropriately.

<!--- -------------------------------------------------------------------- --->

## Step 3: develop target implementation

- Edit

  ```sh
  ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.conf
  ```

  to configure the target implementation (and support for it),

- Edit

  ```sh
  ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.h
  ${REPO_HOME}/src/fiat/target/kernel/imp/kernel_imp.c
  ```

  to complete the target implementation itself.

<!--- -------------------------------------------------------------------- --->

## Step 4: build   target implementation

- *Either*
  decide on the 
  build context
  then
  set the associated environment variable, e.g., by executing

  ```sh
  export CONTEXT="native"
  ```

  *or* accept default of `native`.

- *Either*
  decide on the 
  driver type
  then
  set the associated environment variable, e.g., by executing

  ```sh
  export DRIVER="binary"
  ```

  *or* accept default of `binary`.

- *Either*
  decide on the 
  board  type
  then
  set the associated environment variable, e.g., by executing

  ```sh
  export BOARD="native"
  ```

  *or* accept default of `native`.

- *Either*

  1. for a 
     `native`
     build context,
     execute

     ```sh
     make target/clean
     make target/fetch-deps
     make target/patch-deps
     make target/build-deps
     make target/build
     ```

     *or*

  2. for a 
     `docker`
     build context,
     execute

     ```sh
     make target/clean
     make target/build
     ```

<!--- -------------------------------------------------------------------- --->

## Step 5: use     target implementation

- *If* the build process succeeds, it produces associated artifacts in
  `${REPO_HOME}/build/target/${BOARD}`, e.g.,
  
  ```sh
  ${REPO_HOME}/build/target/${BOARD}/target.elf
  ${REPO_HOME}/build/target/${BOARD}/target.bin
  ${REPO_HOME}/build/target/${BOARD}/target.hex
  ```

  Using the appropriate programming infrastructure, you can program 
  your board with one of them.

<!--- ==================================================================== --->
