'''
The client sends periodical PING datagrams with a random content to a <server> and <port> specified in command line.
The server returns back (echoes) the same packets (content).
The client checks the content of the received packets to match what was sent and computes the round trip time and displays it to the user - for each sent packet.
'''
import socket
import sys
import time
import threading

start_time = 0

def send_ping(broadcast_socket, broadcast_address_tuple, message):
  global start_time
  while True:
      # Start computation of round trip time
      start_time = time.time()
      broadcast_socket.sendto(message.encode(), broadcast_address_tuple)
      print("Message sent to the broadcast address")
      time.sleep(5)  # Time delay between sends

def receive_ping(broadcast_socket):
  global start_time
  while True:
      # Receive the response
      response, address = broadcast_socket.recvfrom(1024)
      # End computation of round trip time
      end_time = time.time()
      # Compute the round trip time
      rtt = end_time - start_time  # Note we need to track rtt during each communication
      print(f"Received response from {address}: {response.decode()}")
      print(f"Round trip time: {rtt:.4f} seconds")

def main():
  if len(sys.argv) != 3:
      print(f"Usage: {sys.argv[0]} <Broadcast address> <Port>")
      sys.exit(1)

  # Extract broadcast address and port from arguments
  broadcast_address = sys.argv[1]
  broadcast_port = int(sys.argv[2])

  message = "Hello, Network!\0"

  try:
      # Create a datagram socket
      broadcast_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      # Enable the broadcast option on the socket
      broadcast_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
      # Prepare the broadcast address
      broadcast_address_tuple = (broadcast_address, broadcast_port)
      
      # Create threads for sending and receiving pings
      send_thread = threading.Thread(target=send_ping, args=(broadcast_socket, broadcast_address_tuple, message))
      recv_thread = threading.Thread(target=receive_ping, args=(broadcast_socket,))
      
      # Start threads
      send_thread.start()
      recv_thread.start()
      
      # Wait for both threads to complete
      send_thread.join()
      recv_thread.join()

  except socket.error as err:
      print(f"Socket error: {err}")
      sys.exit(1)
  finally:
      # Close the socket
      broadcast_socket.close()

if __name__ == "__main__":
  main()



