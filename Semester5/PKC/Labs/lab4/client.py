#!/usr/bin/env python3
"""
RSA Encrypted Chat Client
Connects to server, receives RSA keys, encrypts/decrypts messages
"""

import socket
import threading
import json
import sys
from rsa import encrypt, decrypt

# Server configuration - CHANGE THIS TO YOUR SERVER'S IP
SERVER_HOST = '127.0.0.1'  # Change to server's IP address
SERVER_PORT = 5555


class ChatClient:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = None
        self.n = None
        self.e = None
        self.d = None
        self.block_size = None
        self.my_address = None
        self.running = True
    
    def validate_plaintext(self, message):
        """
        Plaintext validation: Check that message contains only valid characters.
        Valid characters: A-Z (letters) and space.
        Returns: (is_valid, error_message or None)
        """
        invalid_chars = []
        for char in message:
            if char != ' ' and not ('A' <= char.upper() <= 'Z'):
                if char not in invalid_chars:
                    invalid_chars.append(char)
        
        if invalid_chars:
            return False, f"Invalid characters found: {invalid_chars}. Only letters A-Z and spaces are allowed."
        return True, None
    
    def validate_ciphertext(self, encrypted_blocks):
        """
        Ciphertext validation: Check that encrypted blocks are valid.
        - Must be a list of integers
        - Each block must be in range [0, n)
        Returns: (is_valid, error_message or None)
        """
        if not isinstance(encrypted_blocks, list):
            return False, "Ciphertext must be a list of numbers"
        
        if len(encrypted_blocks) == 0:
            return False, "Ciphertext is empty"
        
        for i, block in enumerate(encrypted_blocks):
            if not isinstance(block, int):
                return False, f"Block {i} is not an integer (got {type(block).__name__})"
            if block < 0:
                return False, f"Block {i} has negative value: {block}"
            if block >= self.n:
                return False, f"Block {i} value {block} exceeds modulus n={self.n}"
        
        return True, None
    
    def connect(self):
        """Connect to the server and receive RSA keys."""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            
            # Get our own address
            self.my_address = f"{self.socket.getsockname()[0]}:{self.socket.getsockname()[1]}"
            
            print(f"[*] Connected to server at {self.host}:{self.port}")
            print(f"[*] Your address: {self.my_address}")
            
            # Receive key information
            buffer = ""
            while "\n" not in buffer:
                data = self.socket.recv(4096).decode('utf-8')
                if not data:
                    raise ConnectionError("Server closed connection")
                buffer += data
            
            line, buffer = buffer.split("\n", 1)
            key_data = json.loads(line)
            
            if key_data.get("type") == "key":
                self.n = key_data["n"]
                self.e = key_data["e"]
                self.d = key_data["d"]
                self.block_size = key_data["block_size"]
                
                print(f"\n[KEY RECEIVED]")
                print(f"  n = {self.n}")
                print(f"  e = {self.e}")
                print(f"  d = {self.d}")
                print(f"  Block size: {self.block_size}")
                print()
                
                return True, buffer
            else:
                raise ValueError("Expected key data from server")
        
        except Exception as e:
            print(f"[!] Connection failed: {e}")
            return False, ""
    
    def receive_messages(self, initial_buffer=""):
        """Receive and decrypt messages from the server."""
        buffer = initial_buffer
        
        try:
            while self.running:
                try:
                    data = self.socket.recv(4096).decode('utf-8')
                    if not data:
                        print("\n[!] Server disconnected")
                        self.running = False
                        break
                    
                    buffer += data
                    
                    while "\n" in buffer:
                        line, buffer = buffer.split("\n", 1)
                        if line.strip():
                            try:
                                msg_data = json.loads(line)
                                if msg_data.get("type") == "message":
                                    sender = msg_data.get("sender", "Unknown")
                                    encrypted = msg_data.get("encrypted", [])
                                    
                                    # CIPHERTEXT VALIDATION
                                    valid, error = self.validate_ciphertext(encrypted)
                                    if not valid:
                                        print(f"\r[!] Ciphertext validation failed: {error}")
                                        print("You: ", end="", flush=True)
                                        continue
                                    
                                    # Decrypt the message
                                    try:
                                        decrypted = decrypt(encrypted, self.n, self.d, self.block_size)
                                        # Remove trailing underscores (padding)
                                        decrypted = decrypted.rstrip('_')
                                        
                                        # Determine if it's our own message
                                        if sender == self.my_address:
                                            print(f"\rMe: {decrypted}")
                                        else:
                                            print(f"\r{sender}: {decrypted}")
                                        
                                        # Reprint the input prompt
                                        print("You: ", end="", flush=True)
                                    
                                    except Exception as e:
                                        print(f"\r[!] Decryption error: {e}")
                                        print("You: ", end="", flush=True)
                            
                            except json.JSONDecodeError:
                                pass
                
                except socket.timeout:
                    continue
        
        except Exception as e:
            if self.running:
                print(f"\n[!] Receive error: {e}")
            self.running = False
    
    def send_message(self, message):
        """Encrypt and send a message to the server."""
        if not message.strip():
            return
        
        # PLAINTEXT VALIDATION
        valid, error = self.validate_plaintext(message)
        if not valid:
            print(f"[!] Plaintext validation failed: {error}")
            return
        
        # Normalize message for 27-letter alphabet (only letters and spaces)
        normalized = ""
        for char in message.upper():
            if char == ' ':
                normalized += '_'
            elif 'A' <= char <= 'Z':
                normalized += char
        
        if not normalized:
            print("[!] Message must contain letters or spaces only")
            return
        
        try:
            # Encrypt the message
            encrypted = encrypt(normalized, self.n, self.e, self.block_size)
            
            # Send to server
            msg_data = {
                "type": "message",
                "encrypted": encrypted
            }
            self.socket.send((json.dumps(msg_data) + "\n").encode('utf-8'))
        
        except Exception as e:
            print(f"[!] Send error: {e}")
    
    def run(self):
        """Main client loop."""
        success, initial_buffer = self.connect()
        if not success:
            return
        
        print("=" * 50)
        print("Chat started! Type your message and press Enter.")
        print("Only letters (A-Z) and spaces are supported.")
        print("Press Ctrl+C to quit.")
        print("=" * 50)
        print()
        
        # Start receive thread
        receive_thread = threading.Thread(
            target=self.receive_messages,
            args=(initial_buffer,),
            daemon=True
        )
        receive_thread.start()
        
        try:
            while self.running:
                print("You: ", end="", flush=True)
                message = input()
                if message.strip():
                    self.send_message(message)
        
        except KeyboardInterrupt:
            print("\n\n[*] Disconnecting...")
        
        except EOFError:
            print("\n\n[*] Disconnecting...")
        
        finally:
            self.running = False
            if self.socket:
                self.socket.close()


def main():
    # Allow specifying server IP as command line argument
    host = SERVER_HOST
    port = SERVER_PORT
    
    if len(sys.argv) >= 2:
        host = sys.argv[1]
    if len(sys.argv) >= 3:
        try:
            port = int(sys.argv[2])
        except ValueError:
            print(f"Invalid port: {sys.argv[2]}")
            sys.exit(1)
    
    print("=" * 50)
    print("RSA Encrypted Chat Client")
    print("=" * 50)
    print(f"\nConnecting to {host}:{port}...")
    
    client = ChatClient(host, port)
    client.run()


if __name__ == "__main__":
    main()

