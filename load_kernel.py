import serial
import sys
import os
import struct
import time

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200

BOOTLOADER_READY_MSG = b"[Bootloader] UART initialized\r\n"
DATA_ACK = b"."

def main():
    kernel_img_path = "build/kernel8.img"

    with open(kernel_img_path, "rb") as f:
        kernel_data = f.read()
    kernel_size = len(kernel_data)
    print(f"Loaded kernel '{kernel_img_path}': {kernel_size} bytes.")

    local_checksum = sum(kernel_data) & 0xFFFFFFFF
    print(f"Kernel Checksum: {hex(local_checksum)}")

    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=10) as tty:
        print(f"Opened {SERIAL_PORT} at {BAUD_RATE} bps.")

        print("Waiting for RPi bootloader ready signal...")
        ready_msg = tty.read_until(BOOTLOADER_READY_MSG)

        print("RPi is ready. Sending kernel size...")
        tty.write(struct.pack('<I', kernel_size))
        len_msg = tty.read_until(b"\r\n")
        print(f"Received from RPi: {len_msg.decode().strip()}")

        print("RPi acknowledged size. Sending kernel data...")
        for i in range(kernel_size):
            tty.write(kernel_data[i:i+1])

        time.sleep(1)
        done_msg = tty.read_until(b"\r\n")
        print(f"Received from RPi: {done_msg.decode().strip()}")

if __name__ == "__main__":
    main()
