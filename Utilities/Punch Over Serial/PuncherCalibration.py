# Please set the calibration params to 0 before calibration.

from serial import Serial

PUNCHER_SERIAL = "COM7" # com device

if __name__ == "__main__":
    ser = Serial(PUNCHER_SERIAL, 115200, timeout=5)

    ba = bytearray("M80 Y96 P0".encode("ASCII"))
    ser.write(ba)

    for _ in range(148):
        ba = bytearray("M0 Y96 P0".encode("ASCII"))
        ser.write(ba)

    ba = bytearray("M90 Y96 P0".encode("ASCII"))
    ser.write(ba)

    ser.close()