/*
* Programmer: Samuel Aguirre
* Purpose: Simulates a database of bikes
* Data: 4/13/2020
* Class: CSCI 222
*
*/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <string.h>
#include "Header.h"

using namespace std;

int main() {

	// declares three data structures, linked list
	Bike* generalList = 0;
	Bike* manuf = 0;
	Bike* id = 0;
	std::list<Bike*> generalList1;
	
	fstream bikeData("data.txt");
	//if the file is opened successfully opened read the first line of data from the file
	if (bikeData.is_open()) {
	    string line;
		int branchTo = 0;
		//while loops executes until the end of the file
		while (!bikeData.eof()) {

			getline(bikeData, line);
			branchTo = stoi(line);

			switch (branchTo) {
			case 1: 
				//creates a bike object from data in bikeData
				readBike(bikeData, generalList, manuf, id);
			    break;
			case 2:
		        //prints all bikes in no particular order
				printBikes(generalList, status::ALL, order::NO_ORDER);
				break;
			case 3:
				//prints the bikes by id
				printBikes(id, status::ALL, order::ID);
				break;
			case 4:
				//prints the bikes order by manuf
				printBikes(manuf, status::ALL, order::MANUF);
				break;
			case 5:
				//prints bikes available for rent
				printBikes(generalList, status::NOT_RENTED, order::NO_ORDER);
				break;
			case 6:
				//prints bikes not available for rent
				printBikes(generalList, status::RENTED, order::NO_ORDER);
				break;
			case 7:
				//performs a transaction
				do_transact(bikeData, generalList, status::NOT_RENTED);
				break;
			case 8:
				//returns the bike
				do_transact(bikeData, generalList, status::RENTED);
				break;
			case 9:
				//deletes a bike from the list
				del_id_bike(bikeData, id);
				break;
			case 10:
				//deletes a bike from the list
				del_manuf(bikeData, manuf);
				break;
			 }

		}

	}
	//displays en erroe message if the data file could not open
	else{
		cerr << "[ERROR - File Could Not Open]";
		return -1;
	}

	//deletes all bike objects 
	deleteBikes(generalList);

	//closes the file
	bikeData.close();
	return 0;
}
void readBike(fstream& bikeData, Bike*& generalList, Bike*& manuf, Bike*& id) {

	// creating a new bike node
	Bike* newBike = new Bike;
	
	if (!newBike) {
		cerr << "[ERROR - Not Enough Memory]";
	}
	else {
		// initializing the bike object
		init(newBike);
		if (bikeData.is_open()) {

			string line;
			int n = 1;
			while (n != 5) {

				getline(bikeData, line);
				switch (n) {
				case 1:

					//assgins the id_num field using the data from file
					newBike->id_num = stoi(line);
					break;
				case 2:
					
					//assgins the siz field using the data from file
					newBike->size = stoi(line);
					break;
				case 3:

					//assgins the cost_per_day field using the data from file
					newBike->cost_per_day = stof(line);
					break;
				case 4:

					//assgins the manufact field using the data from file
					strcpy_s(newBike->manufact, line.c_str());
					break;
				}
				
				++n;
			}
			// adds the new bike to the general list
			generalList = addBike(generalList, newBike);
			id = addBike(id, newBike, order::ID);
			manuf = addBike(manuf, newBike, order::MANUF,1);
		}
		else {
			cerr << "[ERROR - Could Not Open File]";
		}
	}

}
Bike* addBike(Bike* head, Bike* new_bike) {
	
	//if the list is empty insert node at the beginning of the list
	if (!head) {
		head = new_bike;
		return head;
	}
	else {
		new_bike->next = head;
		head = new_bike;
		return head;
	}

}
Bike* addBike(Bike* head_id, Bike* new_bike, order) {

	//inserts the node into the list if the list is empty
	if (!head_id) {
		new_bike->next_id = head_id;
		head_id = new_bike;
		return head_id;
	}
	//compares the id if the first node in the list, with the id of new_bike
	else if (new_bike->id_num > head_id->id_num) {

		new_bike->next_id = head_id;
		head_id = new_bike;
		return head_id;
	}
	//else search for the place in the list to insert node 
	else {
		Bike* temp = head_id;
		Bike* prev = head_id;
		while (temp && new_bike->id_num < temp->id_num) {
			prev = temp;
			temp = temp->next_id;
		}
		//if we traversed through the whole list we insert at the end of the list
		if (!temp) {
			prev->next_id = new_bike;
			return head_id;
		}
		//if we are here that means we will insert node within the list
		else {

			new_bike->next_id = temp;
			prev->next_id = new_bike;

			return head_id;
		}

	}
	
}
Bike* addBike(Bike* head_manuf, Bike* new_bike, order, int) {

	//if list is empty, insert node at the beginning of the list
	if (!head_manuf) {
		new_bike->next_manuf = head_manuf;
		head_manuf = new_bike;
		return head_manuf;
	}
	//else, compare the head manuf with newBike manuf, if ture insert newBike at the start of the list
	else if (strcmp(new_bike->manufact, head_manuf->manufact) < 0) {

		new_bike->next_manuf = head_manuf;
		head_manuf = new_bike;
		return head_manuf;
	}
	//else search for the place in the list to insert node 
	else {
		Bike* temp = head_manuf;
		Bike* prev = head_manuf;
		while (temp && strcmp(new_bike->manufact,temp->manufact) > 0) {

			prev = temp;
			temp = temp->next_manuf;
		}
		//if we traversed through the whole list we insert at the end of the list
 		if (!temp) {
			prev->next_manuf = new_bike;
			return head_manuf;
		}
		//if we are here that means we will insert node within the list
		else {
			new_bike->next_manuf = temp;
			prev->next_manuf = new_bike;

			return head_manuf;
		}
	}

}
void printBikes(Bike* head,status print1,order print2) {

	//prints all bikes in no particular order
	if (print1 == status::ALL && print2 == order::NO_ORDER) {
		cout << "[ALL BIKES]:\n\n";
		cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
		cout << "----------------------------------------------\n";
		Bike* temp = head;
		//while we havent reached the end of the list
		while (temp) {
			//prints a bike that has not been deleted, in no particular order
			if (!temp->deleted) {
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n";
				temp = temp->next;
			}
			else {
				temp = temp->next;
			}
		
		}
		cout << "\n";
	}
	//
	else if (print1 == status::ALL && print2 == order::ID) {
		cout << "[BIKES ORDERED BY ID]:\n\n";
		cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
		cout << "----------------------------------------------\n";
		Bike* temp = head;
		while (temp) {
			//prints a bike that has not been deleted, in order by id
			if (!temp->deleted) {
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n";
				temp = temp->next_id;
			}
			else {
				temp = temp->next_id;
			}

		}
		cout << "\n";
	}
	//
	else if (print1 == status::ALL && print2 == order::MANUF) {
		cout << "[BIKES ORDERED BY MANUF]:\n\n";
		cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
		cout << "----------------------------------------------\n";
		Bike* temp = head;
		while (temp) {
			//prints a bike that has not been deleted, ordered by manuf
			if (!temp->deleted) {
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n";
				temp = temp->next_manuf;
			}
			else {
				temp = temp->next_manuf;
			}

		}
		cout << "\n";
	}
	else if (print1 == status::NOT_RENTED) {
		cout << "[BIKES AVALIABLE TO RENT]:\n\n";
		cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
		cout << "----------------------------------------------\n";
		Bike* temp = head;
		while (temp) {
			//prints a bike for rent
			if (temp->rented_code == status::NOT_RENTED) {
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n";
				temp = temp->next;
			}
			else {
				temp = temp->next;
			}
		}
		cout << "\n";
	}
	else if (print1 == status::RENTED) {
		cout << "[BIKES NOT AVALIABLE TO RENT]:\n\n";
		cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
		cout << "----------------------------------------------\n";
		Bike* temp = head;
		while (temp) {
			//prints a bike being rented
			if (temp->rented_code == status::RENTED) {
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n";
				temp = temp->next;
			}
			else {
				temp = temp->next;
			}
		}
		cout << "\n";

	}

}
void do_transact(fstream& bikeData, Bike* head, status print1) {

	Bike* temp = head;

	if (bikeData.is_open()) {
		if (print1 == status::NOT_RENTED) {
			string line;
			// grabs the ID from file
			getline(bikeData, line);
			// loop will iterate while not end of list or ID of bike not found
			while (temp) {
				// if bike is found in list by id and bike is not rented execute conditional
				if (temp->id_num == stoi(line) && temp->rented_code == status::NOT_RENTED) {
					//set status as rented
					temp->rented_code = status::RENTED;

					cout << "[TRANSACTION]:\n\n";
					cout << setw(9) << "[ID]" << setw(14) << "[Days]" << setw(14) << "[COST]" << setw(10) << "[RENTER]\n";
					cout << "----------------------------------------------\n";


					cout << "  " << setw(7) << line;
					getline(bikeData, line);

					//prints out number of days and cost for rental
					cout << "  " << setw(12) << line << "  " << setw(12) << stoi(line) * temp->cost_per_day << setw(2);

					//grabs the last piece of data for transaction and then prints of the size of bike
					getline(bikeData, line);
					//assigns to whom the bike is rented to in the struct
					strcpy_s(temp->to_whom, line.c_str());
					cout << " " << line;
					cout << "\n\n";

					// return after the transaction has been processed
					return;
				}
				else if (temp->id_num == stoi(line) && temp->rented_code == status::RENTED) {

					cout << "[The Current Bike Is Rented]";
					return;
				}
				temp = temp->next;
			}
		}
		else if(print1 == status::RENTED){

			string line;
			// grabs the ID from file
			getline(bikeData, line);
			// loop will iterate while not end of list or ID of bike not found
			while (temp) {
				// if bike is found in list by id and bike is not rented execute conditional
				if (temp->id_num == stoi(line) && temp->rented_code == status::RENTED) {
					
					//sets the rented_code to NOT_RENTED
					temp->rented_code = status::NOT_RENTED;

					cout << "[RETURING BIKE]:\n\n";
					cout << setw(9) << "[ID]" << setw(14) << "[Days]" << setw(14) << "[COST]" << setw(10) << "[RENTER]\n";
					cout << "----------------------------------------------\n";

					cout << "  " << setw(7) << line;
					getline(bikeData, line);

					//prints out number of days and cost for rental
					cout << "  " << setw(12) << line << "  " << setw(12) << stoi(line) * temp->cost_per_day << setw(2) 
						<< " " << temp->to_whom << setw(3);
					cout << "\n\n";

					//return after the bike has been retuned
					return;
				}
				else if (temp->id_num == stoi(line) && temp->rented_code == status::NOT_RENTED) {

					cout << "[SORRY - BIKE HAS BEEN RETURNED]:\n\n";
					cout << setw(9) << "[ID]" << setw(14) << "[Days]" << setw(14) << "[COST]" << setw(10) << "[RENTER]\n";
					cout << "----------------------------------------------\n";

					cout << "  " << setw(7) << line;
					getline(bikeData, line);

					//prints out number of days and cost for rental
					cout << "  " << setw(12) << line << "  " << setw(12) << stoi(line) * temp->cost_per_day << setw(2)
						<< " " << temp->to_whom << setw(3);
					cout << "\n\n";
					return;
				}
				temp = temp->next;
			}
			
		}
	}
	else {
		cerr << "[ERROR - Could Not Open File]";
	}

}
void del_id_bike(fstream& bikeData, Bike* id) {

	Bike* temp = id;

	if (bikeData.is_open()) {
		string line;
		getline(bikeData, line);
		int searchID = stoi(line);

		//while not the end of the list
		while (temp) {

			//deletes the desired bike by id and returns
			if (temp->id_num == searchID) {
				cout << "[DELETING BIKE]:\n\n";
				cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
				cout << "----------------------------------------------\n";
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n\n";
				temp->deleted = true;
				return;
			}
			temp = temp->next_id;
		}
	}
	else {
		cerr << "[ERROR - Could not Open File]"; 
		return;
	}
}
void del_manuf(fstream& bikeData, Bike* manuf) {

	Bike* temp = manuf;

	if (bikeData.is_open()) {
		string line;
		getline(bikeData, line);

		//while not the end of the list
		while (temp) {

			//deletes the manufact by setting its flag to true
			if (!strcmp(temp->manufact,line.c_str())) {
				cout << "[DELETING BIKE]:\n\n";
				cout << setw(9) << "[ID]" << setw(14) << "[BIKE]" << setw(14) << "[COST]" << setw(10) << "[SIZE]\n";
				cout << "----------------------------------------------\n";
				cout << "  " << setw(7) << temp->id_num << setw(7) << "  " << setw(7) << temp->manufact;
				cout << setw(7) << "  " << setw(7) << temp->cost_per_day << setw(7) << "  " << temp->size;
				cout << "\n\n";
				temp->deleted = true;
				return;
			}
			temp = temp->next_manuf;
		}
	}
	else {
		cerr << "[ERROR - Could not Open File]";
		return;
	}
}
void deleteBikes(Bike* head) {

	Bike* prev = head;
	Bike* temp = head;

	//deletes all nodes in the list
	while (temp) {

		prev = temp;
		temp = temp->next;
		delete prev;
	}
	
}
void init(Bike* newBike) {

	
	strcpy_s(newBike->manufact, "");
	newBike->id_num = 0;
	newBike->size = 0;
	newBike->cost_per_day = 0.0;
	newBike->rented_code = status::NOT_RENTED;
	newBike->deleted = false;
	strcpy_s(newBike->to_whom, "");


	// sets the next pointer to 0
	newBike->next_id = 0;
	newBike->next = 0;
	newBike->next_manuf = 0;
}
