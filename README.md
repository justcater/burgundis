# Burgundis

An *in-memory key-value store* in C++ with a line-based, `\n` delimited TCP protocol.

## Features

- Concurrent client handling (thread-per-connection via mutex protection)
- Multi-command support over a single connection

## Status

- In-memory only, data doesn't survive a restart - write-ahead log to be added
- No TTL/Expiration
- Text protocol only

## Requirements

- Linux/macOS (POSIX sockets)
- `make` and `g++`

## Build & Run

```bash
git clone https://github.com/justcater/burgundis.git
cd burgundis
make
make run # or ./build/burgundis
```

## Usage

```bash
nc localhost 6379 # default port
SET name burgundy
OK
GET name
burgundy
DEL name
OK
GET name
(nil)
```

## Commands

| Command | Syntax | Output |
| ------- | ------ | ------ |
| SET | `SET <key> <value>` | `OK`|
| GET | `GET <key>` | value or `(nil)` |
| DEL | `DEL <key>` | `OK` |

## License
This project is licensed under the MIT License - see the LICENSE file for details.