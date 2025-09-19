import serial.tools.list_ports
import time

ports = serial.tools.list_ports.comports()

# port_list = [port.name for port in ports]

# print(port_list)

port_obj = [0,'/dev/tty.usbmodem114101','/dev/tty.usbmodem114201','/dev/tty.usbmodem114401','/dev/tty.usbserial-11430']

for i in range(1,len(port_obj)):
    print(i)
    port_obj[i] = serial.Serial(
        f'{port_obj[i]}', 115200, timeout=1)
    
    if port_obj[i].isOpen():
        pass
    else:
        break

print("Done!")

obj_7 = ""
obj_8 = ""
obj_9 = []

print("안정화 중...")

time.sleep(5)

for i in range(1,len(port_obj)):
    port_obj[i].write('q'.encode())
    data = port_obj[i].read()
    time.sleep(1)
    if data == b'9':
        obj_9.append(i)
        print(obj_9)
    elif data == b'8':
        obj_8 = i
    elif data == b'7':
        obj_7 = i

print("안정화 끝!")

time.sleep(3)

for i in range(len(obj_9)):
    port_obj[obj_9[i]].write('c'.encode())

time.sleep(5)

for i in range(len(obj_9)):
    port_obj[obj_9[i]].write('n'.encode())


# port_obj[obj_7].write('r'.encode())
# port_obj[obj_8].write('r'.encode())

# time.sleep(5)

# port_obj[obj_7].write('g'.encode())
# port_obj[obj_8].write('g'.encode())

# time.sleep(5)

# port_obj[obj_7].write('a'.encode())
# port_obj[obj_8].write('a'.encode())

for i in range(1,len(port_obj)):
    port_obj[i].close()

print("Done2!")