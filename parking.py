"""
  We own a parking lot that can hold up to n cars at any given point in time. 
  Each slot is given a number starting at one increasing with increasing distance 
  from the entry point in steps of one. We want to create an mated ticketing 
  system that allows our customers to use our parking lot without human intervention.
"""

"""
 * Required:
    - Vehicle Registration numbers for all cars which are parked by the driver of a certain age,
    - Slot number in which a car with a given vehicle registration plate is parked. 
    - Slot numbers of all slots where cars of drivers of a particular age are parked.
"""

# Libraries 
import heapq

# Global Variables for multithreaded implementation
TotalSlots = -1
slots_being_unparked = set()

# Program variables
# Unordered map to store vehicle details
vehicles_by_age = {}
vehicle_by_registration_number = {}
vehicle_by_slot_number = {}


# Priority queue to store available slots - accending order
available_slots = []

# Class for parking lot
class Vehicle :

    # Parking slot number
    # - slot_number

    # Vehicle registration number
    # - vehicle_registration_number

    # Vehicle age
    # - driver_age

    # Constructor
    def __init__(self, slot_number, vehicle_registration_number, driver_age) :
        self.__slot_number = slot_number
        self.__registration_number = vehicle_registration_number
        self.__driver_age = driver_age
    

    # Getter for slot number
    def get_slot_number(self) :
        return self.__slot_number
    

    # Getter for vehicle registration number
    def  get_vehicle_registration_number(self) :
        return self.__registration_number

    # Getter for driver age
    def get_driver_age(self) :
        return self.__driver_age
        


def park(args) :

    # Get the vehicle registration number
    vehicle_registration_number = args[0]

    # Get the driver age
    driver_age = int(args[2])

    # Decreament the number of slots
    global TotalSlots
    TotalSlots-=1

    # Check if the slot is available
    if (TotalSlots < 0) :
        TotalSlots = len(available_slots)
        print("Sorry, parking lot is full")  
        return
    

    # Get the next available slot
    slot_number = heapq.heappop(available_slots)

    # Create a new vehicle object
    vehicle = Vehicle(slot_number, vehicle_registration_number, driver_age)

    # If the vehicle is already parked
    if (vehicle_registration_number in vehicle_by_registration_number) :
        print ("Sorry, vehicle is already parked")  
        return
    

    # Insert the vehicle in the registration number map
    vehicle_by_registration_number[vehicle_registration_number] = vehicle

    # Insert the vehicle in the parking lot
    if (driver_age not in vehicles_by_age) :
        vehicles_by_age[driver_age] = set()
    vehicles_by_age[driver_age].add(vehicle)

    # Insert the vehicle in the slot number map
    vehicle_by_slot_number[slot_number] = vehicle

    # Print the slot number
    print ( "Car with vehicle registration number \"{}\" has been parked at slot number {}".format(vehicle_registration_number, slot_number))

def create_parking_lot(args) :

    global TotalSlots,available_slots

    # Get the number of slots
    n = int(args[0])

    # Create a new parking lot
    available_slots = list(range(1, n+1))
    heapq.heapify(available_slots)

    # Print the messasge
    print ("Created parking of {} slots".format(n))
    
    # Set the global variable
    TotalSlots = n



def get_slot_numbers_by_age (args) :

    # Get the driver age
    age = int(args[0])

    # If there are no vehicles of a particular age
    if (age not in  vehicles_by_age or len(vehicles_by_age[age]) == 0) :
        # print "No cars are parked by the driver of age "  age  
        print("No cars are parked by the driver of age " + str(age) )
        return
    
    slots = set()
    # Print the slot numbers
    for  i in vehicles_by_age[age] :
        # Check if slot is not being unparked
        if (i not in slots_being_unparked) :
            slots.add(str(i.get_slot_number()))
    
    print(",".join(slots))

def get_slot_number_by_reg(args) :

    # Get the vehicle registration number
    vehicle_registration_number = args[0]

    # If the vehicle is not parked
    if ( vehicle_registration_number not in vehicle_by_registration_number) :
        print("Not found")
        return
    

    # Print the slot number
    print (vehicle_by_registration_number[vehicle_registration_number].get_slot_number())  



def get_vehicles_by_age(args):

    # Get the driver age
    age = int(args[0])

    # If there are no vehicles of a particular age
    if ( age not in vehicles_by_age or len(vehicles_by_age[age]) == 0) :
        # print "No cars are parked by the driver of age "  age  
        print("No cars are parked by the driver of age " +  str(age) )
        return

    regs = []

    # Print the vehicle numbers
    for i in vehicles_by_age[age] :
        if ( i.get_slot_number() not in slots_being_unparked) :
           regs.append(i.get_vehicle_registration_number())
        
    print(",".join(regs)) 



def unpark (args) :

    global TotalSlots
    # Get the slot number
    slot = int(args[0])

    # Add to slots being unpared
    slots_being_unparked.add(slot)

    # If the slot is not occupied
    if (slot not in vehicle_by_slot_number) :
        print ("Slot " + str(slot) +  " is empty")  
        return
    

    # Get the vehicle
    vehicle = vehicle_by_slot_number[slot]

    # Remove the vehicle from the registration number map
    del vehicle_by_registration_number[vehicle.get_vehicle_registration_number()]

    # Remove the vehicle from the parking lot
    vehicles_by_age[vehicle.get_driver_age()].remove(vehicle)

    # Remove the vehicle from the slot number map
    del vehicle_by_slot_number[slot]

    # Add the slot to the available slots
    heapq.heappush(available_slots,slot)

    # Increase the number of slots
    TotalSlots = len(available_slots)

    # Print the message
    print ("Slot number " + str(slot) + " vacated, the car with vehicle registration number \"" + str(vehicle.get_vehicle_registration_number()) + 
    "\" left the space, the driver of the car was of age " + str(vehicle.get_driver_age()))  

    # Delete the vehicle object
    del vehicle

    # Remove the slot from the slots being unparked
    slots_being_unparked.remove(slot)

# Main function
if __name__ == "__main__" :

    # Size of parking lot
    # - n 
    
    # Input
    # - age
    # -  reg_no,commd,temp

    # Commands
    command = {}
    
    # Command for parking lot creation
    command["Create_parking_lot"] = create_parking_lot
    # Command for parking a car
    command["Park"] = park

    # Command for slot numbers of a car with a given age
    command["Slot_numbers_for_driver_of_age"] = get_slot_numbers_by_age

    # Command for slot number of a car with a given registration number
    command["Slot_number_for_car_with_number"] = get_slot_number_by_reg

    # Command for vehicle registration number of a car with a given age
    command["Vehicle_registration_number_for_driver_of_age"] = get_vehicles_by_age

    # Command for unpark
    command["Leave"] = unpark

    # Read input .txt
    with open("input.txt", "r") as f :

        # Read the commads till the end of the file
        for line in f :

            # Split the line
            line = line.split()

            # Get the command
            command_name = line[0]

            # Check if the command is valid
            if (command_name not in command) :
                print ("Invalid command")
                continue

            # Get the arguments
            arguments = line[1:]

            # Call the command
            command[command_name](arguments)