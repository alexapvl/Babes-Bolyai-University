from random import randint

"""
Write an application that manages a list of circles.
Each circle has a unique center (x,y - ints) and a positive radius (int).
The application will have a menu-driven user interface and will provide the following features:

    1. Add a circle
        - adds the given circle to the list.
        - error if circle with given center already exists, the center
        or radius not given, empty or radius < 0

    2. Delete a circle
        - deletes the circle with the given center
        - error if non-existing center is given

    3. Show all circles
        - shows all circles in descending order of their radius

    4. Show circles that intersect a given one
        - select a circle from the list of existing circles
        - print those which intersect it
        (bonus: sort printed circles by descending order of radius)

    5. exit
        - exit the program

    Observations:
        - Add 10 random circles at program startup
        - Write specifications for non-UI functions
        - Each function does one thing only, and communicates via parameters and return value
        - The program reports errors to the user. It must also report errors from non-UI functions!
        - Make sure you understand the circle's representation
        - Try to reuse functions across functionalities (Less code to write and test)
        - Don't use global variables!
"""

"""
    Functions that have to do with circles
    - functions that know the representation fo a circle (list or dict)
    - no input/ no print
"""


# circle center (1,2) and radius 4
# {"x": 1, "y": 4, "radius": 4}

# V1 - dict representation

def create_circle(x: int, y: int, radius: int) -> list:
    """
    Create circle centered at (x,y) with given radius
    :param x: center X
    :param y: center Y
    :param radius: radius (int > 0)
    :return: The new circle
    """

    # What if the radius <= 0 => we can't create a circle
    if radius <= 0:
        # circle could not be created
        # V1 => return an error code
        # return None

        # V2 => raise an exception
        raise ValueError("Cannot create circle with non-positive radius")

    # everything is okay => return a dict
    # return {"x": x, "y": y, "radius": radius}
    return [x, y, radius]


def get_center(circle: dict) -> tuple:
    # returns a tuple
    # return circle["x"], circle["y"]
    return circle[0], circle[1]


def get_radius(circle) -> str:
    return circle[2]


def to_str(circle: dict) -> str:
    # get_* functions decouple to_str from the circle's representation
    x, y = get_center(circle)
    r = get_radius(circle)
    return "circle center (" + str(x) + "," + str(y) + "), radius = " + str(r)


"""
    Program functionalities go here
    - we don't care what a circle is represented like
    - no input/no print
    - add/delete circles etc.
"""


def generate_circles(n: int) -> list:
    """
    Generate n pseudo-random circles
    :param n: how many circles to create
    :return: the list of circles
    """
    result = []

    current_x = randint(1, 20)
    current_y = randint(1, 20)
    radius = randint(1, 10)
    result.append(create_circle(current_x, current_y, radius))

    while n > 1:
        current_x += randint(1, 20)
        current_y += randint(1, 20)
        radius = randint(1, 20)
        result.append(create_circle(current_x, current_y, radius))
        n -= 1

    return result


"""
    User interface
    - all input/print statements are here
"""

def read_circle():
    """
    Read circle from keyboard
    :return: New circle
    Raise ValueError if invalid input data was provided
    """
    x = int(input("center x = "))
    y = int(input("center y = "))
    radius = int(input("radius = "))
    return create_circle(x, y, radius)


def add_circle_ui(data: list):
    """
    Add a new circle to the list of circles
    :param data: The list of circles
    :return: None
    """
    circle = read_circle()
    # TODO check that circles have unique center
    data.append(circle)

def print_menu():
    print("1. Show all circles")
    print("2. Add a circle")
    print("0. Exit")

def start():
    # here we keep the list of circles in the program
    # NOT a global variable
    circles = generate_circles(10)

    #main program loop
    while True:
        print_menu()

        # we need to try ... except the line below to catch ValueError
        # option = int(input(">"))

        option = input("> ").strip()
        try:
            if option == "1":
                for c in circles:
                    # to_str() will become str() when it grows up
                    print(to_str(c))
            elif option == "2":
                # add a new circle
                add_circle_ui(circles)
                pass
            elif option == "0":
                break # stop the innermost loop that break is in
            else:
                print("Invalid input")
        except ValueError as ve:
            print(ve)

if __name__ == "__main__":
    """
    What happens below?
        1. create_circle() is called, raises an exception
        2. the assignment to circle variable does not take place, 
        circle variable remains undefined
        3. print(circle) is not called and control is transferred
        to the except block
        4. the code in the except block is executed
    """
    # try:
    #    circle = create_circle(1, 1, -5)
    #    print(circle)
    # except ValueError as ve:
    #    print(ve)

    #circles = generate_circles(10)
    #for c in circles:
    #    print(to_str(c))

    start()