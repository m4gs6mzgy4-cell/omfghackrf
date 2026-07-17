
# omfghackrf — Unified RF Attack Framework

omfghackrf is a comprehensive, battle-tested framework for RF (Radio Frequency) attacks. It integrates signal capture, protocol handling, jamming, brute force, rolling code, vehicle/CAN, locker, and CCTV attacks into a single, unified engine. The framework is designed for criminalistic use to gain physical access to devices and systems.

## Features

- **Unified RF Engine**: All attack libraries are integrated into a single engine, making it easy to use with the existing UI and launchers.
- **Signal Capture**: Capture and replay RF signals.
- **Jamming**: Jam frequencies with noise.
- **Sniffing**: Sniff and log RF signals.
- **Injection**: Inject custom signals.
- **Modulation**: Modulate signals.
- **Sweeping**: Sweep frequencies.
- **Chirping**: Send chirp signals.
- **Pulsing**: Send pulse signals.
- **Noise**: Send noise signals.
- **Decoding**: Decode signals.
- **Encoding**: Encode signals.
- **Scanning**: Scan frequencies for activity.
- **Recording**: Record signals to a file.
- **Brute Force**: Brute-force signals.
- **Rolling Code**: Attack rolling code systems.
- **Vehicle CAN**: Attack vehicle CAN systems.
- **Locker**: Attack locker systems.
- **CCTV**: Attack CCTV systems.

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/m4gs6mzgy4-cell/omfghackrf.git
   cd omfghackrf
   ```

2. **Install dependencies:**
   ```sh
   sudo apt-get install libhackrf-dev build-essential
   ```

3. **Build the engine:**
   ```sh
   make
   ```

## Usage

### Running an Attack

```sh
./rf_engine <attack_name> <frequency> <duration>
```

Example:
```sh
./rf_engine replay 433920000 3
./rf_engine jam 433920000 5
```

### List of Attacks

```sh
./rf_engine --list
```

## Attacks

- `replay`: Replay a captured signal.
- `jam`: Jam a frequency with noise.
- `sniff`: Sniff and log RF signals.
- `inject`: Inject custom signals.
- `modulate`: Modulate signals.
- `sweep`: Sweep frequencies.
- `chirp`: Send chirp signals.
- `pulse`: Send pulse signals.
- `noise`: Send noise signals.
- `decode`: Decode signals.
- `encode`: Encode signals.
- `scan`: Scan frequencies for activity.
- `record`: Record signals to a file.
- `bruteforce`: Brute-force signals.
- `rolling_code`: Attack rolling code systems.
- `vehicle_can`: Attack vehicle CAN systems.
- `locker`: Attack locker systems.
- `cctv`: Attack CCTV systems.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Disclaimer

This tool is for educational and research purposes only. Use it responsibly and legally. The authors are not responsible for any misuse or damage caused by this tool.