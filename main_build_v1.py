import serial
from time import sleep
import time
import csv
import datetime

today = datetime.date.today()

fileName = "eletro_post.csv"
mcu_connection = 'COM8'  # ('/dev/ttyUSB1')
serial = serial.Serial(mcu_connection, 9600)
serial.flushInput()
sleep(5)

black_list =[]
white_list = ["D6 D7 D8 DF"]
user_list = {'NFCID:': None}  # NFCID: USUÁRIO
succ_list = {'NFCID:': None, 'email:': None, 'DMY:': None}  # NFCID, EMAIL, DATA/HORA, CONSUMO
error_list = {'NFCID:': None, 'email:': None, 'DMY:': None, 'error:': None}  # NFCIOD, EMAIL, DATA/HORA, ERROR


def decode_format(data):
    decode_data = data.decode("utf-8")
    out_data = decode_data[0:][:-2]
    return out_data


def unpack_data(inc_data):
    dep_data = inc_data.split()
    dep_data[1] = float(dep_data[1])
    dec_data = {dep_data[0]: dep_data[1]}
    return dec_data


def serial_comm(cmd_in, time_in, flush):
    if flush:
        serial.flushInput()
    cmd = bytes(str(cmd_in), "utf-8")
    serial.write(cmd)
    sleep(time_in)
    data = serial.readline()
    decode_data = data.decode("utf-8")
    serial_data_out = decode_data[0:][:-2]
    return serial_data_out


while True:
    try:
        serial.flushInput()
        data_1 = b'1'
        data_2 = b'2'
        serial.write(data_1)
        sleep(2)
        state_check = decode_format(serial.readline())
        try:  # CARD_CHECK
            if state_check == 'ST_SNFC':  # b'1'
                serial.flushInput()
                serial.write(data_2)
                sleep(.5)
                serial_data = decode_format(serial.readline())
                print(serial_data)
                sleep(2)
                id_check = decode_format(serial.readline())
                print(id_check)
                if id_check in white_list:
                    serial.flushInput()
                    serial.write(b'3')  # b'NFC-OK' Estado de leitura OK, o MCU irá o retorno do MCU será CABLE_CHECK
                    sleep(.5)
                    print("White list ok")
                    # with open(str('succs')+f'_data{today}', "a") as f:
                    #    writer = csv.writer(f, delimiter=";")
                    #    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
                else:
                    serial.write(b'4')  # b'NFC-NOK' Estado de leitura falhou, o retorno do MCU será o CARD_CHECK
                    sleep(1)
                    with open(str('unsuccs')+f'_data{today}', "a") as f:
                        writer = csv.writer(f, delimiter=";")
                        writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
            elif state_check == 'ST_WAIT_IR':
                print(f'Aguardando aproximação no sensor')
                sleep(1)
        except:
            continue
        try:  # CABLE_CHECK
            state_check = decode_format(serial.readline())
            sleep(1)
            if state_check == "ST_IDCOK":
                serial.flushInput()
                serial.write(b'2')  # b'ST_IDREQ' Solicitação de ID NFCARD
                sleep(.5)
                serial_data = decode_format(serial.readline())
                sleep(2)
                id_check = decode_format(serial.readline())
                print(id_check)
                print("Cable check")
            # elif state_check == 'CABLE_CONNECTED':
                '''Definir função para cálculo de consumo'''
                consumo = None
                with open(str('succs') + f'_data{today}', "a") as f:
                    writer = csv.writer(f, delimiter=";")
                    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)};{consumo}'])
#            elif state_check == 'CABLE_ERROR':
#                with open(str('error') + f'_log{today}', "a") as f:
#                   writer = csv.writer(f, delimiter=";")
#                    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
        except:
            continue
    except:
        print("Script interrompido")
        break