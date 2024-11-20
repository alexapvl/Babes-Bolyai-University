# lab3ex2server.py
import socket
import threading
import random
import struct
import sys
import time


random.seed()
start = 1
stop = 2**10 - 1
my_num = random.randint(start, stop)
print('Server number: ', my_num)
mylock = threading.Lock()
client_guessed = False
winner_thread = None
client_count = 0
e = threading.Event()
e.clear()

# Dictionary to store each client's status based on address
clients_status = {}

def handle_guess(data, client_addr, s):
    global mylock, client_guessed, my_num, winner_thread, client_count, e

    # Check if received data has exactly 4 bytes before unpacking
    if len(data) != 4:
        print(f"Received invalid data length from {client_addr}: {len(data)} bytes")
        return

    cnumber = struct.unpack('!I', data)[0]
    response = None

    with mylock:
        if cnumber > my_num:
            response = b'S'
        elif cnumber < my_num:
            response = b'H'
        else:
            if not client_guessed:
                client_guessed = True
                winner_thread = client_addr
                response = b'G'
                print(f'We have a winner: {client_addr}')
                e.set()
            else:
                response = b'L'

    if response:
        s.sendto(response, client_addr)
        print(f"Sent {response.decode()} to {client_addr}")

def reset_server():
    global mylock, client_guessed, winner_thread, my_num, client_count, e
    while True:
        e.wait()
        print("All threads are finished now")
        e.clear()
        with mylock:
            client_guessed = False
            winner_thread = None
            client_count = 0
            my_num = random.randint(start, stop)
            print('New server number: ', my_num)

if __name__ == '__main__':
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind(('0.0.0.0', 1234))
        print("Server is ready and listening on UDP port 1234")
    except socket.error as msg:
        print("Socket error:", msg.strerror)
        exit(-1)

    threading.Thread(target=reset_server, daemon=True).start()

    while True:
        data, client_addr = s.recvfrom(1024)
        if client_addr not in clients_status:
            clients_status[client_addr] = client_count
            client_count += 1
            welcome_message = f"Hello client #{clients_status[client_addr]}! You are entering the number guess competition now!"
            s.sendto(welcome_message.encode('ascii'), client_addr)

        handle_guess(data, client_addr, s)