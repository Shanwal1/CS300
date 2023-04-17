//Shannon Walden
//Project 2
//4/17/2023

#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>



using namespace std;



struct Course {
    
    string courseNumber; 
    string courseTitle;
    vector<string> prerequisites;
    
    Course() {}
    
};

struct Node {
    Course course;
    Node *left;
    Node *right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) :
            Node() {
        this->course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printSampleSchedule(Node* node);
    void printCourseInformation(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Remove(Node* node);
    void InsertCourse(Course course);
    int Prerequisites(Course course);
    void PrintSampleSchedule();
    void PrintCourseInformation(string courseNum);
    

};


 //Default constructor
BinarySearchTree::BinarySearchTree() {   
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    Remove(root);
}

void BinarySearchTree::Remove(Node* node) {
    if (node) {
        Remove(node->left);
        Remove(node->right);
        delete node;
    }
}
/**
 * Insert a Course
 */
void BinarySearchTree::InsertCourse(Course course) {
    if (root == nullptr) { 
        root = new Node(course); 
    }
    else {
        this->addNode(root, course); 
    }
}
int BinarySearchTree::Prerequisites(Course course) {
    int count = 0;
    for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
        if (course.prerequisites.at(i).length() > 0)
            count++;
    }
    return count;
}

void BinarySearchTree::PrintSampleSchedule() {
    this->printSampleSchedule(root);
}
void BinarySearchTree::PrintCourseInformation(string courseNumber) {
    this->printCourseInformation(root, courseNumber);
}
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {// if node is larger then add to left
        if (node->left == nullptr) {// if no left node
            node->left = new Node(course); // this node becomes left
        }
        else { // else recurse down the left node
            this->addNode(node->left, course);
        }
    }
    else {// else
        if (node->right == nullptr) {// if no right node
            node->right = new Node(course); // this node becomes right
        }
        else { // else recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::printSampleSchedule(Node* node) {
    if (node != nullptr) { //if node isn't empty
        printSampleSchedule(node->left); 
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        printSampleSchedule(node->right); 
    }
    return;
}
void BinarySearchTree::printCourseInformation(Node* current, string courseNumber) {
    while (current != nullptr) { //while current isn't empty
        if (current->course.courseNumber.compare(courseNumber) == 0) {// keep looping downwards until bottom reached or matching courseNumber found
            cout << " " << endl;
            cout << current->course.courseNumber << ", " << current->course.courseTitle << endl; //if match is found print it out
            unsigned int prereqs = Prerequisites(current->course);
            cout << " " << endl;
            cout << "Prerequisites:";
            unsigned int i = 0; //if current course has prerequisites print them out
            for (i = 0; i < prereqs; i++) {
                cout << current->course.prerequisites.at(i);
                if (i != prereqs - 1) {
                    cout << ", ";
                }
                if (i == 0) {
                    cout << current->course.prerequisites.at(i);
                    cout << " " << endl;
                return;
                }
            }
        }
        else if (courseNumber.compare(current->course.courseNumber)<0){
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    cout << "This course number cannot be found: " << courseNumber << endl;
}
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading course data file " << csvPath << endl;
        std::ifstream courseFile(csvPath);
        if (courseFile) {
            std::string line;
            while (std::getline(courseFile, line)) {
                std::istringstream iss(line);
                std::vector<std::string> tokens;
                std::string token;
                while (std::getline(iss, token, ',')) {
                    tokens.push_back(token);
                }
                // Create a data structure and add to the collection of Courses
                Course course;
                course.courseNumber = tokens[0];
                course.courseTitle = tokens[1];
                for (unsigned int i = 2; i < tokens.size(); i++) {
                    course.prerequisites.push_back(tokens[i]);
                }
                bst->InsertCourse(course); // push this course to the end
            }
            courseFile.close(); //close the data file
            return true;

        }
    return false;

}
  

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "";
        break;
    }



    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = nullptr;
    
    cout << "Welcome to the course planner." << endl;
    cout << " " << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "  1. Load Data Structure. " << endl;
        cout << "  2. Print Course List. " << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << " " << endl;
        cout << "What would you like to do?";
        cin >> choice;
        

        switch (choice) {

        case 1:
            bst = new BinarySearchTree;
            cout << "Please enter the course data file you would like to be loaded" << endl;
            cin >> csvPath;
            loadCourses(csvPath, bst);
            break;

        case 2:
            if (bst != nullptr ) {
                cout << "Here is a sample schedule:" << endl;
                cout << " " << endl;
                bst->PrintSampleSchedule();
                cout << " " << endl;
            }
            else {
                cout << "Please load the course data file first" << endl;
            }
            break;

        case 3:
            if (bst != nullptr)  {
                if (courseKey.length() == 0) {
                    cout << "What course do you want to know about?" << endl;
                    cin >> courseKey;
                    for (auto& userChoice : courseKey)userChoice = toupper(userChoice);
                    bst->PrintCourseInformation(courseKey);
                }
                else {
                    cout << "Please load the course data file first" << endl;
                }
            }
            courseKey = "";
            break;

        default:
            if (choice != 9) {
                cout << choice << " is not a valid option." << endl;
                cout << " " << endl;
                break;

           }
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

	return 0;
}
