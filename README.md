# HW06

## System Architecture

- **BLE Server (ESP32 #1):**
  - Broadcasts BLE advertising packets.
  - Sets txPower to a known value (e.g., 0 dBm).

- **BLE Client (ESP32 #2):**
  - Scans for BLE advertisements.
  - Reads RSSI values from the server.
  - Applies path-loss model to estimate distance.
  - Displays real-time distance in Serial Monitor and on a Web Server.
  - Controls LED for proximity alert.

---

## Data for building model

| Actual Distance (m) | RSSI (dBm) |
|---------------------|------------|
| 0.5                 | -49        |
| 1.0                 | -58        |
| 1.5                 | -62        |
| 2.0                 | -66        |
| 3.0                 | -70        |
| 4.0                 | -73        |

We assume txPower = 0 dBm.  
Now we apply the model:  
**RSSI = txPower - 10 * n * log10(distance)**  
→ Rearranged:  
**n = (txPower - RSSI) / (10 * log10(distance))**

Calculate n for each point:

| Distance (m) | RSSI | n ≈ (0 - RSSI) / (10 * log10(d)) |
|--------------|------|----------------------------------|
| 0.5          | -49  | 49 / (10 * log10(0.5)) ≈ 1.47     |
| 1.0          | -58  | 58 / (10 * log10(1.0)) ≈ 5.80     |
| 1.5          | -62  | 62 / (10 * log10(1.5)) ≈ 4.99     |
| 2.0          | -66  | 66 / (10 * log10(2.0)) ≈ 4.95     |
| 3.0          | -70  | 70 / (10 * log10(3.0)) ≈ 4.68     |
| 4.0          | -73  | 73 / (10 * log10(4.0)) ≈ 4.57     |

Average `n` ≈ 4.58 (rounded)


---

## Distance Estimation Model

We use a path-loss model based on empirical RSSI data:
    distance (m) = 10 ^ ((txPower - RSSI) / (10 * 4.58))
Where:
- `txPower` = transmission power at 1m (set to 0 dBm)
- `RSSI` = Received Signal Strength Indicator
- `4.58` = path-loss exponent, estimated from sample data

This model will be used in the ESP32 client for real-time distance estimation.
