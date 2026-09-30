# Timing Analysis

This document summarises the timing behaviour of the I2C measurement sequence and STOP‑mode wakeup path, verified using a logic analyser.

---

## I2C Bus Timing

### Configuration
- Bus speed: **100 kHz**  
- TRISE: **17**  
- CCR: **80**  

### Measured Values

| Event | Typical Duration |
|-------|------------------|
| START → Address | ~98 µs |
| Write command (2 bytes) | ~189 µs |
| Measurement delay | **~15 ms** (14.67 ms) |
| Read 6 bytes and STOP condition | ~645 µs |

Total transaction time (excluding measurement delay): **~932 µs**

Total including measurement delay: **~15.6 ms**

---

## EXTI Wakeup Timing

### Wake Source
- PC13 falling edge  
- EXTI13 interrupt  

### Measured Values

| Event | Duration |
|-------|----------|
| EXTI13 falling edge → ISR entry | ~13 µs |
| ISR exit → first instruction in main loop | ~2 µs |
| Clock restore (HSI ready) | ~5 µs |

Total wakeup time: **~20 µs**

---

## STOP Mode Behaviour

STOP mode configuration:
- `SLEEPDEEP = 1`  
- `PDDS = 0` → STOP mode  
- `LPDS = 0` → regulator ON  

This results in:
- CPU halted  
- Clocks disabled  
- EXTI active  
- Wakeup on falling edge  

---

## Verification Method

A logic analyser was connected to:
- **SCL & SDA** lines  
- **PA5 & PA6**   
- **PC13** (wake source)

Captured waveforms confirm:
- Correct I2C timing  
- Deterministic wakeup behaviour  
- No clock stretching  
- Clean ACK/NACK transitions  

Screenshots are in `/Docs/images/`.
