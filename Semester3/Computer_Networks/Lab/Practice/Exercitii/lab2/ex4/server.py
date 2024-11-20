import socket

clients = []
sorted_values = []

def server_UDP(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((host, port))
    print(f"Server listening on {host}:{port}")

    while True:
        try:
            data, client_addr = sock.recvfrom(1024)
            clients.append(client_addr)
            data =  data.decode()
            print(f"Recived message: {data} from {client_addr}")

            values,_ = sock.recvfrom(1024)
            values = values.decode()
            sorted_values1 = values.split()
            for elem in sorted_values1:
                sorted_values.append(int(elem))
            sorted_values.sort()
            print(f"sorted values: {sorted_values}")
            len_sort = len(sorted_values)
            message = f"Sorted values: {sorted_values} of length {len_sort}"

            sock.sendto(message.encode(), client_addr)
        except ValueError:
            sock.sendto("Invalid input".encode(), client_addr)
            print(f"Sending invalid input to {client_addr}\n")

if __name__ == "__main__":
    server_UDP("0.0.0.0", 8080)