from operator import itemgetter

def reroute(data: list, curr_dest: str, new_dest: str) -> list:
    for flight in data:
        if flight["destination"] == curr_dest:
            flight["destination"] = new_dest
    return data

def valid_code(data: list, code: int) -> bool:
    for flight in data:
        if flight["code"] == code:
            return False
    return True

def create_flight(code: str, duration: str, departure: str, destination: str) -> dict:
    return {"code": code, "duration": duration, "departure": departure, "destination": destination}
    
    

def to_str(flight: dict) -> str:
    return "Code: " + str(flight["code"]) + " | Duration: " + str(flight["duration"]) + " | Departure city: " + str(flight["departure"]) + " | Destination city: " + str(flight["destination"])

def print_menu():
    print("1. Add a flight")
    print("2. Modify the duration of a given flight")
    print("3. Reroute flight for a given detination")
    print("4. Show all flights with a given departure city")
    print("0. Exit")
    
# code, duration, departure city, destination city

def main():
    flights=[]
    flights.append({"code": "00001", "duration": 100, "departure": "Cluj-Napoca", "destination": "London"})
    flights.append({"code": "00002", "duration": 50, "departure": "Bucuresti", "destination": "Atena"})
    flights.append({"code": "00003", "duration": 30, "departure": "Iasi", "destination": "Berlin"})
    flights.append({"code": "00004", "duration": 95, "departure": "Cluj-Napoca", "destination": "Praga"})
    flights.append({"code": "00005", "duration": 70, "departure": "Targu-Mures", "destination": "Praga"})
    while True:
        print_menu()
        option: str = input(">>>")
        if option == "1":
            while True:
                try:
                    print("Add a new flight")
                    code: str = input("Code: ")
                    if valid_code(flights, code): # Checks if the code provided is unique        
                        duration = int(input("Duration: "))
                        departure: str = input("Departure city: ")
                        destination: str = input("Destination city: ")
                        if len(code) < 3 or duration < 20 or len(departure) < 3 or len(destination) < 3:
                            raise ValueError("Invalid input. Code, destination and departure need to be at least 3 characters long and duration greater than 20.")
                    else:
                        raise ValueError("Code provided for flight already exists. Try again")
                        flights.append(create_flight(code, duration, departure, destination))
                except ValueError as ve:
                    print(ve)
        elif option == "2":
            while True:
                found: bool = False
                print("Modify a given flight by code")
                code: str = input("Code of flight is: ")
                for flight in flights:
                    if flight["code"] == code:
                        new_duration = int(input(f"Enter new duration for flight {code}: "))
                        flight["duration"] = new_duration
                        found = True
                if found:
                    break
                else:
                    print("ERROR: Flight with given code not found. Try again.")
        elif option == "3":
            while True:
                try:
                    print("Reroute all flights for a given destination")
                    curr_dest: str = input("Destination: ")
                    new_dest: str = input("New Destination: ")
                    if len(new_dest) < 3:
                        raise ValueError("Length less than 3 characters. Try again")
                    flights = reroute(flights, curr_dest, new_dest)
                    print("Flights have been rerouted with success.")
                    break
                except ValueError as ve:
                    print(ve)
        elif option == "4":
            print("Show all flights given by departure city")
            departure_show: str = input("Departure city: ")
            show_list = []
            for flight in flights:
                if flight["departure"] == departure_show:
                    show_list.append(flight)
            
            show_list = sorted(show_list, key = itemgetter("duration"))
            for flight in show_list:
                print(to_str(flight))
            if len(show_list) == 0:
                print("No flights with given destination were found.")
        elif option == "0":
            print("Program exited.")
            break
        else:
            print("Invalid option, try again.")
  
main()
