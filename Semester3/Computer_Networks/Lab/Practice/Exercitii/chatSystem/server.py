import socket
import threading

# Global dictionary to store peer information
peers = {}

def handle_client(conn, addr):
    print(f"Connected to client: {addr}")
    try:
        data = conn.recv(1024).decode()
        print(f"Received from {addr}: {data}")

        if data.startswith("REGISTER"):
            udp_port, tcp_port = data.split()[1], data.split()[2]
            print(f"Registered client: {addr} with UDP:{udp_port} TCP:{tcp_port}")
            peers[(addr[0], udp_port, tcp_port)] = (addr[0], udp_port, tcp_port)
            conn.sendall(b"REGISTERED")
            send_client_list()  # Send updated client list to all clients
        elif data.startswith("UNREGISTER"):
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
    server_socket.bind(("0.0.0.0", 4000))  # Server listens on port 4000
    server_socket.listen(5)
    print("Server listening on port 4000")

    while True:
        try:
            print("Waiting for a connection...")
            conn, addr = server_socket.accept()
            print(f"Connection established with {addr}")
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()
        except KeyboardInterrupt:
            print("Server shutting down gracefully.")
            break
        except Exception as e:
            print(f"Error accepting client: {e}")
            continue

    server_socket.close()
    print("Server closed.")

if __name__ == "__main__":
    start_server()