#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <chrono>
#include <string>

const int ARRAY_SIZE = 500;
const int MAX_VALUE = 1000;

/// typedefs for easy type creation
using TIME_REGISTER = std::map <std::string, std::chrono::time_point<std::chrono::steady_clock>>;
using TIME_VECTOR_PAIR = std::pair<std::string, long long>;
using TIME_VECTOR = std::vector<TIME_VECTOR_PAIR>;

TIME_REGISTER m_beginTimerList;
TIME_REGISTER m_finishTimerList;
TIME_VECTOR m_finalTimerSheetMs;

// log start timer
void addStartTime(std::string displayName)
{
	auto startTimer = std::chrono::high_resolution_clock::now();
	m_beginTimerList.insert(std::make_pair(displayName, startTimer));
	m_finalTimerSheetMs.push_back(TIME_VECTOR_PAIR(displayName, 0));
};
// lof finish timer
void addFinishTime(std::string id)
{
	//log finish timer
	auto finishTimer = std::chrono::high_resolution_clock::now();
	m_finishTimerList.insert(std::make_pair(id, finishTimer));
};
// print time sheet of finihsed timers
void printFinalTimeSheet()
{
	// iterate through all values in timesheet and print them.
	for (auto obj : m_finalTimerSheetMs)
	{
		auto differenceInTime = m_finishTimerList[obj.first] - m_beginTimerList[obj.first];
		obj.second = std::chrono::duration_cast<std::chrono::milliseconds>(differenceInTime).count();
		std::cout << obj.first << " : " << std::to_string(obj.second) << "ms to execute\n" << std::endl;
	}
}

// print results of current array
void print(std::vector<int> & array, int arraySize)
{
	for (size_t i = 0; i < arraySize; i++)
	{
		std::cout << array[i] << ",";
		i++;
	}
	std::cout << "\n";
}

int section(std::vector<int>& array, const int left, const int right) {
	// get a mid point in the array
	const int mid = left + (right - left) / 2;
	const int pivotPtr = array[mid];
	// move the mid point value to the front of the array.
	std::swap(array[mid], array[left]);
	std::shared_ptr<int> i_left = std::make_shared<int>( left + 1);
	std::shared_ptr<int> j_right = std::make_shared<int>(right);
	// loop through section between leftPtr and rightPtr to find pivots correct placing
	// while swapping  < and > values in array to pivot with each other 

//#pragma omp parallel for 
	while (*i_left <= *j_right) {
		// find next element from left  that is more then pivotPtr
		/// NOTE: checking for i_left and j_right are still valid
		while (*i_left <= *j_right && array[*i_left] <= pivotPtr) {
			*i_left = *i_left+1;
		}
		// find next element for far right which is smaller then pivot
		while (*i_left <= *j_right && array[*j_right] > pivotPtr) {
			*j_right = *j_right-1;
		}
		// double check if the left ptr is < right ptr. then swap
		if (*i_left < *j_right) {
			std::swap(array[*i_left], array[*j_right]);
		}
	}
	// swap original left with 
	std::swap(array[*i_left - 1], array[left]);
	// return new mid point
	return *i_left - 1;
}

void quicksort(std::vector<int> &array, const int left, const int right, const int arraySize) {
	// check if left and right ad still valid values
	if (left >= right)
		return;
	// get the new midpoint
	int midPtr = section(array, left, right);

	quicksort(array, left, midPtr - 1, arraySize);
	quicksort( array, midPtr + 1, right, arraySize);
}

int main() {
	// Vector of ints, as vector is on the heap not on the heap and threadsafe.
	std::vector<int> array;
	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		array.push_back((rand() % MAX_VALUE) + 1);
	}
	int arraySize = array.size();
	
	addStartTime("Quicksort Elapsed: ");

	quicksort(array, 0, arraySize - 1, arraySize);
	addFinishTime("Quicksort Elapsed: ");
	
	printFinalTimeSheet();
	print(array, arraySize);
	return 0;
}