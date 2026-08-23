// CS300Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <algorithm>
#include <limits>
#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


//Defining structure for the course
struct Course {
    string courseNumber;
    string courseName;
    //vector for the prerequisities for a course
    vector<string> prereq;
};


//Defining structure for the node of the BST
//Tracking the nodes height and the number of comparisons made during a search
struct Node {
    Course course;
    Node* leftNode;
    Node* rightNode;
    int height;
    Node(Course newCourse) {
        course = newCourse;
        leftNode = nullptr;
        rightNode = nullptr;
		height = 1;
    }
};

//Defining BST class to hold data for the courses
class CoursesBST {
private: Node* root;
       int searchComparisons = 0;

	   //Calculates the height of a node by checking if the node is null and returning 0 if it is, otherwise it returns the height of the node
       int height(Node* node) {
           if (node == nullptr) {
               return 0;
           }
           return node->height;
	   }

	   //Updates the height of a node by taking the maximum height of the left and right nodes and adding 1 to it
       void updateHeight(Node* node) {
		   node->height = 1 + max(height(node->leftNode), height(node->rightNode));

	   }

	   //Calculates the balance factor of a node by subtracting the height of the right node from the height of the left node
	   //The balance factor is used to determine if the tree is balanced or not
	   //If the balance factor is greater than 1 or less than -1 then the tree is unbalanced and needs to be rotated
       
       int getBalance(Node* node) {
           if (node == nullptr) {
               return 0;
           }
           return height(node->leftNode) - height(node->rightNode);
	   }
      
	   //Calculates the height of the BST by recursively traversing the left and right nodes and returning the maximum height
       int treeHeight(Node* node) {
           if (node == nullptr) {
               return 0;
           }
			   return 1 + max(treeHeight(node->leftNode), treeHeight(node->rightNode));

       }

       ///Inserts node into BST in order by coparing the left node and right node to the current course number
       //If course number is less than the current course number it will go to the left node and vice versa if it is greater
       Node* insert(Node* node, Course course) {
           if (node == nullptr) {
               return new Node(course);
           }
           if (course.courseNumber < node->course.courseNumber) {
               node->leftNode = insert(node->leftNode, course);
           }
           else {
               node->rightNode = insert(node->rightNode, course);
           }
           
		   //Updates the height of the node after insertion
		   updateHeight(node);
		   int balance = getBalance(node);

		   //Left Left Case
           if (balance > 1 && course.courseNumber < node->leftNode->course.courseNumber) {
               return rotateRight(node);
           }

		   //Right Right Case
           if (balance < -1 && course.courseNumber >= node->rightNode->course.courseNumber) {
               return rotateLeft(node);
		   }

		   //Left Right Case
           if (balance > 1 && course.courseNumber >= node->leftNode->course.courseNumber) {
               node->leftNode = rotateLeft(node->leftNode);
               return rotateRight(node);
		   }

		   //Right Left Case
           if (balance < -1 && course.courseNumber < node->rightNode->course.courseNumber) {
               node->rightNode = rotateRight(node->rightNode);
			   return rotateLeft(node);
               }
		   return node;
       }

       //Searches for a specific course number in the BST by comparing the left/right nodes to the one that is being searched for
       Node* search(Node* node, string courseNumber) {
           if (node == nullptr) 
           {
               return nullptr;
               
           }
           searchComparisons++;
           if (node->course.courseNumber == courseNumber)
           {
               return node;
           }
           if (courseNumber < node->course.courseNumber)
           {
               return search(node->leftNode, courseNumber);
           }
           else {
			   return search(node->rightNode, courseNumber);
           }
       }

	   //Rotates the tree to the right by making the left node the new root and the current root the right node of the new root
       Node* rotateRight(Node* y) {
           Node* x = y->leftNode;
           Node* T2 = x->rightNode;
           x->rightNode = y;
           y->leftNode = T2;
           updateHeight(y);
           updateHeight(x);
           return x;
	   }

	   //Rotates the tree to the left by making the right node the new root and the current root the left node of the new root
       Node* rotateLeft(Node* x) {
           Node* y = x->rightNode;
           Node* T2 = y->leftNode;
           y->leftNode = x;
           x->rightNode = T2;
           updateHeight(x);
           updateHeight(y);
           return y;
	   }

       //Traverses the BST in order and prints the course number and course name by visiting the left child node, then the current node, and then the right child
       void printInOrder(Node* node) {
           if (node == nullptr) {
               return;
           }
               printInOrder(node->leftNode);
               cout << node->course.courseNumber << ": " << node->course.courseName << endl;
               printInOrder(node->rightNode);
           
       }

public:
    CoursesBST() {
        root = nullptr;
    }

    int getHeight() {
        return treeHeight(root);
	}

    void insert(Course course) {
        root = insert(root, course);
    }
    //Searches for a specific course in the BST and returns the node if it is found
    //Prints the nodes information
    void printCourse() {

        string userCourse;
        cout << "Enter the course number: ";
        cin.ignore(); //Clear input buffer
        getline(cin, userCourse);
		searchComparisons = 0;
        Node* foundNode = search(root, userCourse);
        if (foundNode != nullptr) {
            cout << "Course has been found " << endl;
            cout << foundNode->course.courseNumber << ": " << foundNode->course.courseName << endl;
            cout << "Course's prerequisites: ";
            if (foundNode->course.prereq.empty()) {
                cout << "No prerequisities " << endl;
            }
            else {
                for (int i = 0; i < foundNode->course.prereq.size(); ++i) {
                    cout << foundNode->course.prereq[i] << " ";
                }
            }
        }

        else {
            cout << "Course not found" << endl;
        }

        cout << "Total Comparisons during search: " << searchComparisons << endl;
    }


    void printAllCourses() {
        printInOrder(root);
    }
};
    

//Read and load the courses from the CSV file
void loadCourses(string fileName, CoursesBST& bst) {
    string fileLine;
    ifstream file(fileName);
    //Checks if the file is open and can be opened
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << endl;
        return;
    }
    //Reads each line in the file and splits each line into corresppounding tokens 
    while (getline(file, fileLine)) {
        stringstream ss(fileLine);
        string token;
        Course course;
        getline(ss, token, ',');
        course.courseNumber = token;
        getline(ss, token, ',');
        course.courseName = token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                course.prereq.push_back(token);
            }
        }
        bst.insert(course);
    }
    file.close();
    cout << "Courses have been loaded successfully " << endl;

    //splits the line into tokens and sets each token to the appropiate w

}


int main()
{
	//Creating a BST object to hold the courses
    CoursesBST bst;
    string fileName;
    int userChoice = 0;
   
    //Displaying a menu for the user to choose from
    //Exits when the users enter the option 9
    while (userChoice != 9) {

        cout << "-------MENU-------" << endl;
        cout << "1: Load Courses" << endl;
        cout << "2: Print Courses in alphanumeric order" << endl;
        cout << "3: Print course title and prerequisites" << endl;
        cout << "9: Exit " << endl;
		cout << "Enter your choice: ";
        cin >> userChoice;

        //Determines what option the user has selected and performs the corresponding action
        //If the user enters invalid option it will display an error message
        switch (userChoice) {
        case 1: 
            cout << "Enter the name of the file: ";
            cin.clear();
            cin.ignore(); //Clear input buffer
            getline(cin, fileName);
            loadCourses(fileName, bst);
            break;
        case 2:
            bst.printAllCourses();
            break;
        case 3: 
            bst.printCourse();
            cout << endl;
            break;
        case 9: 
            cout << "Terminating Program" << endl;
            break;
        default: 
            cout << "You entered an invalid option. Please try again." << endl;
        }
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
