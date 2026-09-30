# Power Behaviour Notes

This project is designed around low‑power operation using STOP mode.

---

## STOP Mode Characteristics

STOP mode is configured with:
- **Main regulator ON** (`LPDS = 0`)  
- **Deep sleep enabled** (`SLEEPDEEP = 1`)  
- **PDDS = 0** → STOP mode (not Standby)  

This mode reduces power consumption by:
- Halting the CPU  
- Disabling system clocks  
- Keeping SRAM and registers intact  
- Allowing EXTI wakeup  

---

## Wakeup Strategy

Wakeup occurs via:
- **EXTI13 falling edge** (PC13 button)

After wake:
- HSI clock is restored  
- SysTick reinitialised  
- I2C peripheral re-enabled  

This ensures deterministic behaviour after each wake cycle.

---

## Design Intent

The firmware is structured to:
- Minimise active time  
- Perform only essential work after wake  
- Return to STOP mode immediately  
- Demonstrate low‑power firmware design fundamentals  
