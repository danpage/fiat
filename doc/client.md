# Client support

<!--- ==================================================================== --->

## Library

```{eval-rst}
.. autofunction:: libfiat.open

.. autoclass:: libfiat.util.Req
.. autoclass:: libfiat.util.Ack
.. autoclass:: libfiat.util.Err
.. autoclass:: libfiat.util.Type
.. autoclass:: libfiat.util.Index

.. autoclass:: libfiat.driver.DriverAbs
.. autoclass:: libfiat.driver.DriverImpBinary
.. autoclass:: libfiat.driver.DriverImpText

.. autoclass:: libfiat.client.ClientAbs
.. autoclass:: libfiat.client.ClientImpSocket
.. autoclass:: libfiat.client.ClientImpSerial
```

<!--- -------------------------------------------------------------------- --->

## Scripts

Alongside the `libfiat` library itself, you can find a range of client
implementations which make use of it: 
some are purely examples,
whereas
others offers support, e.g., for common tasks.

- The script

  ```sh
  ${FIAT_PATH_REPO}/src/fiat/client/script/example.py
  ```

  reflects the worked
  [example](https://fiat.readthedocs.io/en/latest/example.html).

- The script

  ```sh
  ${FIAT_PATH_REPO}/src/fiat/client/script/inspect.py
  ```

  is intended as a tool to inspect a target implementation: for example,
  given an otherwise 
  ["black box"](https://en.wikipedia.org/wiki/Black_box)
  target implementation, it can be used to

  - discover and report the configuration (e.g., in terms of the GPRs),
  - generate test vectors,
  - validate test vectors.

<!--- ==================================================================== --->
