# FIAT: Firmware for Implementation Attack Targets

![Read the Docs (version)](https://img.shields.io/readthedocs/fiat)
![PyPI - Version](https://img.shields.io/pypi/v/libfiat)

<!--- ==================================================================== --->

## Concept

Set within the more general context of
[cyber-security](https://en.wikipedia.org/wiki/Computer_security),
a  standard       attack
will focus on the (abstract)  specification of some functionality;
it must use any associated implementation in a 
"[black box](https://en.wikipedia.org/wiki/Black_box)" 
model, limited to
explicit,               intentional input and output.
In contrast,
an implementation attack 
will focus on the (concrete) implementation of said functionality;
it can  use the associated implementation in a 
"[grey  box](https://en.wikipedia.org/wiki/Grey_box_model)" 
model, including any and all 
implicit, potentially unintentional input and output.
Narrowing the context to
[cryptography](https://en.wikipedia.org/wiki/Cryptography)
more specifically, consider an attacker tasked with recovery of some
security critical data (e.g., key material) `k` from a target device.
The permitted, black-box interaction is such that
the former              provides  input `x`,
upon which
the latter computes and provides output `r = f( k, x )`.
In 
a  standard       attack,
the attacker attempts to recover `k` using `x` and `r` alone, e.g., 
via traditional
[cryptanalysis](https://en.wikipedia.org/wiki/Cryptanalysis).
In 
an implementation attack, however,
the attacker might be permitted to
 actively influence
and/or
passively   monitor
computation by the target device: doing so captures the concepts of
fault induction
and 
[side-channel](https://en.wikipedia.org/wiki/Side-channel_attack) (or information leakage) 
attack respectively.

Although a "real" attack would typically consider some form of
[COTS](https://en.wikipedia.org/wiki/Commercial_off-the-shelf)
target device, research and development of both attacks and associated
countermeasures will, at least initially, use an alternative platform 
that is more easily 
controlled
and 
   altered.
Such a platform will include *both*
hardware components, 
e.g., a target board compatible with
[ChipWhisperer ](https://www.newae.com/chipwhisperer),
*and*
software components,
e.g., associated
[firmware](https://en.wikipedia.org/wiki/Firmware)
executed on said target board.
FIAT
is intended to provide
domain-specific support for development of the latter: the high-level
goal is for it to

- support interaction modelled as

  ```
  +---------------------------+                 +--------------------------+
  |          client           |                 |          target          |
  +===========================+                 +==========================+
  |                           | ----- req ----> | kernel layer             |
  |                           | <---- ack ----- |~~~~~~~~~~~~~~~~~~~~~~~~~~|
  |                           |                 | driver layer: SPRs, GPRs |
  |                           |                 |~~~~~~~~~~~~~~~~~~~~~~~~~~|
  |                           | <-- trigger --- |  board layer: UART, GPIO |
  +---------------------------+                 +--------------------------+
  ```

  in the sense that
  the client transmits a  req(uest)         to the target,
  the target performs some computation,  
  then
  the target transmits an ack(nowledgement) to the client,

- support 
  [target implementations](https://fiat.readthedocs.io/en/latest/target.html)
  formed from 

  1. a kernel layer, 
     i.e., the use-case specific functionality of interest,
  2. a  board layer,
     i.e., infrastructure related to the hardware, or board said functionality is executed on,
  3. a driver layer,
     which uses the board layer to provide an interface to the kernel,

- support 
  [client implementations](https://fiat.readthedocs.io/en/latest/client.html)
  based on
  [Python](https://www.python.org)
  via an associated,
  [PyPI](https://pypi.org)-hosted
  library called 
  [`libfiat`](https://pypi.org/project/libfiat),

- support a container'ised 
  build system
  for each board using
  [Docker](https://www.docker.com).

<!--- -------------------------------------------------------------------- --->

## Organisation

```
├── bin                 - scripts (e.g., environment configuration)
├── build               - working directory for build
├── doc                 - documentation
└── src
    ├── docker          - source code for containers
    └── fiat            - source code for FIAT
        ├── client      - source code for FIAT client support
        └── target      - source code for FIAT target support
            ├── board     - board  layer
            ├── driver    - driver layer
            ├── kernel    - kernel layer
            └── share     - shared functionality
```

<!--- -------------------------------------------------------------------- --->

## Usage

The easiest way to get started is arguably via the
[documentation](https://fiat.readthedocs.io):
it includes 
a high-level overview of the 
[workflow](https://fiat.readthedocs.io/en/latest/workflow.html)
involved,
plus
a low(er)-level 
[example](https://fiat.readthedocs.io/en/latest/example.html)
of applying said workflow to capture a 
[block cipher](https://en.wikipedia.org/wiki/Block_cipher)
implementation.

<!--- -------------------------------------------------------------------- --->

## Citation

If you want to reference this work
(e.g., in the acknowledgements of a paper or report), 
it'd be really helpful if you'd use the meta-data in
[`CITATION.cff`](./CITATION.cff).
This makes use of the
[Citation File Format (CFF)](https://citation-file-format.github.io),
which GitHub will allow automatic export of, e.g., as a
[BibTeX](https://www.bibtex.org)
entry, using the "cite this repository" menu item in the about box.

<!--- -------------------------------------------------------------------- --->

## Acknowledgements

Although this repository captures an independent (re-)implementation,
prototypes which informed it were previously investigated by
the
[harness](https://github.com/scarv/sca3s-harness)
repository which acts as a component of the
[SCA3S](https://github.com/scarv/sca3s) (or "side-channel analysis as a service")
project.

<!--- ==================================================================== --->
