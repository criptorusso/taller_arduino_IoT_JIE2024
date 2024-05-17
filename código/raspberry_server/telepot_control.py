#!/usr/bin/python3

import time
import telepot
import serial
import datetime
import RPi.GPIO as GPIO

TOKEN = 'COLOCAR_EL_TOKEN_DE_TU_BOT'   # token del bot
chat_id = "ID_DE_TU_CHAT"

rele = 11
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(rele, GPIO.OUT)

def ejecutar_accion(chat_id):
    bot.sendMessage(chat_id, 'ejecutando acción')
    estatus = GPIO.input(rele)
    if estatus == 1: estatus = 0
    else: estatus = 1
    GPIO.output(rele, estatus)
    return

def handle(msg):
    #bot.sendMessage(chat_id, "conectado")
    content_type, chat_type, chat_id = telepot.glance(msg)
    #print(content_type, chat_type, chat_id)
    fecha, hora = fecha_y_hora()
    print(fecha, hora, msg['text'])
    if content_type == 'text':
        if msg['text'] == '/control':
           ejecutar_accion(chat_id)
    return
   
def fecha_y_hora():
    fecha = datetime.date.today()
    hora = datetime.datetime.now()
    return fecha, str(hora.hour) + ':' + str(hora.minute) + ':' + str(hora.second)
        
def check_event_usb():
    s = serial.Serial('/dev/ttyUSB0') # open COM
    s.baudrate = 115000 # set Baud rate to 9600
    s.bytesize = 8    # Number of data bits = 8
    s.parity   = 'N'  # No parity
    s.stopbits = 1    # Number of Stop bits = 1
    msg = s.readline()
    msg = msg.decode('utf-8')
    pos = msg.find('t')
    temp = msg[pos+1:].removesuffix('\r\n')
    fecha, hora = fecha_y_hora()
    print(fecha, hora, temp, end='')
    if float(temp) >= 24:
        print(' -> temperatura elevada!!!')
        bot.sendMessage(chat_id, str(temp) + ' -> temp elevada')
    else:
        print(' -> temperatura normal')
    s.close()
    return

def check_event_file():
	f = open("evento", "r+")
	temp = (f.read()).rstrip()
	f.truncate(0)
	f.close()
	if temp != '':
		fecha, hora = fecha_y_hora()
		print(fecha, hora, temp)
		bot.sendMessage(chat_id, str(temp) + ' -> temp elevada')
	return

##### MAIN ######
bot = telepot.Bot(TOKEN)
bot.message_loop(handle)
print ('Leyendo...')

# Keep the program running.
while 1:
   try:
    check_event_file()
    time.sleep(10)    
   except Exception as e:
      print ('error', e, 'reconectando...')
      time.sleep(10)


