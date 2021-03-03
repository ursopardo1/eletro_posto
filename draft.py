from eletro_function import decode_format
import serial
from time import sleep
mcu_connection = 'COM8'  # ('/dev/ttyUSB1')
serial = serial.Serial(mcu_connection, 9600)
data_1 = b'1'
data_2 = b'2'
data_3 = b'3'
data_4 = b'4'
data_5 = b'5'

# Loop 1
for i in range(1, 10):
    serial.flushInput()
    serial.write(data_1)
    sleep(1.5)
    print(decode_format(serial.readline()))
    serial.write(data_2)
    sleep(1.5)
    print(decode_format(serial.readline()))
    serial.write(data_3)
    sleep(1.5)
    print(decode_format(serial.readline()))


for i in range(1, 10):
    serial.write(data_4)
    sleep(1.5)
    # print(decode_format(serial.readline()))
    print(serial.readline())
    sleep(.5)

serial.write(data_5)
sleep(1.5)
print(serial.readline())
sleep(.5)
