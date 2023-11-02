#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void allocate(int *mover, ifstream &a,bool &space) { // populate array with data from file
    string allocate;
    for (int i = 0; i < 9; i++) { // read in 9 different lines until getline is found
        getline(a, allocate, '\n');
        for (char c: allocate) { // for each character within this line
            if (c == '\n') { // ignore newline character
                continue;
            } else {
                if (isspace(c)) {
                    *mover = 0; // populate array with value 0 to represent space within file
                    space = true; // this makes it impossible for the file to be "solved", only valid
                    mover++; // move the pointer to the next elemeent
                } else { // this element is not a space, therefore it is a number
                    *mover = int(c) - 48; // populate element of array with the integer
                    // int returns the ascii value of the char, subtracting 48 gives us the integer
                    mover++; // move the element

                }
            }
        }
    }
}

void (validateRows)(int * mover, string name, bool &valid,int count){
    int *compare = new int [9]; // dynamic allocation of comparison array
    int  *test = nullptr; // keeping track of the head of our comparison pointer
    test = compare;
    for(int i = 0; i < 9; i++){ // populate comparison array with 0's
        *compare = 0;
        compare++;
    }
    compare = test; // reset comparison pointer to start
    for(int i = 0; i < 9; i++){ // mover moves correctly
        if(*mover == 0){ // if white space (represented by 0), ignore and move pointer
            mover++; continue;
        }
        else {
            int x = *mover - 1; // subtracting 1 to populate that number element
            compare += x; // move pointer to numbered element
            ++*(compare); // increment that value to track how many times it has occured within row
            mover++; // increment pointer to get next one within allocation
            compare = test; // reset comparison pointer
        }
    }

    for(int i = 0; i < 9; i++){ // check for duplicates within row
        if(*compare > 1) { // if value is greater than 1, it showed multiple times within the row
            valid = false; // making the puzzle invalid
            cout << setw(4) << left << name << "\tinvalid\t row " << count + 1 <<  " has multiple " << i+1 << "s" << endl;
        }
        compare++; // move to the next array element
    }
    compare = test; // must point to start of array prior to deletion
    delete [] compare; // delete array
}

void validateColumns(int *mover, string name,bool &valid, int count) {
    int *compare = new int [9]; // dynamic allocation of comparison array
    int  *test = nullptr;
    test = compare;
    for(int i = 0; i < 9; i++){ // populate array with 0's
        *compare = 0;
        compare++;
    }
    compare = test;
    for(int i = 0; i < 9; i++){ // 9 columns every call
        int x = *mover - 1; // subtracting 1 to populate that number element
        compare+=x; // move pointer to numbered element
        ++*(compare); // increment that value to track how many times it has occured within column
        mover+=9; // increment pointer to get next one within allocation
        compare = test; // reset comparison pointer
    }
    compare = test;
    for(int i = 0; i < 9; i++){ // check for duplicates within column
        if(*compare > 1) { // if value is greater than 1, it showed multiple times within the column
            valid = false; // the puzzle is invalid due to duplicate
            cout << setw(4) << left << name << "\tinvalid\t column " << count + 1 << " has multiple " << i+1 << "s" << endl;
        }
        compare++; // move the array to next element
    }
    // deleting the array here produces "double free or corruption (out)" error
}

void validateBox(int *mover, string name, int count,bool &valid){
    int *compare = new int [9];
    int  *test = nullptr;
    test = compare;
    for(int i = 0; i < 9; i++){ // populate 0's within array
        *compare = 0;
        compare++;
    }
    compare = test;
    for(int i = 0; i < 3; i++){ // validate 1 3x3 box, row by row
        int x;
        if(*mover != 0) { // white space would move the pointer out of bounds
            x = *mover - 1; // subtracting 1 to populate that number element
            compare += x; // move pointer to numbered element
            ++*(compare); // increment that value to track how many times it has occured within box
            compare = test; // reset comparison pointer
        }
        for(int j = 0; j < 2; j++){ // validate following 2 elements
            mover++;
            if(*mover == 0){continue;}
            x = *mover - 1;
            compare+=x;
            ++*(compare);
            compare = test;
        }
        compare = test; // reset comparison pointer to start
        mover+=7; // move pointer to next row
    }
    compare = test;
    for(int i = 0; i < 9; i++){ // output any invalidity
        if(*compare > 1) {
            valid = false;
            if(count == 0){cout << name << "\tinvalid\t upper left has multiple " << i+1 << "s" << endl;}
            else if(count == 1){cout << setw(4) << left << name << "\tinvalid\t upper middle has multiple " << i+1 << "s" << endl;}
            else if(count == 2){cout << setw(4) << left << name << "\tinvalid\t upper right  has multiple " << i+1 << "s" << endl;}
            else if(count == 3){cout << setw(4) << left << name << "\tinvalid\t left has multiple " << i+1 << "s" << endl;}
            else if(count == 4){cout << setw(4) << left << name << "\tinvalid\t middle has multiple " << i+1 << "s" << endl;}
            else if(count == 5){cout << setw(4) << left << name << "\tinvalid\t right has multiple " << i+1 << "s" << endl;}
            else if(count == 6){cout << setw(4) << left << name << "\tinvalid\t lower left has multiple " << i+1 << "s" << endl;}
            else if(count == 7){cout << setw(4) << left << name << "\tinvalid\t bottom middle has multiple " << i+1 << "s" << endl;}
            else if(count == 8){cout << setw(4) << left << name << "\tinvalid\t lower right has multiple " << i+1 << "s" << endl;}
        }
        compare++;
    }
    compare = test;
    delete [] compare;
}

int main() { //
    string name, file; // puzzle name string and file name
    cin >> file; // read in file name
    ifstream a(file);
    if (a.good()) { // if the file stream is in a good state
        int *full = new int[81]; // where the puzzle will be stored
        int *mover = nullptr; // pointer to traverse the puzzle
        mover = full; //
        while (getline(a, name)) { // so long as there are files to read, the first puzzle name will go here
            while (name.empty() && !a.eof()) { // the following puzzle names will come after an empty line
                getline(a, name);
            }
            if(a.eof()){break;} // file could end with an empty line
            bool space = false; // if there are spaces, this is true -> puzzle is valid until bottom;
            bool valid = true; // there can be spaces and it still be valid, this is to keep track
            mover = full;
            allocate(mover, a, space); // build our array
            int box = 0; // store the math used to point to the top left of each 3x3 box
            mover = full; // allocate moved pointer mover, reset it to start of array
            for (int p = 0; p < 9; p++){ // for all 9 rows / columns / boxes
                mover = full;
                mover+=(9*p); // point to 0th, 9th, 18th... element in array
                validateRows(mover, name,valid,p); // validate said rows
                mover = full; // reset pointer
                mover+=p; // point to 0th, 1st, 2nd element in array as start of column
                validateColumns(mover, name,valid,p); // validate said column
                mover = full; // reset pointer
                if(p%3 == 0){box = 27 * (p/3);} // 27 skips three lines, division is for the 0th iteration
                else if (p < 3){box = 3*p;} // top left of 2nd, 3rd box
                else{box+=3;} // if not prior conditions, move the pointer by 3 so it points to top left
                mover+=box; // moving the pointer
                validateBox(mover, name, p,valid); // validating
            }
            if(valid && !space){cout << name << '\t' << "solved" << endl;} // output statement
            else if(valid && space){cout << name << '\t' << "valid" << endl;} // output statement
        }
        a.close(); // close file stream
        delete [] full; // delete dynamically allocated array
    }
    else{
        cout << "File could not be opened." << endl;
    }
}
