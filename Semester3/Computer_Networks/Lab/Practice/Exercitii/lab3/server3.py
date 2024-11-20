import socket
import json
import threading

# Define shapes
class Shape:
    def __init__(self, shape_type, x1, y1, x2, y2, contour_color, fill_color=None):
        self.shape_type = shape_type
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.contour_color = contour_color
        self.fill_color = fill_color

    def to_dict(self):
        return {
            "shape_type": self.shape_type,
            "x1": self.x1,
            "y1": self.y1,
            "x2": self.x2,
            "y2": self.y2,
            "contour_color": self.contour_color,
            "fill_color": self.fill_color
        }

# Server Class
class WhiteboardServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.canvas = []
        self.clients = set()
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_socket.bind((self.host, self.port))

    def add_client(self, addr):
        self.clients.add(addr)

    def remove_client(self, addr):
        if addr in self.clients:
            self.clients.remove(addr)

    def broadcast(self, message):
        for client in self.clients:
            self.server_socket.sendto(message.encode(), client)

    def handle_client(self):
        while True:
            data, addr = self.server_socket.recvfrom(1024)
            self.add_client(addr)

            message = data.decode('utf-8')
            if message == "GET_CANVAS":
                self.send_canvas(addr)
            else:
                self.process_drawing(message, addr)

    def process_drawing(self, message, addr):
        drawing = json.loads(message)
        shape = Shape(
            shape_type=drawing['shape_type'],
            x1=drawing['x1'],
            y1=drawing['y1'],
            x2=drawing['x2'],
            y2=drawing['y2'],
            contour_color=drawing['contour_color'],
            fill_color=drawing.get('fill_color')
        )

        self.canvas.append(shape)
        self.broadcast(message)

    def send_canvas(self, addr):
        canvas_data = json.dumps([shape.to_dict() for shape in self.canvas])
        self.server_socket.sendto(canvas_data.encode(), addr)

    def start(self):
        print(f"Server started on {self.host}:{self.port}")
        threading.Thread(target=self.handle_client, daemon=True).start()

# Run the server
if __name__ == "__main__":
    server = WhiteboardServer()
    server.start()

    while True:
        pass
