//
//  main.cpp
//  hotel system
//
//  Created by Andrea d Cazares on 10/1/24.
//

#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

//globals section
const int MAX_GUESTS = 20;//max capacity
const int MAX_ROOMS_AVAIL = 20; //max rooms available
const float BASE_PRICE = 200.00;//cost of hotel room on a normal day to day

//prototypes section
void menuDisplay();
void checkIn(string guests[], int& guestCount, bool rooms[],int roomNumbers[], int nightsStaying[],string currentMonth);
void checkOut (string guests[], int& guestCount, bool rooms[],int roomNumbers[],int nightsStaying[]);
void displayGuestInfo(string guests[], int guestCount, int roomNumbers[], int nightsStaying[]);
void displayRoomsAvail(bool rooms[]);

string randMonth();
float priceAdjust(int guestCount);
float seasonalAdj(float price,string month);
long long getRandConf();


int main(){
    srand(static_cast<unsigned int>(time(0)));
 
    int menuChoice;
   

    bool rooms[MAX_ROOMS_AVAIL] = {false};//create a array and check if rooms are available
    int roomNumbers[MAX_GUESTS] = {0}; // to store room numbers for each guest
    string guests [MAX_GUESTS]; //contains guest name
    int nightsStaying[MAX_GUESTS] = {0}; // store nights staying for each guest
    int guestCount = 0;
   

    string currentMonth = randMonth();
    cout << "we are currently in: "<< currentMonth<<endl; //display a random month so that we might do a seasonaldiscount

    do{
        cout << " please choose a menu option\n ";
        menuDisplay(); //displays menu to user
        cin >> menuChoice;

    
        switch (menuChoice){
            case 1: checkIn(guests, guestCount, rooms,roomNumbers, nightsStaying,currentMonth);
            break;
            
            case 2: checkOut(guests, guestCount, rooms,roomNumbers,nightsStaying);
            break;

            case 3: displayGuestInfo(guests, guestCount, roomNumbers, nightsStaying);
            break;

            case 4: displayRoomsAvail(rooms);
            break;
            
            default :
                cout<< "Error,Invalid Entry. Try Again\n";
                break;
        }
    }
    while (menuChoice !=5);
    
    return 0;
}


void menuDisplay(){ // so that a menu will pop up and show avail rooms and room representation
    cout <<" ~~~~~~~~~~ "<< "Hotel Check in"<<" ~~~~~~~~~~ "<< endl;
    cout << " 1. Check A guest In\n" ;
    cout << " 2. Check A Guest Out\n ";
    cout << " 3. Guest Info\n" ;
    cout << " 4. Rooms Available\n ";
    cout << " 5. Exit system \n";

}
// guest check in
void checkIn(string guests[], int& guestCount, bool rooms[], int roomNumbers[],int nightsStaying[],string currentMonth) {
        int roomNum;
        string guestName;
        int nights;
        float roomPrice= BASE_PRICE;
        string couponCode;
    
   
      
        if (guestCount >= MAX_GUESTS) { //checks if hotel has any capacity
            cout << " hotel is at max capacity. " << endl;
            return;
        } else {
            cout << " enter guest name\n ";
            cin.ignore();
            getline (cin,guestName);
          
            do {
                cout << " Enter a room number 1- "<< MAX_ROOMS_AVAIL<<endl;// enter and store in roomNum
                cin>> roomNum;
                
            
                if (roomNum < 1 || roomNum > MAX_ROOMS_AVAIL) {
                        cout << " Invalid Number, try again.\n ";
                }
                else if (rooms[roomNum - 1]) {  //this section will pop a error if room entered is in use
                        cout << " Room is Already occupied, choose a different room.\n ";
                }
            }
            while
            (roomNum < 1 || roomNum > MAX_ROOMS_AVAIL || rooms[roomNum - 1]);
            //rm num is ! < 1 ,   not greater than max amount of rooms avail, checks if occupied( true= occupued)
            cout << " Enter The Amount Of Nights The Guest Will Stay " << endl;
            cin >> nights;

            cout << "Do you have a coupon code? Enter code or 'no': ";
            cin >> couponCode;
            
            //do price adjustment
            roomPrice = priceAdjust(guestCount); //occ adjust
            roomPrice = seasonalAdj(roomPrice, currentMonth);//season adjust
          
           // helps Store guest information
            guests[guestCount] = guestName;
            rooms[roomNum - 1] = true;
            roomNumbers[guestCount] = roomNum;
            nightsStaying[guestCount] = nights;

            //using the coupon code
            if (couponCode == "SAVE50"||couponCode == "save50"){
                roomPrice -= 50; // Apply $50 discount directly to roomPrice
                
                cout << "your $50 coupon has been applied\n";
            }
            else if(couponCode != "no" && couponCode != "No") {
                cout<< "invalid coupon try a different code or say No\n";//
            }

            cout << "Guest " << guestName << " checked into room " << roomNum << " for " << nights << " nights." << endl;
            cout << " confirmation number is: " << getRandConf() << endl;//rand conf function call
            cout<< "room price per night is: $"<< roomPrice<<endl;
            cout<< "total stay cost is: $"<< (roomPrice*nights)<<endl;
            
            cout << "Base room price: $" << BASE_PRICE << endl;
            cout << "Price after occupancy adjustment: $" << roomPrice << endl;
            cout << "Price after seasonal adjustment for " << currentMonth << ": $" << roomPrice << endl;

          guestCount++;
    }
    
    
}
//goal make a check out syatem that allows you to also input the amount that is due for the stay
void checkOut (string guests[], int& guestCount, bool rooms[],int roomNumbers[],int nightsStaying[]){
    int roomNumb;
    cout << " Enter The Checking Out Guest's Room Number: \n";
    cin >> roomNumb;

    for (int i= 0; i<guestCount; i++){ //will iteterate through all current guests
        if (roomNumbers[i] == roomNumb){ //if guests room num matches the entered #
           cout<< guests[i]<< " is checked out\n";
           rooms[roomNumb - 1] = false; //allows vacancy and other can check in
            

            // nested loop to remove guest from arrays by shifting remaining guests
            for (int j = i; j < guestCount - 1; j++) {
                guests[j] = guests[j + 1];
                roomNumbers[j] = roomNumbers[j + 1];
            }
            guestCount--;//decrements total guest since removing one
            return;

        }
    }
    cout<< "room already vacant/guest not found\n";
}

void displayGuestInfo(string guests[], int guestCount, int roomNumbers[], int nightsStaying[]) {
    cout << "Guest Information:" << endl;
    cout << setw(15) << left << "Name" << setw(15) << "Room Number" << setw(15) << "Nights" << endl;
    for (int i = 0; i < guestCount; i++) { //will iterate through all current guest
        cout << setw(15) << left << guests[i]  << setw(15) << roomNumbers[i] << setw(15) << nightsStaying[i] << endl;
    }
}

void displayRoomsAvail(bool rooms[]){
    cout<< "rooms available are: \n";

    for (int i = 0; i < MAX_ROOMS_AVAIL; i++) {
        cout << "Room " << (i + 1) << ": ";
        if (rooms[i] == true) {
            cout << "Occupied";
        } else {
            cout << "Available";
        }
        cout << endl;
    }
}
float priceAdjust(int guestCount){// tried roomsOccupied in parameter
    float changedPrice= BASE_PRICE;// assign 200to changedPrice since cant change const
    float occupancyRate = static_cast<float>(guestCount) / MAX_ROOMS_AVAIL; //convert int occupied rooms to float
    
    //if occupancy half full take 10% off price, if + than 50% full add 10% to price
    if (occupancyRate < 0.5){
        
        changedPrice *=  0.9; ///10% off (100 -10%= 90% || 0.9)
    }
    else if (occupancyRate>=0.5){
         changedPrice *= 1.1; //10% more(100 + 10%= 110% || 1.1)
    }
    return changedPrice;
}
//incrrease price if any of these months
float seasonalAdj(float price,string month){
if (month =="April" || month == "May"|| month =="June" || month== "July" || month =="August") {
            return price * 1.2; // 20% increase of price because high demand season
    }
    return price;
}

//to get a random month so we can apply discount based on season
string randMonth() {
    string months[] = {"January", "February","March",  "April", "May", "June",
                       "July","August","September", "October ", "November","December"}; //array to hold months
   return months[rand()% 12];
    //return months[3];(tester)


}
//confirmation nums
long long getRandConf(){
    long long maxNum = 1000000000000;
     long long resNum = 1 + rand() % maxNum;
    return resNum;
    
}


