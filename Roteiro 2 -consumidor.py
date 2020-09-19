from threading import Thread, Condition
from time import sleep
from random import randint

total_buffer = 5

buffer = list()
Mutex = Condition()

def produtor():
    global buffer
    while True:
        Mutex.acquire()

        buffer.append(randint(0,10))
        print("Produzido ", buffer)
        Mutex.notify()

        if len(buffer) == total_buffer:
            Mutex.notify()
            Mutex.wait()  # Coloca pra dormir

        Mutex.release()
        sleep(0.2)

def consumidor():
    global buffer
    while True:
        Mutex.acquire()

        if len(buffer) == 0:
            Mutex.wait() # Coloca pra dormir
            print("Buffer Vazio")

        print("Retirou {}".format(buffer.pop()))

        Mutex.notify()
        Mutex.release()

        sleep(2)

a = Thread(target=produtor)
b = Thread(target=consumidor)

a.start()
b.start()




