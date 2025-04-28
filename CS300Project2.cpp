// CS300Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
struct Node {
    Course course;
    Node* leftNode;
    Node* rightNode;
    Node(Course newCourse) {
        course = newCourse;
        leftNode = nullptr;
        rightNode = nullptr;
    }
};

//Defining BST class to hold data for the courses

class CoursesBST {
private: Node* root;

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
           return node;
       }

       //Searches for a specific course number in the BST by comparing the left/right nodes to the one that is being searched for
       Node* search(Node* node, string courseNumber) {
           if (node == nullptr || node->course.courseNumber == courseNumber) {
               return node;
           }
           if (courseNumber < node->course.courseNumber) {
               return search(node->leftNode, courseNumber);
           }
           else {
               return search(node->rightNode, courseNumber);
           }
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
