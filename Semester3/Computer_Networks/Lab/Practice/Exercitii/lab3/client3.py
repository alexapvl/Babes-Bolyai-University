import pygame
import socket
import json

# Define shape types
LINE = "line"
CIRCLE = "circle"
RECTANGLE = "rectangle"
ELLIPSE = "ellipse"

# Client Class
class WhiteboardClient:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.canvas = []
        self.screen = None
        self.drawing = False
        self.current_shape = None

    def start(self):
        pygame.init()
        self.screen = pygame.display.set_mode((800, 600))
        pygame.display.set_caption("Collaborative Whiteboard")

        # Request the canvas from the server on startup
        self.request_canvas()

        # Main client loop
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.drawing = True
                    self.current_shape = [event.pos[0], event.pos[1], event.pos[0], event.pos[1]]  # x1, y1, x2, y2
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.drawing = False
                    self.send_drawing_event(LINE, *self.current_shape)
                    self.current_shape = None
                elif event.type == pygame.MOUSEMOTION and self.drawing:
                    self.current_shape[2], self.current_shape[3] = event.pos[0], event.pos[1]

            self.draw_canvas()
            pygame.display.flip()

        pygame.quit()

    def draw_canvas(self):
        self.screen.fill((255, 255, 255))  # Clear the screen to white
        for shape in self.canvas:
            if shape['shape_type'] == LINE:
                pygame.draw.line(self.screen, shape['contour_color'], (shape['x1'], shape['y1']),
                                 (shape['x2'], shape['y2']), 5)
            elif shape['shape_type'] == CIRCLE:
                pygame.draw.circle(self.screen, shape['contour_color'], (shape['x1'], shape['y1']),
                                   shape['x2'], 5)
            elif shape['shape_type'] == RECTANGLE:
                pygame.draw.rect(self.screen, shape['contour_color'], pygame.Rect(shape['x1'], shape['y1'],
                                                                                 shape['x2'], shape['y2']), 5)
            elif shape['shape_type'] == ELLIPSE:
                pygame.draw.ellipse(self.screen, shape['contour_color'], pygame.Rect(shape['x1'], shape['y1'],
                                                                                    shape['x2'], shape['y2']), 5)

    def send_drawing_event(self, shape_type, x1, y1, x2, y2):
        message = json.dumps({
            "shape_type": shape_type,
            "x1": x1,
            "y1": y1,
            "x2": x2,
            "y2": y2,
            "contour_color": (0, 0, 0),  # Black contour color
            "fill_color": None
        })
        self.client_socket.sendto(message.encode(), (self.host, self.port))

    def request_canvas(self):
        self.client_socket.sendto("GET_CANVAS".encode(), (self.host, self.port))

    def listen_for_updates(self):
        while True:
            data, _ = self.client_socket.recvfrom(1024)
            canvas_data = json.loads(data.decode('utf-8'))
            self.canvas = canvas_data

# Run the client
if __name__ == "__main__":
    client = WhiteboardClient()
    client.listen_for_updates()  # Start listening for updates in a separate thread
    client.start()
