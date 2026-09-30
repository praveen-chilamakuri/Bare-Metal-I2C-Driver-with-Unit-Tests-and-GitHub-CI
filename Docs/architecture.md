# Architecture

This project implements a bare‑metal, interrupt‑driven I2C driver for the SHT31 temperature and humidity sensor on the STM32F411RE Nucleo board.  
The firmware is designed for low‑power operation, entering STOP mode between measurements and waking only on an EXTI13 falling‑edge interrupt.

The architecture is intentionally simple, deterministic, and hardware‑centric — ideal for demonstrating embedded fundamentals.

---

## System Components

### 1. I2C Transaction Layer (`i2c_driver.c`)
Responsible for low‑level bus control:
- START / STOP generation  
- Address phase  
- Byte write/read  
- ACK/NACK control  
- Timing based on 100 kHz standard mode  

This layer is not unit‑testable (pure registers) and is validated using integration testing (logic analyser).

---

### 2. Sensor Command Layer (`sht31.c`)
Implements SHT31‑specific behaviour:
- Measurement command (0x24, 0x0B)  
- Reading 6 raw bytes  
- CRC8 validation  
- Data grouping (Temp + CRC, Hum + CRC)

This layer is fully unit‑tested using Ceedling (mocked registers).

---

### 3. Platform Layer (`gpio_init.c`, `stop_mode.c`, `clock.c`)
Handles MCU‑specific configuration:
- GPIO setup for I2C pins and LED  
- EXTI13 configuration (PC13 → wake source)  
- STOP‑mode entry  
- Clock restoration after wake  
- SysTick reinitialisation  

This layer is not unit‑testable (pure registers) and is validated using integration testing (logic analyser).

---

## Execution Flow

### 1. Boot
- HSI enabled  
- SysTick configured  
- GPIO + I2C initialised  

### 2. While Loop
The MCU enters STOP mode.

### 3. STOP Mode
- Regulator ON  
- CPU halted  
- Clocks disabled  
- Wake source: EXTI13 falling edge  

### 4. Wakeup
Triggered by PC13 falling edge:
- HSI restored  
- SysTick reconfigured  
- I2C reinitialised  

### 5. Measurement
- SHT31 measurement command  
- 15 ms wait  
- 6‑byte read  
- CRC validation  
- LED pulse on PA5 if CRC OK  

---

