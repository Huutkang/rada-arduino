import serial, time

arduinoData = serial.Serial('com3', 9600)
time.sleep(1)


run = True


try:
    while run:
        while (arduinoData.in_waiting==0):
            pass
        dataPacket = arduinoData.readline().decode('utf-8').strip('\r\n')
        print(dataPacket)
except Exception as e:
    print(e)
    arduinoData.close()



