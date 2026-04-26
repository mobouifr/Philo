*This project has been created as part of the 42 curriculum by mobouifr.*

<div align="center">

# Philosophers

### I never thought philosophy would be so deadly: a thread-level starvation simulation with strict timing.

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![POSIX Threads](https://img.shields.io/badge/Concurrency-POSIX%20Threads-222222?style=for-the-badge)
![42 Norm](https://img.shields.io/badge/42-Norminette-0A0A0A?style=for-the-badge)

</div>

## What Is This?

`philo` is a concurrency simulation where each philosopher runs in its own thread, competes for two fork mutexes, and alternates between eating, sleeping, and thinking until either someone dies or everyone has eaten enough times.

The interesting part is not printing state changes. The interesting part is coordinating many threads under timing constraints without races: shared stop flags, protected last-meal timestamps, synchronized logs, and lock ordering to avoid circular waits.

This project teaches practical multithreading fundamentals in C: thread lifecycle (`pthread_create`/`pthread_join`), mutex strategy, cooperative stop conditions, and time-sensitive monitoring loops.

> Concurrency is easy to start and hard to stop correctly.

## How It Works

Each philosopher thread runs a routine loop and a monitor loop runs in the main thread.

```text
				+----------------------------+
				|        monitor_routine     |
				|  - checks death condition  |
				|  - checks meal completion  |
				+-------------+--------------+
							  |
							  v (kill_philo / stop)
 +-------------------+   +-------------------+   +-------------------+
 | philosopher #1    |   | philosopher #2    |   | philosopher #N    |
 | think/sleep/eat   |   | think/sleep/eat   |   | think/sleep/eat   |
 +---------+---------+   +---------+---------+   +---------+---------+
		   |                       |                       |
		   +----------- lock/unlock fork mutexes ----------+
```

Key synchronization points in this codebase:

1. `forks[]` is an array of `pthread_mutex_t` (one mutex per fork).
2. `print_lock` serializes log lines.
3. `last_meal_mutex`, `meal_eaten_mutex`, and `died_philo` protect shared state.
4. `switch_last_philo_forks` enforces a lock order based on mutex addresses before taking forks.

<details>
<summary>Verified Build Flags</summary>

```bash
-Wall -Wextra -Werror -g3 -pthread
```

</details>

## Features

| Feature | Status | Notes |
|---|---|---|
| Mandatory `philo` binary | ✓ | Built from `Makefile` target `philo`. |
| One thread per philosopher | ✓ | Created in `creat_philosophers` with `pthread_create`. |
| One mutex per fork | ✓ | Allocated as `args->forks` and initialized per philosopher count. |
| Synchronized logging | ✓ | `print_state` uses mutexes to avoid overlapped messages. |
| Death monitor | ✓ | `monitor_routine` checks starvation with `is_dead` and prints `died`. |
| Optional meal limit argument | ✓ | If present, simulation ends after all philosophers reach required meals. |
| Single philosopher handling | ✓ | Dedicated path in `only_one_philo`. |
| Input validation | ✓ | Numeric checks and `INT_MAX`-bounded conversion via `ft_atoi`. |
| Bonus program (`philo_bonus`) | — | No bonus directory or bonus target present. |

## Project Structure

```text
.
├── Makefile               ← Build rules and compiler flags for `philo`
├── philo.h                ← Shared structs, includes, and function prototypes
├── philo.c                ← Entry point, parsing, allocation, thread launch, cleanup
├── routine.c              ← Philosopher routine and action functions
├── monitor.c              ← Starvation and completion monitoring logic
├── philo_utils.c          ← Logging, timing helpers, and args initialization
├── philo_utils2.c         ← Lock-order helper and shared mutex initialization
└── utils/
	├── ft_atoi.c          ← Strict positive integer parser with overflow guard
	└── ft_isdigit.c       ← Digit predicate used by parser
```

## Getting Started

### Build

```bash
make
```

### Run

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Example Runs

```bash
./philo 1 200 100 100
./philo 2 200 60 60 2
```

### Makefile Rules

| Rule | Effect |
|---|---|
| `all` | Builds `philo` |
| `philo` | Links object files into the final binary |
| `clean` | Removes object files |
| `fclean` | Removes object files and binary |
| `re` | Runs `fclean` then `all` |

<details>
<summary>Argument Constraints Enforced by the Current Code</summary>

```text
argc must be 5 or 6
number_of_philosophers >= 1
time_to_die >= 1
time_to_eat >= 1
time_to_sleep >= 1
if optional argument exists: number_of_times_each_philosopher_must_eat >= 1
all numeric arguments must pass ft_atoi validation
```

</details>

## Resources

| Resource | Why it helps |
|---|---|
| https://man7.org/linux/man-pages/man3/pthread_create.3.html | Thread lifecycle and API contract |
| https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html | Mutex semantics and locking behavior |
| https://man7.org/linux/man-pages/man3/gettimeofday.3p.html | Timestamp source used for simulation timing |
| https://en.wikipedia.org/wiki/Dining_philosophers_problem | Classic concurrency model behind the exercise |

