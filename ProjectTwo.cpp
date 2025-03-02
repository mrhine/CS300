// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Marissa Rhine
// Date Created: Feb 22, 2025
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


using namespace std;

// Struct to hold course information
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Struct for a node in the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;
};

// Define the root of the binary search tree
Node* root = nullptr;


// Function to insert a new course into the binary search tree
void insertNode(Node*& node, const Course& course) {
    if (node == nullptr) {
        node = new Node{ course, nullptr, nullptr };
    }
    else if (course.courseNumber < node->course.courseNumber) {
        insertNode(node->left, course);
    }
    else {
        insertNode(node->right, course);
    }
}

void addCourse(const string& courseNumber, const string& title, const vector<string>& prerequisites) {
    Course newCourse;
    newCourse.courseNumber = courseNumber;
    newCourse.title = title;
    newCourse.prerequisites = prerequisites;

    insertNode(root, newCourse);
}

// Function to search for a course
Course* searchCourse(Node* node, const string& courseNumber) {
    if (node == nullptr) {
        return nullptr;
    }
    else if (courseNumber == node->course.courseNumber) {
        return &node->course;
    }
    else if (courseNumber < node->course.courseNumber) {
        return searchCourse(node->left, courseNumber);
    }
    else {
        return searchCourse(node->right, courseNumber);
    }
}

// Function to check if line entry is in valid format
bool isValidFormat(const string& line) {
    // Check if there are at least two commas (for courseNumber, title, and at least one prerequisite)
    int commaCount;
    commaCount = count(line.begin(), line.end(), ',');
    if (commaCount < 2) {
        return false;
    }
    return true;
}

// Function to read and parse from file
void readAndParseFile(const string& fileName) {
    ifstream file(fileName);
    string line;

    if (file.is_open()) {

        while (getline(file, line)) {

            if (isValidFormat(line)) {

                int firstComma = line.find(',');
                int secondComma = line.find(',', firstComma + 1);

                string courseNumber = line.substr(0, firstComma);
                string title = line.substr(firstComma + 1, secondComma - firstComma - 1);

                vector<string> prerequisites;
                string prereq;

                // Use a stringstream to extract prerequisites
                stringstream ss(line.substr(secondComma + 1));
                while (getline(ss, prereq, ',')) {
                    if (!prereq.empty()) {
                        prerequisites.push_back(prereq);
                    }
                }
                // Add the course to the binary search tree
                addCourse(courseNumber, title, prerequisites);
            }
        }
        file.close();
        cout << "File loaded." << endl;
    }
}


// Function to print course information
void printCourseInfo(const string& courseNumber) {
    Course* course = searchCourse(root, courseNumber);
    if (course != nullptr) {
        cout << "Course Number: " << course->courseNumber << endl;
        cout << "Title: " << course->title << endl;
        cout << "Prerequisites: ";
        if (course->prerequisites.empty()) {
            cout << "None" << endl;
        }
        else {
            for (const string& prereq : course->prerequisites) {
                cout << prereq << " ";
            }
            cout << endl;
        }
    }
    else {
        cout << "Course not found" << endl;
    }
}

// Function to perform an inorder traversal of the tree (helper function)
void inorderTraversal(Node* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << endl;
        inorderTraversal(node->right);
    }
}

// Function to print the course list in alphanumeric order
void printCourseList() {
    cout << "Course List:" << endl;
    inorderTraversal(root); // Directly call the inorderTraversal helper function
}

int main() {
    int choice;

    do {
        // Display menu
        cout << "Menu:" << endl;
        cout << "1. Load Courses" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Find Details for Course" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: //1. Load Courses
        {
            string fileName;
            cout << "Enter name of file: ";
            cin.ignore();
            getline(cin, fileName);
            cout << "Opening fileName" << fileName << endl;
            readAndParseFile(fileName);
            break;
        }
        case 2: //2. Print Course List
            printCourseList();
            break;
        case 3: //3. Find Details for Course
        {
            string courseNumber;
            cout << "Enter course number: " << endl;
            cin >> courseNumber;
            printCourseInfo(courseNumber);
            break;
        }
        case 9: //9. Exit
            cout << "Goodbye." << endl;
            break;
        }

    } while (choice != 9);

    return 0;
}