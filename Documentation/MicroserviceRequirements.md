# Microservice Generic Requirements

### 1. Code Offset
> Code Region Offset must be "0".

The developer is free to set the Code Size. But there can be restrictions, like alignments, on the Code size depending the architecture.

### 2. RAM Offset
> RAM Region Offset must be "0x8000000".

The developer is free to set the RAM Size. But there can be restrictions, like alignments, on the RAM size depending the architecture.

### 3. Position Indepedent
The Microservice image must be Position Independent (PIE) and shall have a Global Offset Table (GoT)

### 4. Required Symbols

 Microcontainer must provide some symbol to the Microservice Runtime Please see the required symbols below

| Word Index | Field Name               | Description                                 |
|------------|--------------------------|---------------------------------------------|
| **WORD 0** | Main Stack Pointer       | Initial value of the main stack pointer.|
| **WORD 1** | Main Stack Limit         | Defined limit for the main stack.|
| **WORD 2** | Entry Point              | Microservice execution entry point address.|
| **WORD 3** | GOT RO Offset            | Global Offset Table Read-Only section offset.|
| **WORD 4** | GOT RW Offset            | Global Offset Table Read-Write section offset.|
| **WORD 5** | GOT Size                 | Total size of the Global Offset Table.|
