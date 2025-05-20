# HW07

## System Architecture

- **BLE Server (ESP32 #1):**
  - Broadcasts BLE advertising packets.
  - Sets txPower to a known value (e.g., 0 dBm).

- **BLE Client (ESP32 #2):**
  - Scans for BLE advertisements.
  - Reads RSSI values from the server.
  - Applies path-loss model to estimate distance.
  - Displays real-time distance in Serial Monitor.
  - Controls LED for proximity alert.

## Data for Building Model

| Actual Distance (m) | RSSI (dBm) |
|---------------------|------------|
| 0.5                 | -49        |
| 1.0                 | -58        |
| 1.5                 | -62        |
| 2.0                 | -66        |
| 3.0                 | -70        |
| 4.0                 | -73        |

We calibrated the model with:

- `txPower = -58 dBm` (measured average RSSI at 1 meter)
- `n = 2.2` (realistic indoor path-loss exponent)

## Distance Estimation Model

Path-loss model:

```
distance (m) = 10 ^ ((txPower - RSSI) / (10 * 2.2))
```

Where:
 - txPower = -58 dBm (measured at 1m)
 - RSSI = Received Signal Strength Indicator
 - 2.2 = path-loss exponent (adjusted for indoor environment)

## Test and Evaluation

We tested the distance estimation model at several known distances using measured RSSI values and compared them with the estimated distances based on the improved model.


| Actual Distance (m) | Measured Distance (m) | Error (m) |
|---------------------|-----------------------|-----------|
| 0.45                | 1.87                  | 1.42      |
| 0.90                | 1.20                  | 0.30      |
| 1.35                | 1.50                  | 0.15      |
| 1.80                | 1.57                  | 0.23      |
| 2.25                | 2.00                  | 0.25      |
| 2.70                | 3.50                  | 0.80      |

![graph](https://github.com/tmin002/IoT25-HW07/blob/main/graph.png)

### Test video

Serial Monitor<br>
[![Watch the video](https://img.youtube.com/vi/PsfHGMFpzy0/0.jpg)](https://youtu.be/PsfHGMFpzy0)
<br>

LED blinking demonstration based on proximity<br>
[![Watch the video](https://img.youtube.com/vi/Xlhx_D0ntHQ/0.jpg)](https://youtu.be/Xlhx_D0ntHQ)
<br>

### Analysis

Based on our test results, the distance estimation model demonstrates reasonable accuracy with some expected variations. The model performs particularly well in the middle range (1.35m to 2.25m) with errors less than 0.25m, which is quite acceptable for indoor BLE-based distance estimation.

Key observations:
1. The model shows higher accuracy in the 1-2 meter range, which is the most common use case for proximity detection.
2. There are some larger errors at the extremes (0.45m and 2.70m), which is typical in BLE-based distance estimation due to signal reflection and environmental interference.
3. The average error across all measurements is approximately 0.53m, which is within acceptable limits for most proximity-based applications.

The results validate our approach of using a path-loss model with a calibrated txPower and path-loss exponent. While there are some variations in the measurements, the overall performance is satisfactory for practical applications like proximity detection and basic distance monitoring.


