import socket
import threading
import subprocess

def newUser(conn: socket.socket):
    command = conn.recv(1024).decode('utf-8')
    print(conn.getsockname(), ": Received command: " + command)
    result = subprocess.check_output(command, shell=True, text=True)
    result = result.encode('utf-8')
    print(conn.getsockname(),": Sent result: ", result)
    conn.sendall(result)
    conn.close()

if __name__ == '__main__':
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(('localhost', 1234))
        s.listen(5)
    except socket.error:
        print('Socket error')
        exit(1)
    while True:
        conn, addr = s.accept()
        th = threading.Thread(target=newUser, args=(conn,))
        th.start()