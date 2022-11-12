# Transmission Content Definition
## Transmission Frame Discription
The transmission process is made up of frames. Each frame has 16 bytes, and is composed of three parts: Frame head, Frame body, and CRC byte.

### Frame Head
The frame head decides what type of data this frame contains. The definitnons of frame heads are listed in the folowing table.

| Frame Head    | Description               |
| -----------   | -----------               |
| 0xC0          | (H)Control Frame          |
| 0xD0          | (H)Hole Data Frame        |
| 0xE0          | (C)Client Response        |
| 0xEF          | (H/C)CRC Error            |

*The letter before descrption means this message could only send by host or client.

### Frame body
The frame body takes 6 bytes after frame head. Content of frame body is various by frame head. 

"-----------" in following tables means the rest bytes are filled with 0x00.

#### Control Frame
Since there's no much controlling commands, only the first byte of frame body in control frame is effective, and all other bytes should be filled with 0x00. The definitions are as follow.

| Frame Body        | Description                   |
| -----------       | -----------                   |
| 0x01 -----------  | Heart beat package            |
| 0x10 -----------  | Query Client Status           |
| 0x11 -----------  | Start Punch Mission           |
| 0x20 -----------  | Enable Serial Command         |
| 0x21 -----------  | Enable Wifi Command           |
| 0x30 -----------  | Disable Serial Command        |
| 0x31 -----------  | Disable Wifi Command          |
| 0xA0 -----------  | Hole Data Transmission Begin  |
| 0xA1 -----------  | Hole Data Transmission End    |
| 0xB0 - uint16 --  | Set cursor to hole X          |

#### Hole Data Frame
Each hole data frame carries position data of one hole (related to pervious hole).

| X Position    | Y position         | Description                   |
| -----------   | -----------        | -----------                   |
| 0xMM          | 8 byte double ----  | Hole at X potision # MM, Z position equals the float.    |

#### Client Response
Frame body in client response frames contains responses from the client. The first byte defines response type, and the rest 5 bytes are the message.

| Response Type | Message           | Description                       |
| -----------   | -----------       | -----------                       |
| 0x01          | -----------       | Client Response                   |
| 0x01          | 0x01 -----------  | Client Response: Success          |
| 0x01          | 0x10 -----------  | Client Response: Heart Beat       |
| ------------- | ----------------- | --------------------------------- |
| 0x10          | -----------       | Client Status                     |
| 0x10          | 0x01 -----------  | Client Status: Ready to transmit. |
| 0x10          | 0x0F -----------  | Client Status: Transmit disabled. |
| 0x10          | 0x10 -----------  | Client Status: Idle.              |
| 0x10          | 0x11 -----------  | Client Status: Punching.          |
| ------------- | ----------------- | --------------------------------- |
| 0xA0          | -----------       | Client Variables                  |
| 0xA0          | menuId --value--  | Client Variable: MenuId + value   |
| ------------- | ----------------- | --------------------------------- |
| 0xE0          | -----------       | Runtime Error                     |
| 0xE0          | 0x01 -----------  | Runtime Error: No hole data.      |
| 0xE0          | 0x02 -----------  | Runtime Error: Data out of range. |
| 0xE0          | 0x11 -----------  | Runtime Error: Puncher busy.      |
| 0xE0          | 0xA0 -----------  | Runtime Error: Serial command disabled.   |
| 0xE0          | 0xA1 -----------  | Runtime Error: Wifi command disabled.     |
| 0xE0          | 0xFF -----------  | Runtime Error: Unknown command    |
| ------------- | ----------------- | --------------------------------- |
| 0xE1          | -----------       | Data Error                        |
| 0xE1          | 0x01 -----------  | Data Error: X out of range.       |
| ------------- | ----------------- | --------------------------------- |
| 0xE2          | -----------       | Hardware Failure                  |
| 0xE2          | 0x01 0xMM ------  | Hardware Failure: Unable to read motor load values of motor #MM. (00 - X, 01 - Y) |
| 0xE2          | 0x02 -----------  | Hardware Failure: Unable to read magnet encoder values. |
| 0xE2          | 0x03 -----------  | Hardware Failure: Z axis got jammed.      |
| 0xE2          | 0x04 -----------  | Hardware Failure: Encoder not detected.   |

#### CRC Error Frame
CRC error frame means there's an error during transmission. No data would be contained in this frame.

### CRC Byte
CRC is a widely used verfying algorithm. CRC8 is used in this protocol, derived from the previous 7 bytes.


## Sending Movements to Client
### Step 1. Host
abc