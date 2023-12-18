# solid-tcp2can
Convert CAN-BUS frames from TCP to vcan0 interface

## Description
 
### SYSTEM 1
Linux Machine with physical CAN Bus devices.

### SYSTEM 2
Linux Machine without physical CAN Bus devices.

This tool can convert CAN-BUS frames received over a TCP connection into a format that can be sent to the vcan0 interface on a Linux machine. This allows SYSTEM 2, which does not have physical CAN Bus devices, to communicate with SYSTEM 1 as if it did.

## Usage

To use this tool, run the following command:

```bash
./solid-tcp2can
```

