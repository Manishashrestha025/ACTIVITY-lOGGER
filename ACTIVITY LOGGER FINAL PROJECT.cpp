#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <queue>

using namespace std;

// Structure to represent a task
struct Task {
    string name;
    tm dueDateTime;

    Task(const string& n, const tm& due) : name(n), dueDateTime(due) {}

    // Overloading '<' operator for comparison based on due date and time
    bool operator<(const Task& other) const {
        return mktime(const_cast<tm*>(&dueDateTime)) > mktime(const_cast<tm*>(&other.dueDateTime)); // Earlier due dates have higher priority
    }
};

// Class to manage the to-do list using priority queue and linked list
class ToDoList {
private:
    priority_queue<Task> taskQueue; // Priority queue to store tasks sorted by due date/time
    struct Node {
        Task task;
        Node* next;
        Node(const Task& t) : task(t), next(nullptr) {}
    };
    Node* head; // Head of linked list to store tasks in the order they were added

public:
    ToDoList() : head(nullptr) {}

    // Function to add a task to the to-do list
    void addTask(const string& name, const tm& dueDateTime) {
        Task task(name, dueDateTime);
        taskQueue.push(task);
        addToLinkedList(task);
        cout << "Activity Name added successfully.\n";
    }

    // Function to add a task to the linked list
    void addToLinkedList(const Task& task) {
        Node* newNode = new Node(task);
        if (!head || task.dueDateTime.tm_year < head->task.dueDateTime.tm_year ||
            (task.dueDateTime.tm_year == head->task.dueDateTime.tm_year &&
             difftime(mktime(const_cast<tm*>(&task.dueDateTime)), mktime(const_cast<tm*>(&head->task.dueDateTime))) < 0)) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;
        while (current && (task.dueDateTime.tm_year > current->task.dueDateTime.tm_year ||
                           (task.dueDateTime.tm_year == current->task.dueDateTime.tm_year &&
                            difftime(mktime(const_cast<tm*>(&task.dueDateTime)), mktime(const_cast<tm*>(&current->task.dueDateTime))) > 0))) {
            prev = current;
            current = current->next;
        }
        prev->next = newNode;
        newNode->next = current;
    }

    // Function to remove a task from the to-do list
    void removeTask(const string& name) {
        if (taskQueue.empty()) {
            cout << "No Activity Name in the Activity logger.\n";
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        // Remove task from linked list
        while (current && current->task.name != name) {
            prev = current;
            current = current->next;
        }

        if (!current) {
            cout << "Activity Name not found.\n";
            return;
        }

        if (!prev) {
            head = head->next;
        } else {
            prev->next = current->next;
        }

        delete current;

        // Remove task from priority queue
        priority_queue<Task> temp;
        while (!taskQueue.empty()) {
            if (taskQueue.top().name != name) {
                temp.push(taskQueue.top());
            }
            taskQueue.pop();
        }
        taskQueue = temp;

        cout << "Activity Name removed successfully.\n";
    }

    // Function to display all tasks in the to-do list
    void displayTasks() {
        if (!head) {
            cout << "No tasks in the Activity logger.\n";
            return;
        }

        cout << "To-Do List:\n";
        Node* current = head;
        int index = 1;
        while (current) {
            cout << index++ << ". " << current->task.name << " (Due: " << formatDateTime(current->task.dueDateTime) << ")\n";
            current = current->next;
        }
    }

    // Function to format date and time as string
    string formatDateTime(const tm& dateTime) {
        stringstream ss;
        ss << setfill('0') << setw(4) << dateTime.tm_year + 1900 << "-" << setw(2) << dateTime.tm_mon + 1 << "-" << setw(2) << dateTime.tm_mday
            << " " << setw(2) << dateTime.tm_hour << ":" << setw(2) << dateTime.tm_min;
        return ss.str();
    }

    ~ToDoList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Function to parse date and time string
tm parseDateTime(const string& dateStr, const string& timeStr) {
    tm dateTime = {};
    sscanf(dateStr.c_str(), "%d-%d-%d", &dateTime.tm_year, &dateTime.tm_mon, &dateTime.tm_mday);
    sscanf(timeStr.c_str(), "%d:%d", &dateTime.tm_hour, &dateTime.tm_min);
    dateTime.tm_year -= 1900; // Adjust year to tm_year format
    dateTime.tm_mon -= 1; // Adjust month to tm_mon format
    return dateTime;
}

int main() {
    ToDoList todoList;

    cout << "Welcome to the The Activity logger :)\n";


                string name, dueDateStr, dueTimeStr;
                cout << "Enter task Activity Name: ";
                getline(cin, name);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDateStr);

                cout << "Enter due time(HH:MM): ";
                getline(cin, dueTimeStr);

                tm dueDateTime = parseDateTime(dueDateStr, dueTimeStr);
                todoList.addTask(name, dueDateTime);
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Activity Name\n";
        cout << "2. Remove Activity Name\n";
        cout << "3. Display Activity Name\n";
        cout << "4. Exit \n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore(); // Ignore newline character

        switch (choice) {
            case 1: {

                cout << "Enter task Activity Name: ";
                getline(cin, name);

                cout << "Enter due date (YYYY-MM-DD): ";
                getline(cin, dueDateStr);

                cout << "Enter due time(HH:MM): ";
                getline(cin, dueTimeStr);

                tm dueDateTime = parseDateTime(dueDateStr, dueTimeStr);
                todoList.addTask(name, dueDateTime);
              break;
            }
            case 2: {
                string name;
                cout << "Enter the name of the Activity to remove:";
                getline(cin, name);
                todoList.removeTask(name);
                break;
            }
            case 3:
                todoList.displayTasks();
                break;
            case 4:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
