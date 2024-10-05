#include <iostream>
#include <string>
using namespace std;

class Node {
public:
    long long value; // Use long long for 64-bit segment
    Node* next;
    Node(long long value) : value(value), next(nullptr) {}
};

class NodeList {
public:
    Node* head;
    Node* tail;

    NodeList() : head(nullptr), tail(nullptr) {}

    void insert(long long value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void print() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }

    NodeList createNodeListFromString(const string& largeNumber) {
        NodeList nodeList;
        int length = largeNumber.length();

        // Process the string in chunks of 16 digits
        for (int i = 0; i < length; i += 16) {
            string segment = largeNumber.substr(i, 16);
            long long value = stoll(segment); // Convert the segment to long long
            nodeList.insert(value);
        }

        return nodeList;
    }

    // Division of two NodeLists
    pair<NodeList, long long> Division(NodeList& divisorList) {
        NodeList quotient;
        long long remainderValue = 0; // Keep a single long long for remainder

        Node* currentDividend = this->head;
        long long divisorValue = divisorList.head->value;

        while (currentDividend != nullptr) {
            // Combine the current segment to the remainder
            long long combinedValue = remainderValue * 10000000000000000LL + currentDividend->value;

            // Calculate the quotient for the current combined value
            long long currentQuotient = combinedValue / divisorValue; // Integer division
            quotient.insert(currentQuotient); // Store the quotient

            // Update the remainder using the current quotient
            remainderValue = combinedValue % divisorValue; // Remainder after division

            currentDividend = currentDividend->next; // Move to the next segment
        }

        return {quotient, remainderValue}; // Return the resulting quotient and the remainder
    }

    // Function to check if the original number is prime
    bool isPrime(NodeList& originalNumber) {
        NodeList iList;
        Node* current = originalNumber.head;
        string originalString;

        // Build the original number string for length comparison
        while (current != nullptr) {
            originalString += to_string(current->value);
            current = current->next;
        }

        // Get the length of the original number
        int originalLength = originalString.length();

        long long i = 1; // Start with 1
        bool foundDivisor = false;

        while (to_string(i * 2).length() <= originalLength) {
            iList.insert(i); // Store current value of i in NodeList

            // Create a NodeList for divisor
            NodeList divisorList;
            divisorList.insert(i);

            // Perform division
            auto [quotient, remainder] = originalNumber.Division(divisorList);

            if (remainder == 0) {
                foundDivisor = true; // Found a divisor
                break;
            }

            i *= 2; // Double i
        }

        // If no divisor is found, conclude it is prime
        return !foundDivisor;
    }
};

int main() {
    NodeList originalNumber;

    // Read the large number from user input
    string num1 = "78945612301234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";


    originalNumber = originalNumber.createNodeListFromString(num1);

    cout << "Original Number: ";
    originalNumber.print();

    // Check if the number is prime
    bool prime = originalNumber.isPrime(originalNumber);
    if (prime) {
        cout << "The number is prime." << endl;
    } else {
        cout << "The number is not prime." << endl;
    }

    return 0;
}
