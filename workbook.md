# Sources:

- https://www.youtube.com/watch?v=WikQ5n1QXQs
- https://www.youtube.com/watch?v=2Mhqwt2xTxk
- https://www.youtube.com/watch?v=icEx1Q_U7J0
- https://www.circuitstate.com/tutorials/what-is-can-bus-how-to-use-can-interface-with-esp32-and-arduino/#Signaling
- https://netmodule-linux.readthedocs.io/en/latest/howto/can.html

# CAN

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=0
- Controller Area Network: serial communication protocol
- Developed by Bosch for the automotive industry

### CAN Topology

<img src="https://raw.githubusercontent.com/DNadas98/can_protocol/main/img/01_can_topology.png" style=" width: 100%; height: auto; max-width: 800px;"/>

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=63
- Shared BUS topology: distributed can nodes, no master/slave devices
- Two-wire differential BUS: twisted pair to avoid EMI
  - differential BUS: two wires carry the same information at the same time but in opposite polarities
  - EMI: electromagnetic interference
- Terminated by 120 Ohm resistors
  - Reduces signal reflections

### CAN Message Arbitration

<img src="https://raw.githubusercontent.com/DNadas98/can_protocol/main/img/02_can_message_arbitration.png" style=" width: 100%; height: auto; max-width: 800px;"/>

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=180
- CAN Nodes have IDs
- On CAN BUS, 0 overpowers 1 => lower ID overpowers higher ID
- Nodes "know" when they have transmitted successfully:
  - Nodes can transmit and read their own transmissions
  - node reads back its own ID from the bus => successful transmission

### CAN Voltage Levels

<img src="https://raw.githubusercontent.com/DNadas98/can_protocol/main/img/03_can_signal_PHY.png" style=" width: 100%; height: auto; max-width: 800px;"/>

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=330
- https://www.circuitstate.com/tutorials/what-is-can-bus-how-to-use-can-interface-with-esp32-and-arduino/#Signaling
- OSI 1. physical layer
- CAN_H: 2.5V to 3.5V
- CAN_L: 2.5V to 1.5V
- CAN High Speed ISO 11898-2:
  - 0, dominant voltage: 2V difference between CAN_H and CAN_L
  - 1, recessive voltage : < 0.5V difference
  - max 1 Mbit/s on CAN, 5 Mbit/s on CAN-FD (flexible data rate)
  - topology: bus
- CAN Low Speed ISO 11898-3:
  - 0, dominant voltage: 1.5V difference between CAN_H and CAN_L
  - 1, recessive voltage: < 0.5V difference
  - max 125 kbit/s
  - topology: bus, star
- CAN controllers are compatible, transceivers are not

### CAN Signal

<img src="https://raw.githubusercontent.com/DNadas98/can_protocol/main/img/04_can_signal_Data_Link.png" style=" width: 100%; height: auto; max-width: 800px;"/>

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=440
- OSI 2. data link layer
- 64 bits of data in a transmission
- `SOF` (start of frame): 1 bit, dominant voltage
- `Arbitration` field: 11 bits
  - stuff bit: 1 bit, recessive voltage, a "check" of the transmission
- `Control` field: 6 bits
  - DLC: data length code, 4 bits, number of bytes in the `Data` field
- `Data` field: 0-8 bytes
- `CRC` field: 15 bits, cyclic redundancy check
  - cryptographic hash function to verify no corruption in the data
- `ACK` field: 2 bits
  - acknowledge field, recessive voltage
- `EOF` (end of frame): 7 bits, recessive voltage
- `IFS` (interframe space): 3 bits, recessive voltage
  - idle time between frames

### ECU: Electronic Control Unit

<img src="https://raw.githubusercontent.com/DNadas98/can_protocol/main/img/05_can_ECU.png" style=" width: 100%; height: auto; max-width: 800px;"/>

- https://youtu.be/WikQ5n1QXQs?feature=shared&t=615
- CAN node: microcontroller with CAN controller
- CAN controller implements the Data Link layer of the protocol
- CAN transceiver: converts the digital signal to the physical signal on the bus, Physical layer

# CAN Interface

### Virtual CAN Interface - vcan

- Load VCAN kernel module
  ```bash
  sudo modprobe vcan
  ```
- Check if the module is loaded successfully
  ```bash
  lsmod | grep vcan
  ```
- Create VCAN interface
  ```bash
  sudo ip link add dev vcan0 type vcan && \
  sudo ip link set up vcan0 && \
  sudo ip link ls | grep vcan0
  ```

- Install CAN Utils
  ```bash
  sudo apt-get update && \
  sudo apt-get install can-utils -y
  ```

- Test VCAN interface: open two terminals
  ```bash
  # 1.
  candump vcan0
  ```
  ```bash
  # 2. Send "hello" "world"
  cansend vcan0 123#68656C6C6F
  cansend vcan0 123#776F726C64
  ```