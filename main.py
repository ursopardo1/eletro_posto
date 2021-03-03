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

black_list =[]
white_list = []
user_list = {'NFCID:': None}  # NFCID: USUÁRIO
succ_list = {'NFCID:': None, 'email:': None, 'DMY:': None}  # NFCID, EMAIL, DATA/HORA, CONSUMO
error_list = {'NFCID:': None, 'email:': None, 'DMY:': None, 'error:': None}  # NFCIOD, EMAIL, DATA/HORA, ERROR

while True:
    try:
        serial.flushInput()
        state_check = serial.readline()  # Leitura de ESTADO
        try: # CARD_CHECK
            if state_check == b'SNFC':  # b'1'
                serial.write(b'IDREQ') # b'2'  # Solicitação de ID NFCARD
                sleep(.5)
                id_check = serial.readline()
                if id_check in white_list:
                    serial.write(b'NFC-OK')  # b'3' Estado de leitura OK, o MCU irá o retorno do MCU será CABLE_CHECK
                    sleep(.5)
                    # with open(str('succs')+f'_data{today}', "a") as f:
                    #    writer = csv.writer(f, delimiter=";")
                    #    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
                else:
                    serial.write(b'NFC-NOK')  # b'4' Estado de leitura falhou, o retorno do MCU será o CARD_CHECK
                    sleep(.5)
                    with open(str('unsuccs')+f'_data{today}', "a") as f:
                        writer = csv.writer(f, delimiter=";")
                        writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
            elif state_check == b'WAIT_IR':
                print(f'Aguardando aproximação no sensor')
                sleep(1)
        except:
            continue
        try:  # CABLE_CHECK
            state_check = serial.readline()
            serial.write(b'IDREQ')  # b'3' Solicitação de ID NFCARD
            sleep(.5)
            id_check = serial.readline()
            sleep(.5)

            if state_check == 'CABLE_CONNECTED':
                '''Definir função para cálculo de consumo'''
                consumo = None

                with open(str('succs') + f'_data{today}', "a") as f:
                    writer = csv.writer(f, delimiter=";")
                    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)};{consumo}'])
            elif state_check == 'CABLE_ERROR':
                with open(str('error') + f'_log{today}', "a") as f:
                    writer = csv.writer(f, delimiter=";")
                    writer.writerow([time.asctime(), f'{id_check};{user_list.get(id_check)}'])
        except:
            continue
    except:
        print("Script interrompido")
        break
