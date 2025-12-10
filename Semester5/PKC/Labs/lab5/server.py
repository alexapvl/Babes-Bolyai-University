#!/usr/bin/env python3
"""
McEliece Encrypted Chat Server
Generates McEliece keys and broadcasts encrypted messages to all connected clients
"""

import socket
import threading
import json
from mceliece import generate_keys, McEliecePublicKey, McEliecePrivateKey

# Store all connected clients
clients = {}  # {client_socket: (address, thread)}
clients_lock = threading.Lock()


def generate_random_mceliece_keys():
    """
    Generate McEliece keys with educational parameters.
    
    Parameters chosen for educational purposes:
    - n = 31: Code length (codeword size in bits)
    - k = 16: Message dimension (can encode 16 bits = 3+ characters)
    - t = 3: Can correct up to 3 bit errors
    
    Returns: (public_key, private_key)
    """
    # Generate keys with default parameters
    public_key, private_key = generate_keys(n=31, k=16, t=3)
    
    return public_key, private_key


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
                if client_socket != sender_socket:  # Exclude sender
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
            "public_key": key_info["public_key"].to_dict(),
            "private_key": key_info["private_key"].to_dict()
        }
        client_socket.send((json.dumps(key_data) + "\n").encode('utf-8'))
        
        # Receive and broadcast messages
        buffer = ""
        while True:
            data = client_socket.recv(8192).decode('utf-8')
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
                            # Display the encrypted message on server (as hex)
                            hex_display = format_hex(encrypted)
                            print(f"\n[ENCRYPTED from {addr_str}]: {hex_display[:80]}...")
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


def format_hex(encrypted_blocks):
    """Convert encrypted blocks to hex string for display."""
    hex_parts = []
    for block in encrypted_blocks[:3]:  # Show first 3 blocks
        value = 0
        for bit in block:
            value = (value << 1) | bit
        hex_parts.append(format(value, 'x'))
    return ' '.join(hex_parts)


def main():
    # Server configuration
    HOST = '0.0.0.0'  # Listen on all interfaces
    PORT = 5556       # Different port from RSA server
    
    print("=" * 60)
    print("McEliece Encrypted Chat Server")
    print("=" * 60)
    
    # Generate McEliece keys
    print("\n[*] Generating McEliece keys...")
    public_key, private_key = generate_random_mceliece_keys()
    
    print(f"\n[KEY INFO]")
    print(f"  Code length (n): {public_key.n}")
    print(f"  Message dimension (k): {public_key.k}")
    print(f"  Error correction (t): {public_key.t}")
    print(f"\n  Public Key: {public_key.k}×{public_key.n} matrix (G')")
    print(f"  Private Key: Original code + scrambling matrices")
    
    key_info = {
        "public_key": public_key,
        "private_key": private_key
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

