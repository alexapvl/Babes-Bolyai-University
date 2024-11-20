import socket
import sys

if __name__ == '__main__':
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1234))
    except socket.error:
        print('Socket error')
        sys.exit(1)
    command = input("Introduce shell command and parameters: ")
    s.send(command.encode())

    output = s.recv(1024)
    print("Result of the command: \n", output.decode())
    s.close()