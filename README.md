# RISCV W7 CPU #

RISCV W7 CPU supports riscv64-imfdcb instruction set architecture and is implemented with 5-stage pipeline. It contains dynamic branch prediction (gshare), instruction and data cache together with fetch and store buffer.

## Dhrystone Benchmark ##
| Cycles | Dhrystone/s/MHz | DMIPS/s/MHz | Iteration |
| ------ | --------------- | ----------- | --------- |
|    362 |            2761 |        1.57 |       100 |

## Coremark Benchmark ##
| Cycles | Iteration/s/MHz | Iteration |
| ------ | --------------- | --------- |
| 379254 |            2.64 |        10 |

Documentation will be expanded in the future.
