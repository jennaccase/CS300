//============================================================================
// Name        : JCase Final Project.cpp
// Author      : Jenna Case
// Description : Project 2
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

//default time for sleep
const int GLOBAL_SLEEP_TIME = 5000;

//struct to hold course info
struct Course {
	string courseNumber;
	string courseName;
	vector<string> preReqList;
};

//define binary search tree
class BinarySearchTree {

	//structures to hold the courses
private:
	struct Node {
		Course course;
		Node* right;
		Node* left;

		//default constructor
		Node() {
			left = nullptr;
			right = nullptr;
		}

		//initilize
		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinarySearchTree();
	void inOrder();
	void Insert(Course aCourse);
	void Remove(string courseNumber);
	Course Search(string courseNumber);
	int Size();
};

//default construction
BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}

//traverse tree in order
void BinarySearchTree::inOrder() {
	inOrder(root);
}

//insert
void BinarySearchTree::Insert(Course aCourse) {
	Node* currNode = root;

	if (root == NULL) {
		root = new Node(aCourse);
	}

	else {
		while (currNode != NULL) {
			if (aCourse.courseNumber < currNode->course.courseNumber) {
				if (currNode->left == nullptr) {
					currNode->left = new Node(aCourse);
					currNode = NULL;
				}
				else {
					currNode = currNode->left;
				}
			}
			else {
				if (currNode->right == nullptr) {
					currNode->right = new Node(aCourse);
					currNode = NULL;
				}
				else {
					currNode = currNode->right;
				}
			}
		}
	}
	size++;
}

//search for a course
Course BinarySearchTree::Search(string courseNumber) {
	Course aCourse;
	Node* currNode = root;

	while (currNode != NULL) {
		if (currNode->course.courseNumber == courseNumber) {
			return currNode->course;
		}
		else if (courseNumber < currNode->course.courseNumber) {
			currNode = currNode->left;
		}
		else {
			currNode = currNode->right;
		}
	}
	return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
	if (node == NULL) {
		return;
	}
	inOrder(node->left);

	//print the node
	cout << node->course.courseNumber << ", " << node->course.courseName << endl;

	inOrder(node->right);
}

int BinarySearchTree::Size() {
	return size;
}

// using https://www.grepper.com/answers/273735/implementing+split+function+in+c%2B%2B to split
vector<string> Split(string lineFeed) {
	char delim = ',';

	lineFeed += delim;
	vector<string> lineTokens;
	string temp = "";
	for (int i = 0; i < lineFeed.length(); i++) {
		if (lineFeed[i] == delim) {
			lineTokens.push_back(temp);
			temp = "";
			i++;
		}
		temp += lineFeed[i];
	}
	return lineTokens;
}

//load the course
void loadCourse(string csvPath, BinarySearchTree* courseList) {
	ifstream inFS;
	string line;
	vector<string> stringTokens;

	inFS.open(csvPath);

	if (!inFS.is_open()) {
		cout << "Could not open the file." << endl;
		return;
	}

	while (!inFS.eof()) {
		Course aCourse;

		getline(inFS, line);
		stringTokens = Split(line);

		if (stringTokens.size() < 2) {
			cout << "\nError, skipping line." << endl;
		}
		else {
			aCourse.courseNumber = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (unsigned int i = 2; i < stringTokens.size(); i++) {
				aCourse.preReqList.push_back(stringTokens.at(i));
			}
			//course to end 
			courseList->Insert(aCourse);
		}
	}
	inFS.close();
}

//display courses
void displayCourse(Course aCourse) {
	cout << aCourse.courseNumber << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";

	if (aCourse.preReqList.empty()) {
		cout << "None" << endl;
	}
	else {
		for (unsigned int i = 0; i < aCourse.preReqList.size(); i++) {
			cout << aCourse.preReqList.at(i);

			//separate prereqs by comma
			if (aCourse.preReqList.size() > 1 && i < aCourse.preReqList.size() - 1) {
				cout << ", ";
			}
		}
	}
	cout << endl;
}

//main
int main(int argc, char* argv[]) {
	string csvPath, aCourseKey;

	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "ABCU_Advising_Program_Input.csv";
	}

	//table to hold courses
	BinarySearchTree* courseList = new BinarySearchTree();

	Course course;
	bool goodInput;
	int choice = 0;

	//menu display
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "1. Load Courses" << endl;
		cout << "2. Display All Course" << endl;
		cout << "3. Search for Course" << endl;
		cout << "9. Exit Program" << endl;
		cout << "Please enter choice: ";

		//clear the string and choice
		aCourseKey = "";
		string anyKey = " ";
		choice = 0;

		try {
			cin >> choice;

			if ((choice > 0 && choice < 4) || (choice == 9)) {
				goodInput = true;
			}
			else {
				goodInput = false;
				throw 1;
			}

			switch (choice) {
			case 1:
				//call loadCourse
				loadCourse(csvPath, courseList);
				cout << courseList->Size() << " courses read" << endl;

				Sleep(GLOBAL_SLEEP_TIME);
				break;

			case 2:
				courseList->inOrder();

				cout << "\nEnter \'y\' to continue..." << endl;
				cin >> anyKey;
				break;

			case 3:
				cout << "\nWhat course do you want to know about?" << endl;
				cin >> aCourseKey;

				course = courseList->Search(aCourseKey);

				if (!course.courseNumber.empty()) {
					displayCourse(course);
				}
				else {
					cout << "\nCourse Number " << aCourseKey << " was not found" << endl;
				}

				Sleep(GLOBAL_SLEEP_TIME);
				break;

			case 9:
				exit;
				break;

			default: throw 2;
			}
		}
		catch (int err) {
			std::cout << "\nPlease check your input" << endl;
			Sleep(GLOBAL_SLEEP_TIME);
		}

		//clear extra input
		cin.clear();
		cin.ignore();

		//clear for new menu
		system("cls");
	}
	cout << "Goodbye" << endl;

	Sleep(GLOBAL_SLEEP_TIME);
	return 0;
}
	
