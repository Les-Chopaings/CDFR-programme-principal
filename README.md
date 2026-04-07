# CDFR Programme Principal

Main program for the CDFR robotics project.

---

## Prerequisites

Install the required dependencies:

```bash
sudo apt update
sudo apt install g++ make libi2c-dev
sudo apt install g++-aarch64-linux-gnu   # For cross-compilation only
```

Verify your compilers:

```bash
g++ --version
aarch64-linux-gnu-g++ --version
```

---

## Getting Started

Clone the repository and initialize submodules:

```bash
git clone git@github.com:Les-Chopaings/CDFR-programme-principal.git
cd CDFR-programme-principal
git submodule update --init
```

---

## Build & Run

> ⚠️ **Always run `make clean-all` before switching between native and cross-compilation**, otherwise the build system may mix incompatible object files.

### Native compilation (x86 or ARM on device)

Build and run directly on your machine (x86 emulation is enabled automatically):

```bash
make
cd bin && ./programCDFR
```

### Cross-compilation (targeting ARM, e.g. Raspberry Pi)

Build the ARM binary from your host machine:

```bash
make clean-all   # Required if you previously run a native build
make install
```

Then deploy and run on the target device (update the IP address as needed):

```bash
ssh ubuntu@192.168.1.99
cd ~/CDFR_PROG/arm_bin/
./programCDFR
```

> **Note:** Update the target IP address (`192.168.1.99`) in the command above and in the makefile to match your device's address on the network.

---

## Project Structure

| Path | Description |
|------|-------------|
| `bin/` | Native build output |
| `~/CDFR_PROG/arm_bin/` | ARM cross-compiled binary (on target device) |

---

## Contributing

This project is maintained by [Les-Chopaings](https://github.com/Les-Chopaings).