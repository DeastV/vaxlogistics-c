# Vaccine Logistics & Batch Management System in C

[![Language](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20POSIX-orange.svg)](https://en.wikipedia.org/wiki/POSIX)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A command-line inventory logistics and inoculation tracking engine written in C11. The system handles pharmaceutical vaccine batch registration, expiration date monitoring, automated batch assignment for patient inoculations, multi-criteria record queries, and safe dynamic heap memory management.

---

## Technical Features

* **Dynamic Memory & Resource Management:**
  * Efficient heap utilization with `malloc`, `realloc`, and `strdup` dynamically resizing batch arrays and patient inoculation registries.
  * Comprehensive memory cleanup routines ensuring zero leaks across standard lifecycles, verified via Valgrind.
* **Batch Validation & Ordering:**
  * Strict batch naming validation enforcing uppercase hexadecimal identifiers.
  * Chronological and alphabetical sorting for expiration tracking and automated priority dispatch.
* **Stream-Based Command Parser:**
  * Buffered standard input processing using `fgets`, `sscanf`, and `strtok` supporting quoted multi-word strings (e.g., patient names).
* **Internationalization (i18n):**
  * Multi-language diagnostic system supporting English (default) and Portuguese error responses via CLI arguments.

---

## Command Reference

The engine reads interactive or piped commands from standard input:

| Command | Arguments | Description |
| :--- | :--- | :--- |
| `c` | `<batch_name> <dd-mm-yyyy> <doses> <vaccine_name>` | Registers a new vaccine batch into inventory. Batch name must be an uppercase hexadecimal string. |
| `t` | `[dd-mm-yyyy]` | Advances the system date (must be equal to or after current date). Displays updated date. |
| `l` | `[vaccine_name ...]` | Lists registered batches sorted by expiration date and name. If vaccine names are provided, filters by those vaccines. |
| `a` | `<patient_name> <vaccine_name>` | Administers a vaccine dose to a patient, automatically selecting the earliest valid batch in stock. Supports quoted names (`"Maria Silva"`). |
| `r` | `<batch_name>` | Removes or depletes a batch. Prints number of applied doses. Deletes batch if unused; zeroes available stock if doses were applied. |
| `u` | `[patient_name]` | Lists patient inoculation histories (`<patient> <batch> <date>`). Lists all records or filters by specified patient. |
| `d` | `<patient_name> [dd-mm-yyyy] [batch_name]` | Removes inoculation records matching the provided criteria (patient, patient+date, or patient+date+batch). Prints count of removed records. |
| `q` | *(none)* | Frees all dynamically allocated memory and terminates the program. |

---

## Example Session

### Input

```text
c 01A 15-06-2025 100 Pfizer
c 02B 20-08-2025 50 Moderna
a "Maria Silva" Pfizer
a Joao Moderna
l
u
q
```

### Output

```text
01A
02B
01A
02B
Pfizer 01A 15-06-2025 99 1
Moderna 02B 20-08-2025 49 1
Maria Silva 01A 01-01-2025
Joao 02B 01-01-2025
```

---

## Project Structure

```text
vaxlogistics-c/
├── Makefile                # Build automation (-Wall -Wextra -Werror -std=c11)
├── LICENSE                 # MIT License
├── .gitignore              # Build and profiling exclusions
├── README.md               # Project documentation
├── include/
│   └── projeto.h           # Data structures, definitions, and prototypes
└── src/
    ├── main.c              # Application entrypoint and command dispatch loop
    ├── functions.c         # Business logic and entity operations
    └── auxiliares.c        # Memory allocation, validation, and sorting utilities
```

---

## Compilation & Execution

### Prerequisites
* **GCC Compiler** (C11 standard, POSIX support)
* **GNU Make**

### Build
```bash
# Compile with strict compiler warnings (-Wall -Wextra -Werror)
make

# Clean binary
make clean
```

### Run
```bash
# Run in default mode (English diagnostics)
./vaxlogistics

# Run with Portuguese diagnostics
./vaxlogistics pt
```

### Memory Profiling with Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all ./vaxlogistics
```

---

## Known Limitations

* **Fixed Vaccine Filter Limit in Listing (`l`):** The `lista_lote` implementation allocates a static pointer buffer for filtered vaccine names (`char *vacinas[51]`). Supplying 52 or more distinct vaccine names to `l` exceeds this bound.
* **Quadratic Sort Complexity:** In-place inventory reordering is executed via bubble sort ($O(n^2)$) on demand across `l` and `a` commands, prioritized for code clarity rather than large-scale data volume throughput.

---

## Credits

* **David Vasques** ([@DeastV](https://github.com/DeastV))
* Individual coursework developed for Introdução aos Algoritmos e Estruturas de Dados (IAED) at Instituto Superior Técnico, Universidade de Lisboa.
* *Note: Original commit history is not available; this is the final submitted version.*
