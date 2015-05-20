# coding:utf-8
"""A
"""

import serial

try:
    dev = serial.Serial(port="/dev/ttyUSB0", baudrate=2400, bytesize=8, parity=serial.PARITY_ODD, stopbits=2);
except serial.SerialException:
    print("ERRO: Porta serial indisponível.");
    quit();

rx_result = [];

def rx(dev):
    newchar = list(dev.read());
    return newchar[0];

def rx_polling(dev):
    global rx_result;
    while not (rx(dev) == 255): continue
    while True:
        rx_result = rx(dev);
        print('rx_result:', rx_result) # SERIAL DEBUG

def tx(dev, data):
    if data == 'a':
        dev.write(bytes([1]));
        dev.write(bytes([1]));
        dev.write(bytes([13]));
    else:
        dev.write(bytes([1]));
        dev.write(bytes([0]));
        dev.write(bytes([13]));

rx_polling(dev);
