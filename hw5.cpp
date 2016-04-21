#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <deque>
using namespace std;

class NameMe {
private:

public:

};

void read_file_into_vector(const string & filename, vector<int> & target) {
	ifstream myfile;
	myfile.open(filename.c_str(), ifstream::in);
	int x;
	while (!myfile.eof()) {
		myfile >> x;
		target.push_back(x);
	}
}

//Swap two positions in a given array
void swap(vector<int> &subject,int a, int b) {
	int swp;
	swp = subject[b];
	subject[b] = subject[a];
	subject[a] = swp;
	cout << "Swapped " << subject[b] << " and " << subject[a] << "." << endl; 
}

//Bubble sort given array in place, and return # of swaps.
int bubble_sort (vector<int> &subject) {
	int swapcount = 0;
	bool not_done = true;
	while (not_done) {
		not_done = false; //If we make a swap we'll set this to true
		
		//We only iterate up to size - 1 since we compare each against the one after it
		//For every index t in the array
		for (int t=0; t < subject.size()-1; t++) {
			//If the value at t is greater than the value to the right of it, move it to the right.
			if (subject[t] > subject[t+1]) {
				not_done=true; //And do this until we go through the whole array without finding something to swap,
				swapcount++; //in which case the array must be sorted.
				swap(subject, t, t+1);
				break;
			}
		}
	}
	return swapcount;
}

//Insertion sort given array in place, return # of swaps
int insertion_sort(vector<int> &subject) {
	int swapcount = 0;
	//For every index in the array left-to-right...
	//(except the first, since we can't move it back any further anyway)
	for (int t=1; t < subject.size(); t++) {
		//Swap the value at t and the value to the immediate left until the value to the left of it
		//is less than it (or we hit the back end of the array).
		for (int i=t; (subject[i] < subject[i-1]) && (i > 0); i--) {
			swap(subject, i, i-1);
			swapcount++;
		}			
	}
	return swapcount;
}

//Returns x if it's greater than min, otherwise returns min.
int l_bound(int x, int min) {
	if (x < min) { return min; }
	return x;
}

//Shell sort given array in place, return # swaps
//Uses Ciura gap sequence, see:
//http://sun.aei.polsl.pl/~mciura/publikacje/shellsort.pdf
const int CUIRRA_GAPS[] = {701, 301, 132, 57, 23, 10, 4, 1};
const int GAP_ARRAY_LENGTH = 8;
int shell_sort(vector<int> &subject) {
	int gap;
	int swapcount = 0;
	//Do a sort for every gap value
	for (int gap_idx = 0; gap_idx < GAP_ARRAY_LENGTH; gap_idx++) {
		//Figure out how many spaces ahead we should look when sorting this iteration
		gap = CUIRRA_GAPS[gap_idx];
		
		//For every position in the array (except the first)...
		for (int t=0; t < subject.size(); t++) {
			//Do insertion sort (move value to the left until it's behind everything greater than it)
			//except we're looking gap steps behind it instead of only 1.
			for (int i=t; (subject[i] < subject[i-gap]) && (i-gap >= 0); i-=gap) {
				swap(subject, i, i-gap);
				swapcount++;
			}
		}
	}
	return swapcount;
}

//Merge sort (reursive implementation) given array in place, return # swaps
int merge_sort(deque<int> &subject, int depth=0) {
	int middle;
	int swapcount = 0;
	int arraylen;
	arraylen = subject.size();
	
	deque<int> left, right;
	
	//a list of size 0 or 1 is already sorted
	if (arraylen < 2) {return 0;}
	
	//Find the index we'll split the array by;
	middle = arraylen / 2;
	
	//Send the left half of the array to left, the right to right
	for (int t=0; t < arraylen; t++) {
		if (t >= middle) { left.push_back(subject[t]); }
		else { right.push_back(subject[t]); }
	}
	
	cout << "Left: " << left.size() << " elements. Right: " << right.size() << " elements." << endl;
	
	//Then mergesort those.
	swapcount += merge_sort(left, depth+1);
	cout << "Left: ";for (int t=0; t < left.size(); t++) {cout << left[t] << " ";} cout << endl;
	swapcount += merge_sort(right, depth+1);
	cout << "Right: ";for (int t=0; t < right.size(); t++) {cout << right[t] << " ";} cout << endl;
	

	//Clear out the array (don't worry; all the values it had are in the two other arrays...)
	subject.clear();
	
	//While both arrays still contain numbers...
	while ( !left.empty() && !right.empty() ) {
		//Get the lowest of the two leftmost values in the array...
		if (right.front() < left.front()) {
			//Then discard that value from the array it came from and put it in the result.
			subject.push_back( right.front() );
			cout << "Merged " << right.front() << " from right array (depth " << depth << ")." << endl;
			right.pop_front();
			swapcount++;
		} else {
			subject.push_back( left.front() );
			cout << "Merged " << left.front() << " from left array (depth " << depth << ")." << endl;
			left.pop_front();
			swapcount++;
		}
	}
	
	
	//Then stick the left over numbers on the end
	while (!left.empty()) { subject.push_back( left.front() ); cout << "Merged " << left.front() << " from left array (depth " << depth << ")" << endl; left.pop_front();}
	while (!right.empty()) { subject.push_back( right.front() ); cout << "Merged " << right.front() << " from right array (depth " << depth << ")" << endl; right.pop_front(); }
	
	return swapcount;
}

//wrapper that makes merge_sort compatible with the same API as the rest of the sort functions
int merge_sort(vector<int> &subject) {
	int swaps;
	deque<int> result;
	for (int t=0; t < subject.size(); t++) {result.push_back( subject[t] );}
	swaps=merge_sort(result, 0);
	subject.clear();
	while (!result.empty()) {subject.push_back( result.front()); result.pop_front();}
	return swaps;
}

//Print n spaces
void indent(int n) { for (int t=0; t < n; t++) { cout << " "; } }

int quicksort(vector<int> & subject, int depth=0) {
	vector<int> less, more, equal;
	int pivot;
	int swaps=0;
	
	if (subject.size() <= 1) { return 0; } //list of length 0 or 1 is sorted
	
	pivot=subject.back();
	indent(depth);
	cout << "Pivot = " << pivot << endl;
	for (int t=0; t < subject.size(); t++) {
		indent(depth);
		if (subject[t] > pivot) { more.push_back(subject[t]); cout << subject[t] << " > pivot" << endl; swaps++;}
		else if (subject[t] < pivot) { less.push_back(subject[t]); cout << subject[t] << " < pivot" << endl; swaps++;}
		else { equal.push_back(subject[t]); cout << subject[t] << " == pivot" << endl; swaps++;}
	}
	
	indent(depth);
	cout << "Less: ";for (int t=0; t < less.size(); t++) {cout << less[t] << " ";} cout << endl;
	indent(depth);
	cout << "More: ";for (int t=0; t < more.size(); t++) {cout << more[t] << " ";} cout << endl;
	
	swaps += quicksort(less, depth+1);
	indent(depth);
	cout << "Less (sorted): ";for (int t=0; t < less.size(); t++) {cout << less[t] << " ";} cout << endl;
	indent(depth);
	swaps += quicksort(more, depth+1);
	cout << "More (sorted): ";for (int t=0; t < more.size(); t++) {cout << more[t] << " ";} cout << endl;
	
	subject.clear();
	subject.reserve( less.size() + more.size() + equal.size());//make subject big enough
    subject.insert( subject.end(), less.begin(), less.end() );//append less
	subject.insert( subject.end(), equal.begin(), equal.end() );//append eq
	subject.insert( subject.end(), more.begin(), more.end() );//append more
	
	return swaps;
}

//Run all the sorts on the given test data
void run_sorts(string fname) {
	vector<int> workvec;
	int swaps;
	
	//Bubble
	read_file_into_vector(fname,workvec);
	cout << "Bubble sort:" << endl;
	swaps = bubble_sort(workvec);
	for (int t=0; t < workvec.size(); t++) {cout << workvec[t] << ", ";}
	cout << "\nMade " << swaps << " swaps." << endl << endl;
	workvec.clear();
	
	//Insertion
	read_file_into_vector(fname,workvec);
	cout << "Insertion sort:" << endl;
	swaps = insertion_sort(workvec);
	for (int t=0; t < workvec.size(); t++) {cout << workvec[t] << ", ";}
	cout << "\nMade " << swaps << " swaps." << endl << endl;
	workvec.clear();

	//Shell
	read_file_into_vector(fname,workvec);
	cout << "Shell sort:" << endl;
	swaps = shell_sort(workvec);
	for (int t=0; t < workvec.size(); t++) {cout << workvec[t] << ", ";}
	cout << "\nMade " << swaps << " swaps." << endl << endl;
	workvec.clear();
	
	//Merge
	read_file_into_vector(fname,workvec);
	cout << "Merge sort:" << endl;
	swaps = merge_sort(workvec);
	for (int t=0; t < workvec.size(); t++) {cout << workvec[t] << ", ";}
	cout << "\nMade " << swaps << " swaps." << endl << endl;
	workvec.clear();
	
	//Qsort
	read_file_into_vector(fname,workvec);
	cout << "Quicksort:" << endl;
	swaps = quicksort(workvec);
	for (int t=0; t < workvec.size(); t++) {cout << workvec[t] << ", ";}
	cout << "\nMade " << swaps << " swaps." << endl << endl;
	workvec.clear();
}

int main(int argc, char* argv[]) {
	run_sorts("test.txt");
}
