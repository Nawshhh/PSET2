# PSET2 : LFG Dungeon Queue Simulator (C++20, threaded)

A small simulator for an MMORPG **Looking For Group (LFG)** system. It forms parties of **1 Tank, 1 Healer, 3 DPS** and runs up to **n** dungeon instances concurrently. Each run’s duration is a random integer in **[t1, t2]** seconds. The program prints the current status of each instance (`active`/`empty`) and ends with a per-instance + aggregate summary.

---

## Features
- **Concurrency limit**: at most `n` instances run at the same time
- **Randomized duration**: `t ∈ [t1, t2]` seconds
- **No deadlock/starvation**: guarded by mutexes, simple dispatcher
- **Live status**: prints instance status periodically
- **Summary**: parties served & total time per instance and in aggregate

---

## Config Text File

Create a config file and name it config.txt

- **n**  : maximum number of concurrent instances
- **t**  : number of tank players in the queue
- **h**  : number of healer players in the queue
- **d**  : number of DPS players in the queue
- **t1** : minimum instance clear time (seconds)
- **t2** : maximum instance clear time (seconds)

```bash
n=
t=
h=
d=
t1=
t2=
```

---

## Build

```bash
# 1
cd PSET2

#2
g++ *.cpp -o main -std=c++20

#3
./main
