# Burgundis

An *in-memory key-value store* in C++ with a line-based, `\n` delimited TCP protocol.

## Features

- Concurrent client handling (thread-per-connection via mutex protection)
- Multi-command support over a single connection
- Persistance via a write-ahead log

## Status

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
./build/burgundis
```
By default, the server listens to port **6379**. To specify a different port, run:
```bash
./build/burgundis <your_port>
```

## Usage

```bash
nc localhost 6379
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