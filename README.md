# HW07

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

### Results Table

| Actual Distance (m) | Measured Distance (m) | Error (m) |
|---------------------|------------------------|-----------|
| 0.5                 | 0.39                   | 0.11      |
| 1.0                 | 1.00                   | 0.00      |
| 2.0                 | 2.31                   | 0.31      |
| 3.0                 | 3.51                   | 0.51      |
| 4.0                 | 4.81                   | 0.81      |

<img src="https://github.com/tmin002/IoT25-HW07/blob/main/graph.png" width="500">

### Analysis

The initial model our team built, which used `txPower = 0` and `n = 4.58`, resulted in significant overestimation of distances and was not suitable for practical use. One major issue was the use of an arbitrary `txPower` value (0 dBm), which did not reflect the actual signal strength observed at a known reference distance. In BLE-based distance estimation, `txPower` should represent the expected RSSI at a distance of 1 meter, as it serves as the baseline for the path-loss model. In our environment, the average RSSI at 1 meter was measured to be approximately `-58 dBm`, and we updated our model accordingly. After calibrating the model by setting `txPower = -58 dBm` and adjusting the path-loss exponent to `n = 2.2`, the distance estimates aligned much more closely with the actual values. Most measurements now fall within ±0.8 meters of the true distance, and the estimate at 1 meter is exactly accurate. As expected, minor errors increase with distance due to RSSI fluctuations, but overall the model performs reliably for short-range BLE-based distance estimation.
