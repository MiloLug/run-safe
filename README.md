# run-safe

A utility for running commands with some benefits like lockfile.

## Compilation Requirements

```sh
make clang 
```

## Build & Install

```sh
# run:
make build
sudo make install
```

## Help

```sh
Usage: run-safe [flags] -c <shell command>

Flags:
    --help        - display help
    -l <path>     - lockfile path
    -Lw           - 'lockfile wait' - wait for the other instances to stop and then run

```
