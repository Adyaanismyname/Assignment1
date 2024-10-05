#include <iostream>
using namespace std;

int id_counter = 1000;

// Making the Processes class
class Process {
  public:
  int process_id;
  string process_name;
  int execution_time;
  int remaining_time;
  Process* next; // For linked lists

  // Constructor
  Process(string name, int exec_time) {
    process_id = id_counter++;
    process_name = name;
    execution_time = exec_time;
    remaining_time = exec_time;
    next = nullptr;
  }

};

// Making the Scheduler class
class Scheduler {
  Process* head; // points to the head of the linked list
  int cpu_time_per_cycle = 3;


  public:
  // Constructor
  Scheduler() {
    head = nullptr;
  }

  // Method to Add a Process
  void addProcess(string name , int time) {
    Process* new_process = new Process(name, time);
    if(head == nullptr) {
      head = new_process;
      new_process->next = head;
    } else {
      Process* ptr = head;
      while(ptr->next != head) {
        ptr = ptr->next;
      }

      new_process->next = head; // maintaining the circular linked list
      ptr->next = new_process;
    }
  }
  // Methods to Remove a Process
  void removeProcess(int process_id) {
    Process* prev = nullptr;
    Process* curr = head;

    // Handling the case where the process is the first in the list
    if(curr->process_id == process_id) {

      Process* last = head;
      while(last->next != head) {
        last = last->next;
      }

      // assigning the last node to the new head
      last->next = curr->next;

      head = curr->next;
      delete curr;

      return;
    }

    // looping through the list
    do {
      // Remove the process
      if(curr->process_id == process_id) {
        prev->next = curr->next;
        delete curr;
        return;
      }

      // Incrementing the pointers
      prev = curr;
      curr = curr->next;

    }
    while(curr != head);



  }

  // Method to Print all the Processes
  void print() {
    // Making a do-while loop to loop through the list of processes
    Process* ptr = head;

    // if list is empty
    if (head == nullptr) {
      return;
    }

    do {
      // outputting the process name and the remaing time
      if(ptr->remaining_time > 0) {
        cout << ptr->process_name << " Remaining Time: " << ptr->remaining_time << ", ";
      }
      else {
        cout << ptr->process_name << ", Completed" << " ";
      }
      ptr = ptr->next;
    }
    while(ptr != head);
  }
  // Deduct remaining time from each process
  void deductRemainingTime(int cpu_time_per_cycle) {
    Process* ptr = head;
    do {
      ptr->remaining_time -= cpu_time_per_cycle;
      ptr = ptr->next;
    }
    while(ptr != head);
  }

  // Checking if any process is completed and removing it accordingly
  void checkAndRemoveCompletedProcesses() {

    // looping through the list
    Process* ptr = head;
    do {
      // checking if the process is completed
      if(ptr->remaining_time <= 0) {
        // checking if that process is the only process left
        if(ptr == head) {
          head = nullptr;
          return;
        }

        // if not the only process left
        Process*temp = ptr->next;
        this->removeProcess(ptr->process_id);
        ptr = temp;

      }
      else {
        ptr = ptr->next;

      }
    }
    while(ptr != head);
  }

  // Method for the scheduler
  void cycleProcesses() {
    int cycle_counter = 1;
    cout << "CPU Time per Process per Cycle : " << cpu_time_per_cycle; // CPU Time per Cycle


    while(head != nullptr) {
      cout << "\nCycle: " << cycle_counter << " ";

      // deducting remaining time from processes
      this->deductRemainingTime(cpu_time_per_cycle);

      // printing all processes
      this->print();

      this->checkAndRemoveCompletedProcesses();

      if(cycle_counter == 2) {
        string name = "P4";
        int time = 12;
        // adding new process
        this->addProcess("P4" , 12);
        cout << endl <<  "Added Process: " << name << " Remaining time: " << time;
      }

      cycle_counter++; //incrementing the counter
    }
    cout << "\nAll Processes Completed" << endl; // Printing the completion message







  }




};


int main() {
  Scheduler schedular;

  schedular.addProcess("P1" , 10);
  schedular.addProcess("P2" , 5);
  schedular.addProcess("P3" , 8);


  schedular.cycleProcesses();



}
