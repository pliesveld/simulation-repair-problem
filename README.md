## Simulation

A simulation a Repair Problem.

## Installation

```
pip3 install -r requirements
```

## Run simulation
```
python3 simulation.py
```

## Description

System needs atleast `n` working machines, and has `s` spare machines. After a random amount time, a machine will break down.  When  a machine breaks down, a spare will be used.  The broken machines enter a queue, where after a random amount of time, the machine will be repaired, and becomes a spare. When the number of broken machines is larger than the number of machines required to run the system, the system is considered crashed, and the simulation ends. The simulation records the time of the system crash, `T`.

The simulation runs up to `k` trials.  The expected value of when the simulation will crash, or `E[T]` is computed.

