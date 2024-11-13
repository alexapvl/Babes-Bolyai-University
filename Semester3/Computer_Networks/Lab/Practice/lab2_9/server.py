import socket
import threading
import random
import struct
import sys
import time

random.seed()
start=1; stop=2**17-1
my_num= random.randint(start,stop) # generate random number
print('Server number: ',my_num)
mylock = threading.Lock()
client_guessed=False
winner_thread=0
e = threading.Event()
e.clear()
threads = []
client_count=0

def worker(client_socket):
    global mylock, client_guessed, my_num, winner_thread, client_count,e

    my_idcount=client_count
    print('client #',client_count,'from: ',client_socket.getpeername(), client_socket )
    message='Hello client #'+str(client_count)+' ! You are entering the number guess competion now !'
    client_socket.sendall(bytes(message,'ascii'))

    while not client_guessed:
        try:
            cnumber=client_socket.recv(4)
            cnumber=struct.unpack('!I',cnumber)[0]
            if cnumber > my_num:
                client_socket.sendall(b'S')
            if cnumber < my_num:
                client_socket.sendall(b'H')
            if cnumber== my_num:
                mylock.acquire()
                client_guessed=True
                winner_thread=threading.get_ident()
                mylock.release()

        except socket.error as msg:
            print('Error:',msg.strerror)
            break

    if client_guessed:
        if threading.get_ident() == winner_thread:
            client_socket.sendall(b'G')
            print('We have a winner', client_socket.getpeername())
            print("Thread ",my_idcount," winner")
            e.set()
        else:
            client_socket.sendall(b'L')
            print("Thread ",my_idcount," looser")
    time.sleep(1)
    client_socket.close()
    print("Worker Thread ",my_idcount, " end")


def resetSrv():
    global mylock, client_guessed, winner_thread, my_num, threads,e, client_count
    while True:
        e.wait()
        for t in threads:
            t.join()
        print("all threads are finished now")
        e.clear()
        mylock.acquire()
        threads = []
        client_guessed = False
        winner_thread=-1
        client_count = 0
        my_num = random.randint(start,stop)
        print('Server number: ',my_num)
        mylock.release()


if __name__=='__main__':
    try:
        rs=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        rs.bind( ('localhost',1234) )
        rs.listen(5)
    except socket.error as msg:
        print(msg.strerror)
        exit(-1)
    t=threading.Thread(target=resetSrv, daemon=True)
    t.start()
    while True:
        client_socket,addrc = rs.accept()
        t = threading.Thread(target=worker, args=(client_socket,) )
        threads.append(t)
        client_count+=1
        t.start()

