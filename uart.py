import serial
import clientClass
import threading
import time

baudrate = 115200
statusPin = 16

def start_UART_relay(_serial, _client, _printable=False):
        while True:

            print("s1")
            sendmessage = bytearray([10 ,18, 8 ,127, 16, 127, 24 ,127, 32 ,127, 40, 127, 48 ,127, 56, 137, 1 ,64, 137, 1, 75, 46])
            sendmessage2 = bytearray([10, 18, 8, 127, 16, 127, 24,127, 32, 127, 40, 127, 48, 127, 56, 187, 1, 64, 187, 1, 75, 46])
            _serial.write(sendmessage)
            time.sleep(0.2)
            _serial.write(sendmessage2)
            time.sleep(0.2)

# ERIC EDIT THE COM PORT!
#-----------------------------------
# run python uart.py
#-----------------------------------
serial_mcu = serial.Serial('COM3', baudrate, timeout=1)

while True:

    print("s1")
    sendmessage = bytearray([10 ,18, 8 ,127, 16, 127, 24 ,127, 32 ,127, 40, 127, 48 ,127, 56, 137, 1 ,64, 137, 1, 75, 46])
    sendmessage2 = bytearray([10, 18, 8, 127, 16, 127, 24,127, 32, 127, 40, 127, 48, 127, 56, 187, 1, 64, 187, 1, 75, 46])
    serial_mcu.write(sendmessage)
    time.sleep(0.1)
    serial_mcu.write(sendmessage)
    time.sleep(0.1)
    serial_mcu.write(sendmessage)
    time.sleep(0.1)
    serial_mcu.write(sendmessage)
    time.sleep(0.1)
    serial_mcu.write(sendmessage)
    time.sleep(0.1)


    serial_mcu.write(sendmessage2)
    time.sleep(0.1)
    serial_mcu.write(sendmessage2)
    time.sleep(0.1)
    serial_mcu.write(sendmessage2)
    time.sleep(0.1)
    serial_mcu.write(sendmessage2)
    time.sleep(0.1)
    serial_mcu.write(sendmessage2)
    time.sleep(0.1)



debug_mcu = serial.Serial('COM7', baudrate, timeout=1)

thread_alive = True

client = clientClass.TCPClient("192.168.2.142", 7000, serial_mcu, statusPin)

## Uncomment for third serial connection, used for debug
# debug = serial.Serial('/dev/serial0', 115200)
# debug_client = clientClass.UART_TCPClient("10.0.0.106", 9000, debug)

client.connect()

receiveThread_Serial = threading.Thread(target=start_UART_relay, args = (serial_mcu, client))
receiveThread_Debug = threading.Thread(target=start_UART_relay, args = (debug_mcu, client, True))

receiveThread_Serial.start()
receiveThread_Debug.start()