import socket
import threading


def udp_listener(preferences, my_udp_socket, server_ip, server_port):
    print(f"Listening for matches on UDP port {my_udp_socket.getsockname()[1]}...")

    while True:
        try:
            # Receive data from other clients
            data, addr = my_udp_socket.recvfrom(1024)
            message = data.decode()

            # Check for a match confirmation message
            if message == "MATCH_CONFIRMED":
                print(f"Match confirmed by {addr}!")

                # Notify the server and exit
                notify_server_match_complete(server_ip, server_port)
                break

            # Evaluate peer preferences received
            peer_prefs = eval(message)
            if (
                preferences["learning_style"] == peer_prefs["learning_style"]
                and preferences["subject"] == peer_prefs["subject"]
                and any(x > 0 and y > 0 for x, y in zip(preferences["availability"], peer_prefs["availability"]))
            ):
                print(f"Match found with {addr}: {peer_prefs}")
                my_udp_socket.sendto("MATCH_CONFIRMED".encode(), addr)  # Send match confirmation

                # Notify the server and exit
                notify_server_match_complete(server_ip, server_port)
                break

        except Exception as e:
            print(f"Error in UDP listener: {e}")
            break


def notify_server_match_complete(server_ip, server_port):
    try:
        # Notify the server about the match completion
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
            server_socket.connect((server_ip, server_port))
            server_socket.sendall("MATCH_COMPLETE".encode())
            print("Notified the server of the match completion.")
    except Exception as e:
        print(f"Error notifying server: {e}")


def client():
    # Get user preferences
    learning_style = input("Enter learning style (a/v/h): ").strip()
    availability = list(map(int, input("Enter availability (7 numbers separated by spaces): ").strip().split()))
    subject = input("Enter subject: ").strip()

    preferences = {
        "learning_style": learning_style,
        "availability": availability,
        "subject": subject
    }

    # Set up a UDP socket for peer communication
    my_udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    my_udp_socket.bind(('0.0.0.0', 0))  # Bind to any available port
    my_udp_port = my_udp_socket.getsockname()[1]

    # Connect to the server via TCP
    server_ip = '192.168.1.4'  # Replace with the server's IP
    server_port = 12345        # Replace with the server's port

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server_socket.connect((server_ip, server_port))
        # Send preferences and UDP port to the server
        server_socket.sendall(
            f'{preferences["learning_style"]};{preferences["availability"]};{preferences["subject"]};{my_udp_port}'.encode()
        )

        # Receive the list of active students
        active_students = eval(server_socket.recv(1024).decode())
        print("Active students:", active_students)
    except Exception as e:
        print(f"Error connecting to server: {e}")
        return
    finally:
        server_socket.close()

    # Start the UDP listener in a separate thread
    threading.Thread(target=udp_listener, args=(preferences, my_udp_socket, server_ip, server_port)).start()

    # Broadcast preferences to other active students (excluding self)
    for student_addr, student_prefs in active_students.items():
        try:
            my_udp_socket.sendto(str(preferences).encode(), student_addr)
        except Exception as e:
            print(f"Error sending to {student_addr}: {e}")


if __name__ == "__main__":
    client()
