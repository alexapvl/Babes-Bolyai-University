import socket
import threading
import sys

peers = {}

# Register the client with the server
def register_with_server(server_ip, server_port, udp_port, tcp_port):
    try:
        conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        conn.connect((server_ip, server_port))
        # Send registration message
        message = f"REGISTER {udp_port} {tcp_port}"
        conn.sendall(message.encode())
        print("Registered with server")
        conn.close()
    except Exception as e:
        print(f"Error registering with server: {e}")

def unregister_with_server(server_ip, server_port, udp_port, tcp_port):
    try:
        conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        conn.connect((server_ip, server_port))
        message = f"UNREGISTER {udp_port} {tcp_port}"
        conn.sendall(message.encode())
        print("Disconnected")
        conn.close()
    except Exception as e:
        print(f"Error disconecting from the chat")

# Listen for notifications (updated peer list) from the server via TCP
def listen_for_notifications(tcp_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("0.0.0.0", tcp_port))
    server_socket.listen(5)
    print(f"Listening for notifications on TCP:{tcp_port}")

    while True:
        conn, addr = server_socket.accept()
        data = conn.recv(1024).decode()
        print(f"RECIVED: {data}")
        if data.startswith("CLIENT_LIST"):
            update_peer_list(data.split("\n")[1:])
        conn.close()

def update_peer_list(peer_data):
    global peers
    peers = {}
    for line in peer_data:
        line = line.strip()  # Remove any leading/trailing whitespace
        if line:  # Check if line is not empty
            parts = line.split()
            if len(parts) == 3:  # Ensure there are exactly 3 parts (addr, udp, tcp)
                addr, udp, tcp = parts
                # Use (addr, udp) as the key to store each client uniquely
                peers[(addr, int(udp))] = (addr, int(udp), int(tcp))
            else:
                print(f"Skipping malformed line: {line}")
    print("Updated peer list:", peers)

# UDP communication with other peers
def udp_communication(udp_port):
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(("0.0.0.0", udp_port))
    print(f"Listening for UDP messages on port {udp_port}")

    while True:
        data, addr = udp_socket.recvfrom(1024)
        print(f"Received UDP message from {addr}: {data.decode()}")

def send_udp_message(message, udp_socket, own_udp_port):
    for (addr, udp_port), (ip, udp, tcp) in peers.items():
        # Exclude the client's own address and UDP port
        if udp_port == own_udp_port:
            continue  # Skip sending message to itself
        udp_socket.sendto(message.encode(), (ip, udp_port))


# Start the client
def start_client(server_ip, server_port, udp_port, tcp_port):


    # Start threads for UDP and notification handling
    threading.Thread(target=listen_for_notifications, args=(tcp_port,), daemon=True).start()
    threading.Thread(target=udp_communication, args=(udp_port,), daemon=True).start()

    # Register with the server
    register_with_server(server_ip, server_port, udp_port, tcp_port)

    # Send messages to peers
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    while True:
        message = input("Enter message to send to peers: ")
        if message == "disconnect":
            unregister_with_server(server_ip, server_port, udp_port, tcp_port)
            udp_socket.close()
            break
        else:
            send_udp_message(message, udp_socket, udp_port)

# Command-line argument parsing using sys.argv
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage example: python3 client.py <udp_port> <tcp_port>")
        sys.exit(1)

    server_ip = "127.0.0.1"
    server_port = 4000
    udp_port = int(sys.argv[1])  # Client's UDP port
    tcp_port = int(sys.argv[2])  # Client's TCP port

    # Start the client
    start_client(server_ip, server_port, udp_port, tcp_port)