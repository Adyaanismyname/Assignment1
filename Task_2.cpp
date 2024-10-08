#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Node {
public:
    long long int value;
    Node* next;
    Node(long long int value) : value(value), next(nullptr) {}
};

class NodeList {
public:
    Node* head;
    Node* tail;

    // constructor

    NodeList() : head(nullptr), tail(nullptr) {}


    // a function that inserts a node at the end of the list
    void insert(long long int value) {

        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    // print the whole list
    void print() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }


    // function for adding two node list numbers
    NodeList add(NodeList& other) {
        NodeList result;
        Node* current1 = this->head;
        Node* current2 = other.head;
        long long carry = 0;

        while (current1 != nullptr || current2 != nullptr || carry) {
            long long sum = carry;
            if (current1) {
                sum += current1->value;
                current1 = current1->next;
            }
            if (current2) {
                sum += current2->value;
                current2 = current2->next;
            }
            carry = sum / 10000000000000000LL;
            result.insert(sum % 10000000000000000LL);
        }

        return result;
    }

    // function for subtracting two node list numbers

    NodeList subtract(NodeList& other) {
        NodeList result;
        Node* current1 = this->head;
        Node* current2 = other.head;
        long long borrow = 0;



        while (current1 != nullptr || current2 != nullptr) {
            long long diff = (current1 ? current1->value : 0) - borrow;
            if (current2) {
                diff -= current2->value;
                current2 = current2->next;
            }
            borrow = 0;
            if (diff < 0) {
                diff += 10000000000000000LL;
                borrow = 1;
            }
            result.insert(diff);
            if (current1) current1 = current1->next;
        }

        return result;
    }

    // padding the number by adding zeros so that the higher(more significant) number can be combined with the lower to make the whole product
    NodeList shift(int m) {
        NodeList result;
        for (int i = 0; i < m; i++) {
            result.insert(0); // Shift by inserting zeroes
        }
        Node* current = this->head;
        while (current) {
            result.insert(current->value);
            current = current->next;
        }
        return result;
    }


// split the number into two parts
    void split(NodeList& list, NodeList& low, NodeList& high) {
        Node* slow = list.head;
        Node* fast = list.head;
        Node* prev = nullptr;

        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }

        if (prev) prev->next = nullptr; // Split the list
        low.head = list.head;
        high.head = slow;
    }


    // multiplication of two nodes(two numbers)
    NodeList karatsuba_Multiply(NodeList& other) {
      // if there is just one node  then just do basic multiplication
        if (this->head->next == nullptr || other.head->next == nullptr) {
            long long product = this->head->value * other.head->value;
            NodeList result;
            result.insert(product);
            return result;
        }

        // Split the numbers into 2  halves
        NodeList X0, X1, Y0, Y1;
        split(*this, X0, X1);
        split(other, Y0, Y1);

        // temporary nodes
        NodeList tempX = X1.add(X0);
        NodeList tempY = Y1.add(Y0);

        // Use recursion to compute the 3 smaller products
        NodeList Z2 = X1.karatsuba_Multiply(Y1);
        NodeList Z0 = X0.karatsuba_Multiply(Y0);
        NodeList Z1 = (X1.add(X0)).karatsuba_Multiply(tempY).subtract(Z2).subtract(Z0);
        NodeList tempZ = Z1.shift(1);



        // Combine the results to form the product
        return Z2.shift(2).add(tempZ).add(Z0);
    }


    pair<NodeList, long long> Division(NodeList& divisorList) {
        NodeList quotient;
        long long remainderValue = 0; // Keep a single long long for remainder

        Node* currentDividend = this->head;
        long long divisorValue = divisorList.head->value;

        while (currentDividend != nullptr) {
            // Combine the current segment to the remainder
            long long combinedValue = remainderValue * 10000000000000000LL + currentDividend->value;

            // Check for potential overflow here, but since we're dealing with long long, it should be okay
            long long currentQuotient = combinedValue / divisorValue; // Integer division
            quotient.insert(currentQuotient); // Store the quotient

            // Update the remainder using the current quotient
            remainderValue = combinedValue % divisorValue;

            currentDividend = currentDividend->next;
        }

        // Normalize the quotient if needed
        // Handle any leading zeros in the quotient
        if (quotient.head == nullptr) {
            quotient.insert(0); // Insert zero if there were no valid divisions
        }

        return {quotient, remainderValue};
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

    string createStringFromNodeList() {
        string result;
        Node* current = this->head;
        while (current!= nullptr) {
          // appending to the string
            result += to_string(current->value);
            current = current->next;
        }

        return result;
    }


    bool isPrime() {
        NodeList i = createNodeListFromString("2");
        NodeList iterator = createNodeListFromString("1");


        while((i.karatsuba_Multiply(i)).createStringFromNodeList().length() != this->createStringFromNodeList().length()) { // this while loop checks if i^2 has length less than the length of the original number
            auto [qoutient , remainder] = this->Division(i);

            if(remainder == 0) {
                return false;
            }


            i = i.add(iterator);
        }

        return true;


    }

};

int main() {
    NodeList list1, list2;

    string num1 = "864297510284736918235647102938475609182374650192837465091283746509182374650918237465091283746509182374650918273465091283746591827364509128374651092837465091283746509182374650928374650192837465091823746501928374650192837465091283746509182374650192837";

    list1 = list1.createNodeListFromString(num1);



    if(list1.isPrime()) {
        cout << "The number is prime." << endl;
    }
    else {
        cout << "The number is not prime." << endl;
    }


}
