/*
  We own a parking lot that can hold up to ‘n’ cars at any given point in time. 
  Each slot is given a number starting at one increasing with increasing distance 
  from the entry point in steps of one. We want to create an automated ticketing 
  system that allows our customers to use our parking lot without human intervention.
*
* 
*/

/*
 * Required:
    - Vehicle Registration numbers for all cars which are parked by the driver of a certain age,
    - Slot number in which a car with a given vehicle registration plate is parked. 
    - Slot numbers of all slots where cars of drivers of a particular age are parked.
*/


// Libraries
#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<set>
#include <fstream>
using namespace std;

// Global Variables for multithreaded implementation
int N;
unordered_set<int> slots_being_unparked;

// Class for parking lot
class Vehicle {
    private:

        // Parking slot number
        int slot_number;

        // Vehicle registration number
        string vehicle_registration_number;

        // Vehicle age
        int driver_age;

    public:
        // Constructor
        Vehicle(int slot_number, string vehicle_registration_number, int driver_age) {
            this->slot_number = slot_number;
            this->vehicle_registration_number = vehicle_registration_number;
            this->driver_age = driver_age;
        }

        // Getter for slot number
        int get_slot_number() {
            return slot_number;
        }

        // Getter for vehicle registration number
        string get_vehicle_registration_number() {
            return vehicle_registration_number;
        }

        // Getter for driver age
        int get_driver_age() {
            return driver_age;
        }

};

void park(string vehicle_registration_number, int driver_age, unordered_map<int, set<Vehicle*>> &vehicles_by_age,unordered_map<string, Vehicle*> &vehicle_by_registration_number,auto &available_slots,unordered_map<int, Vehicle*> &vehicle_by_slot_number) {
    
    // Decreament the number of slots
    N--;

    // Check if the slot is available
    if (N < 0) {
        N = available_slots.size();
        cout << "Sorry, parking lot is full" << endl;
        return;
    }

    // Get the next available slot
    int slot_number = available_slots.top();
    available_slots.pop();

    // Create a new vehicle object
    Vehicle *vehicle = new Vehicle(slot_number, vehicle_registration_number, driver_age);

    // If the vehicle is already parked
    if (vehicle_by_registration_number.find(vehicle_registration_number) != vehicle_by_registration_number.end()) {
        cout << "Sorry, vehicle is already parked" << endl;
        return;
    }

    // Insert the vehicle in the registration number map
    vehicle_by_registration_number[vehicle_registration_number] = vehicle;

    // Insert the vehicle in the parking lot
    vehicles_by_age[driver_age].insert(vehicle);

    // Insert the vehicle in the slot number map
    vehicle_by_slot_number[slot_number] = vehicle;

    // Print the slot number
    cout << "Car with vehicle registration number \"" << vehicle_registration_number << "\" " << "has been parked at slot number "<< slot_number << endl;

}

void create_parking_lot(int n,auto &available_slots) {

    // Create a new parking lot
    for (int i = 1; i <= n; i++) {
        available_slots.push(i);
    }

    // Print the messasge
    cout << "Created parking of " << n << " slots" << endl;

}

void get_slot_numbers_by_age(int age, unordered_map<int, set<Vehicle*>> &vehicles_by_age) {

    // If there are no vehicles of a particular age
    if (vehicles_by_age.find(age) == vehicles_by_age.end() || vehicles_by_age[age].empty()) {
        // cout << "No cars are parked by the driver of age " << age << endl;
        cout<<endl;;
        return;
    }

    // Print the first slot
    auto i = vehicles_by_age[age].begin();

    // Check if slot is not being unparked
    if (slots_being_unparked.find((*i)->get_slot_number()) == slots_being_unparked.end()) {
        cout << (*i)->get_slot_number();
    }
    i++;
    // Print the slot numbers
    for (; i != vehicles_by_age[age].end(); i++) {
        // Check if slot is not being unparked
        if (slots_being_unparked.find((*i)->get_slot_number()) == slots_being_unparked.end()) {
            cout << "," << (*i)->get_slot_number();
        }
    }
    cout << endl;

}

void get_slot_number_by_reg(string vehicle_registration_number, unordered_map<string, Vehicle*> &vehicle_by_registration_number) {

    // If the vehicle is not parked
    if (vehicle_by_registration_number.find(vehicle_registration_number) == vehicle_by_registration_number.end()) {
        // cout << "Not found" << endl;
        cout << endl;
        return;
    }

    // Print the slot number
    cout << vehicle_by_registration_number[vehicle_registration_number]->get_slot_number() << endl;

}

void get_vehicles_by_age(int age,unordered_map<int, set<Vehicle*>> &vehicles_by_age){

    // If there are no vehicles of a particular age
    if (vehicles_by_age.find(age) == vehicles_by_age.end() || vehicles_by_age[age].empty()) {
        // cout << "No cars are parked by the driver of age " << age << endl;
        cout << endl;
        return;
    }

    // Print the first vehile number
    auto i = vehicles_by_age[age].begin();
    if (slots_being_unparked.find((*i)->get_slot_number()) == slots_being_unparked.end()) {
        cout<<(*i)->get_vehicle_registration_number();
    }
    i++;

    // Print the vehicle numbers
    for (; i != vehicles_by_age[age].end(); i++) {
        if (slots_being_unparked.find((*i)->get_slot_number()) == slots_being_unparked.end()) {
            cout << "," << (*i)->get_vehicle_registration_number();
        }
    }
    cout << endl;

}

void unpark(int slot,unordered_map<int, Vehicle*> &vehicle_by_slot_number,unordered_map<string, Vehicle*> &vehicle_by_registration_number,auto &available_slots,unordered_map<int, set<Vehicle*>> &vehicles_by_age) {

    // Add to slots being unpared
    slots_being_unparked.insert(slot);

    // If the slot is not occupied
    if (vehicle_by_slot_number.find(slot) == vehicle_by_slot_number.end()) {
        cout << "Slot " << slot << " is empty" << endl;
        return;
    }

    // Get the vehicle
    Vehicle *vehicle = vehicle_by_slot_number[slot];

    // Remove the vehicle from the registration number map
    vehicle_by_registration_number.erase(vehicle->get_vehicle_registration_number());

    // Remove the vehicle from the parking lot
    vehicles_by_age[vehicle->get_driver_age()].erase(vehicle);

    // Remove the vehicle from the slot number map
    vehicle_by_slot_number.erase(slot);

    // Add the slot to the available slots
    available_slots.push(slot);

    // Increase the number of slots
    N = available_slots.size();

    // Print the message
    cout << "Slot number " << slot << " vacated, the car with vehicle registration number \"" << vehicle->get_vehicle_registration_number() << 
    "\" left the space, the driver of the car was of age "<< vehicle->get_driver_age() << endl;

    // Delete the vehicle
    delete vehicle;

}
// Main function
int main(){

    // Size of parking lot
    int n; 
    
    // Input
    int age;
    string reg_no,commd,temp;

    // Unordered map to store vehicle details
    unordered_map<int, set<Vehicle*>> vehicles_by_age;
    unordered_map<string, Vehicle*> vehicle_by_registration_number;
    unordered_map<int, Vehicle*> vehicle_by_slot_number;


    // Priority queue to store available slots - accending order
    priority_queue<int,vector<int>,greater<>> available_slots;

    // Commands
    unordered_map<string, int> command;
    
    // Command for parking lot creation
    command["Create_parking_lot"] = 1;
    // Command for parking a car
    command["Park"] = 2;

    // Command for slot numbers of a car with a given age
    command["Slot_numbers_for_driver_of_age"] = 3;

    // Command for slot number of a car with a given registration number
    command["Slot_number_for_car_with_number"] = 4;

    // Command for vehicle registration number of a car with a given age
    command["Vehicle_registration_number_for_driver_of_age"] = 5;

    // Command for unpark
    command["Leave"] = 6;

    // Read input .txt
    ifstream input("input.txt");

    // Read the commads till the end of the file
    while(input>>commd){
        int command_id = command[commd];
        switch (command_id)
        {
        case 1:
            input>>n;
            N = n;
            create_parking_lot(n,available_slots);
            break;

        case 2:
            input>>reg_no;
            // cout<<"\'"<<reg_no<<"\'"<<endl;
            input>>temp;
            input>>age;
            park(reg_no,age,vehicles_by_age,vehicle_by_registration_number,available_slots,vehicle_by_slot_number);
            break;

        case 3:
            input>>age;
            get_slot_numbers_by_age(age,vehicles_by_age);
            break;

        case 4:
            input>>reg_no;
            
            // trim the registration number
            // cout<<"\'"<<reg_no<<"\'"<<endl;
            get_slot_number_by_reg(reg_no,vehicle_by_registration_number);
            break;

        case 5:
            input>>age;
            get_vehicles_by_age(age,vehicles_by_age);
            break;

        case 6:
            input>>age;
            unpark(age,vehicle_by_slot_number,vehicle_by_registration_number,available_slots,vehicles_by_age);
            break;

        default:
            cout<<"Invalid command"<<endl;
            break;
        }
    }

}
