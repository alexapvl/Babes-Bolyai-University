import socket
import sys

values = []

def client_UDP(server_ip, port):
    global values
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        while True:
            nr = input ("Enter the number of elem: ")
            sock.sendto(nr.encode(), (server_ip, port))
            nr1 = int(nr)

            while nr1 > 0:
                values.append(input("Enter a value: "))
                nr1 -= 1

            values1 = " ".join(values)
            sock.sendto(str(values1).encode(), (server_ip, port))

            message, _ = sock.recvfrom(1024)
            recived_mess = message.decode()
            print(f"Recived message: {recived_mess}")

    except KeyboardInterrupt:
        print("Client stopped")

    finally:
        sock.close()

if __name__ == "__main__":

    if len(sys.argv) != 3:
        print("Usage example: python3 client.py <serv.ip> <port>")
        sys.exit(1)

    server_ip = sys.argv[1]
    port = int(sys.argv[2])

    client_UDP(server_ip, port)