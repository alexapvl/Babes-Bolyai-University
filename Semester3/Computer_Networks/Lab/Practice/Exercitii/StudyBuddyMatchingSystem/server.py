import socket
import threading

# Server data structure
students = {}  # Key: (IP, port), Value: {learning_style, availability, subject}

def handle_client(conn, addr):
    global students
    try:
        # Receive student preferences
        data = conn.recv(1024).decode()
        if data == "MATCH_COMPLETE":
            # Remove the client from the active list
            if addr in students:
                del students[addr]
                print(f"Student {addr} has left after finding a match.")
            conn.close()
            return

        # Parse and store the preferences
        learning_style, availability, subject, udp_port = data.split(";")
        students[(addr[0], int(udp_port))] = {
            "learning_style": learning_style,
            "availability": eval(availability),
            "subject": subject,
        }

        # Send the list of active students back to the client (excluding itself)
        students_for_client = {
            k: v for k, v in students.items() if k != (addr[0], int(udp_port))
        }
        conn.sendall(str(students_for_client).encode())

        print(f"Student {addr} registered: {students[(addr[0], int(udp_port))]}")
    except Exception as e:
        print(f"Error with client {addr}: {e}")
    finally:
        conn.close()


def server():
    global students
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 12345))
    server_socket.listen(5)
    print("Server started, waiting for connections...")

    while True:
        conn, addr = server_socket.accept()
        threading.Thread(target=handle_client, args=(conn, addr)).start()


if __name__ == "__main__":
    server()
