import socket
import threading

'''
This is the mechanism through which the client communicates with the server
When the client connects, it sends a string which starts with "REGISTER" to the server, followed by,
the udp port and the tcp port so the server knows that a new client connected

When the client disconnects, it sends a string which starts with "QUIT" followed by the udp port 
and the tcp port of the client

After each new connection, the server sends the updated peers list to all clients
In the client, when updating the peer list, it creates a new peer list and after that it checks for
differences between the previous list and the new one, after which it prints those differences on the
screen, i.e. it checks for new connections and disconnections.

That's why when a new client connects to the server, it displays all the currently connected clients up until now, because his(the client) peer list was empty when it connected to the server

The messages exchanged between client-client and client-server are maximum 1024 bytes, 
i.e. 1024 characters in length
'''

# Global dictionary to store peer information
peers = {}

def handle_client(conn, addr):
    # print(f"Connected to client: {addr}") # debugging
    try:
        data = conn.recv(1024).decode()
        # print(f"Received from {addr}: {data}") # debugging 

        if data.startswith("REGISTER"):
            udp_port, tcp_port = data.split()[1], data.split()[2]
            print(f"Registered client: {addr} with UDP:{udp_port} TCP:{tcp_port}")
            peers[(addr[0], udp_port, tcp_port)] = (addr[0], udp_port, tcp_port)
            conn.sendall(b"REGISTERED\n")
            send_client_list()  # Send updated client list to all clients
        elif data.startswith("QUIT"): # if client sends "QUIT" message, remove it from the peers dictionary
            # Remove the client from the peers dictionary based on its unique (IP, UDP, TCP) tuple
            for key in list(peers.keys()):
                ip, udp_port, tcp_port = key
                if ip == addr[0] and udp_port == data.split()[1] and tcp_port == data.split()[2]:
                    del peers[key]
                    print(f"Unregistered client: {addr}")
                    break
            conn.sendall(b"UNREGISTERED")
            send_client_list()  # Send updated client list after removal
    except Exception as e:
        print(f"Error with client {addr}: {e}")
    finally:
        print(f"We currently have {len(peers)} client(s) connected")
        conn.close()

def send_client_list():
    # Send the current client list to all connected clients
    for client in peers.values():
        ip, udp_port, tcp_port = client
        client_data = "\n".join([f"{ip} {udp_port} {tcp_port}" for ip, udp_port, tcp_port in peers.values()])
        # Send client list to each client
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((ip, int(tcp_port)))
            client_socket.sendall(f"CLIENT_LIST\n{client_data}".encode())
            client_socket.close()
        except Exception as e:
            print(f"Error sending client list to {ip}: {e}")

def start_server():
    print("Starting server...")
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(("172.30.247.175", 5000))  # Server listens on port 5000
    server_socket.listen(5)
    print("Server listening on port 5000")

    while True:
        try:
            print("Waiting for a connection...")
            conn, addr = server_socket.accept()
            # print(f"Connection established with {addr}") # debugging
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()
        except KeyboardInterrupt: # handling ctrl + C or ctrl + D
            print("Server shutting down gracefully.")
            break
        except Exception as e:
            print(f"Error accepting client: {e}")
            continue

    server_socket.close()
    print("Server closed.")

if __name__ == "__main__":
    start_server()