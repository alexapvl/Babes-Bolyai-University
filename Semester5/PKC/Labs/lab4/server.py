#!/usr/bin/env python3
"""
RSA Encrypted Chat Server
Generates RSA keys and broadcasts encrypted messages to all connected clients
"""

import socket
import threading
import random
import json
from rsa import generate_keys, calculate_block_size

# First 30 prime numbers for fast key generation
FIRST_30_PRIMES = [
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113
]

# Store all connected clients
clients = {}  # {client_socket: (address, thread)}
clients_lock = threading.Lock()


def generate_random_rsa_keys():
    """
    Generate RSA keys by randomly selecting 2 different primes from the first 30 primes.
    Returns: (n, e, d, block_size, p, q)
    """
    # Pick two different random primes
    p, q = random.sample(FIRST_30_PRIMES, 2)
    
    # Compute n
    n = p * q
    
    # Generate keys
    e, d, phi, block_size = generate_keys(n, p, q)
    
    return n, e, d, block_size, p, q


def broadcast_message(encrypted_msg, sender_addr, sender_socket):
    """
    Broadcast an encrypted message to all connected clients.
    Include the sender's address so clients know who sent it.
    """
    message_data = {
        "type": "message",
        "sender": sender_addr,
        "encrypted": encrypted_msg
    }
    message_json = json.dumps(message_data) + "\n"
    
    with clients_lock:
        disconnected = []
        for client_socket, (addr, _) in clients.items():
            try:
              if client_socket != sender_socket: # exclude the sender socket when sending a message
                client_socket.send(message_json.encode('utf-8'))
            except Exception as e:
                print(f"[!] Failed to send to {addr}: {e}")
                disconnected.append(client_socket)
        
        # Remove disconnected clients
        for sock in disconnected:
            if sock in clients:
                del clients[sock]


def handle_client(client_socket, client_addr, key_info):
    """
    Handle communication with a connected client.
    """
    addr_str = f"{client_addr[0]}:{client_addr[1]}"
    print(f"\n[+] Client connected: {addr_str}")
    
    try:
        # Send key information to the client
        key_data = {
            "type": "key",
            "n": key_info["n"],
            "e": key_info["e"],
            "d": key_info["d"],
            "block_size": key_info["block_size"]
        }
        client_socket.send((json.dumps(key_data) + "\n").encode('utf-8'))
        
        # Receive and broadcast messages
        buffer = ""
        while True:
            data = client_socket.recv(4096).decode('utf-8')
            if not data:
                break
            
            buffer += data
            while "\n" in buffer:
                line, buffer = buffer.split("\n", 1)
                if line.strip():
                    try:
                        msg_data = json.loads(line)
                        if msg_data.get("type") == "message":
                            encrypted = msg_data.get("encrypted", [])
                            # Display the encrypted message on server
                            print(f"\n[ENCRYPTED from {addr_str}]: {encrypted}")
                            # Broadcast to all clients
                            broadcast_message(encrypted, addr_str, client_socket)
                    except json.JSONDecodeError:
                        pass
    
    except Exception as e:
        print(f"\n[!] Error with client {addr_str}: {e}")
    
    finally:
        with clients_lock:
            if client_socket in clients:
                del clients[client_socket]
        client_socket.close()
        print(f"\n[-] Client disconnected: {addr_str}")


def main():
    # Server configuration
    HOST = '0.0.0.0'  # Listen on all interfaces
    PORT = 5555
    
    print("=" * 60)
    print("RSA Encrypted Chat Server")
    print("=" * 60)
    
    # Generate RSA keys
    print("\n[*] Generating RSA keys...")
    n, e, d, block_size, p, q = generate_random_rsa_keys()
    
    print(f"\n[KEY INFO]")
    print(f"  Primes selected: p={p}, q={q}")
    print(f"  n = p × q = {n}")
    print(f"  Public exponent (e): {e}")
    print(f"  Private exponent (d): {d}")
    print(f"  Block size: {block_size}")
    print(f"\n  Public Key: ({n}, {e})")
    print(f"  Private Key: ({n}, {d})")
    
    key_info = {
        "n": n,
        "e": e,
        "d": d,
        "block_size": block_size
    }
    
    # Create server socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        server_socket.bind((HOST, PORT))
        server_socket.listen(5)
        
        # Get the server's IP address
        hostname = socket.gethostname()
        local_ip = socket.gethostbyname(hostname)
        
        print(f"\n[*] Server started on {HOST}:{PORT}")
        print(f"[*] Clients can connect using: {local_ip}:{PORT}")
        print("\n" + "=" * 60)
        print("Waiting for connections... (Encrypted messages will appear here)")
        print("=" * 60)
        
        while True:
            client_socket, client_addr = server_socket.accept()
            
            # Start a thread for this client
            client_thread = threading.Thread(
                target=handle_client,
                args=(client_socket, client_addr, key_info),
                daemon=True
            )
            
            with clients_lock:
                clients[client_socket] = (f"{client_addr[0]}:{client_addr[1]}", client_thread)
            
            client_thread.start()
    
    except KeyboardInterrupt:
        print("\n\n[*] Server shutting down...")
    
    finally:
        server_socket.close()


if __name__ == "__main__":
    main()

