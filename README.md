<h1 align="center">Bare‑Metal I2C Driver with Unit Tests and GitHub CI</h1>

**License:** `MIT` | **MCU:** `STM32F411RE` | **Field:** `Embedded Systems` | **Sensors:** `SHT31` | **Focus:** `Baremetal`

<p align="center">
  <a href="https://github.com/praveen-chilamakuri/baremetal-i2c-driver/actions">
    <img src="https://github.com/praveen-chilamakuri/baremetal-i2c-driver/actions/workflows/ceedling.yml/badge.svg?branch=main" alt="Ceedling Tests">
  </a>
</p>

A bare‑metal, interrupt‑driven I2C driver for the SHT31 temperature and humidity sensor, running on the STM32F411RE Nucleo board.  
The project demonstrates low‑level firmware design, deterministic timing, STOP‑mode power behaviour, and full unit‑test coverage for all logic and hardware‑mockable modules using Ceedling.  
Continuous Integration is provided through GitHub Actions, running all tests automatically on every push.

Instead of computing temperature and humidity, the firmware reads the raw SHT31 bytes, validates them using CRC, and the values were manually confirmed using a logic analyser.  
This keeps the project intentionally focused on pure bare‑metal driver design, deterministic timing, and low‑level I2C signalling.

---

## 🚀 Project Overview

This firmware implements:

- A **bare‑metal I2C driver** (START, STOP, ACK/NACK, byte‑level control)
- **EXTI‑driven wakeup** from STOP mode (PC13 falling edge)
- **SHT31 measurement sequence** (command + 6‑byte read + CRC validation)
- **STOP‑mode low‑power operation** with manual clock restoration
- **Ceedling unit tests** for all logic and mockable hardware layers
- **GitHub CI** for automated test execution

The design is intentionally simple, deterministic, and hardware‑centric — ideal for demonstrating embedded fundamentals to engineering teams and recruiters.

---

## 📂 Repository Structure

```text
baremetal-i2c-driver/
├── Firmware/                # Application source & include files, project files
├── Ceedling Tests/          # Unit testing files
├── Docs/                    # Logic analyser screenshots, architecture, timing-analysis, power-notes
├── .github/                 # CI workflows
├── LICENSE                  # MIT License
└── README.md                # Project configuration description
```

---

## 🧪 Test Coverage

This project achieves **100% unit test coverage** for all logic and hardware‑mockable modules using **Ceedling (Unity + CMock)**.

### ✔ Covered by unit tests  
- SHT31 command and parsing logic  
- CRC8 validation  
- Measurement sequencing logic  

### ❌ Not covered by unit tests (non‑mockable)
- Register‑level hardware configuration  
- STOP‑mode entry/exit  
- EXTI wakeup behaviour  
- Clock restoration  

These non‑mockable blocks are validated through **integration testing** using a logic analyser.

---

## 🔧 Hardware

- **MCU:** STM32F411RE Nucleo  
- **Sensor:** SHT31 (I2C)  
- **Wake Source:** EXTI13 (PC13 falling edge)  
- **Tools:** Logic analyser for timing verification  

---

## 📡 STOP Mode Behaviour

The system enters **STOP mode (regulator ON)** between measurements:

- `SLEEPDEEP = 1`  
- `PDDS = 0` → STOP mode  
- `LPDS = 0` → main regulator ON  

Wakeup occurs via **EXTI13**, after which:

- HSI clock is restored  
- SysTick is reinitialised  
- I2C1 is reconfigured  

This ensures deterministic behaviour after each wake cycle.

---

## ⏱ Timing Verification

Timing determinism is validated using a logic analyser:

- I2C transaction duration  
- SCL/SDA waveform integrity  
- EXTI wakeup latency  
- STOP‑mode exit timing  

Measured values are documented in the timing notes.

---

## 🏗 Build & Test Instructions

### Run Unit Tests

```bash
ceedling test:all
```

---

## CI Pipeline

The GitHub Actions workflow (`.github/workflows/ceedling.yml`) automatically:

- Builds the test suite  
- Runs all unit tests  
- Reports pass/fail status  

---

## 🎯 Why This Project Matters

This project demonstrates: 

- Ability to design **bare‑metal drivers** on STM32  
- Strong understanding of **interrupt‑driven firmware**  
- Experience with **STOP‑mode low‑power design**  
- Use of **test‑driven development** in embedded C  
- Integration of **CI pipelines** for firmware projects  
- Evidence of **timing determinism** and hardware‑level validation  
- Clean documentation and professional engineering workflow  

It reflects the engineering practices expected in embedded/firmware roles across the UK.

---

## 📜 License

MIT License — free to use, modify, and build upon.


