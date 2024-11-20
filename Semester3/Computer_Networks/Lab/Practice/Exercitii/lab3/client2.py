#lab3ex2client.py
import socket
import struct
import random
import time


if __name__ == '__main__':
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_address = ('10.139.1.84', 1234)
    except socket.error as msg:
        print("Error:", msg.strerror)
        exit(-1)

    finished = False
    sr = 1
    er = 2**10 - 1
    random.seed()
    step_count = 0

    # Initial message from server
    s.sendto(b'Hello', server_address)
    data, _ = s.recvfrom(1024)
    print(data.decode('ascii'))

    while not finished:
        my_num = random.randint(sr, er)
        try:
            s.sendto(struct.pack('!I', my_num), server_address)
            answer, _ = s.recvfrom(1)
        except socket.error as msg:
            print("Error:", msg.strerror)
            s.close()
            exit(-2)

        step_count += 1
        print('Sent', my_num, 'Answer', answer.decode('ascii'))
        
        if answer == b'H':
            sr = my_num
        elif answer == b'S':
            er = my_num
        elif answer in (b'G', b'L'):
            finished = True

        time.sleep(0.25)

    s.close()
    if answer == b'G':
        print("I am the winner with", my_num, "in", step_count, "steps")
    else:
        print("I lost!!!")