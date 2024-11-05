#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>      // For this_thread::sleep_for
#include <chrono>      // For chrono::milliseconds

using namespace std;

bool simulatedSearchRequest() {
    // 30% chance of success
    return rand() % 10 < 3;
}


bool exponentialBackoffSearch(int maxRetries, int initialWaitTime) {
    int retries = 0;
    int waitTime = initialWaitTime; // in milliseconds

    while (retries < maxRetries) {
        cout << "Attempt " << retries + 1 << " of search..." << endl;

        if (simulatedSearchRequest()) {
            cout << "Search successful!" << endl;
            return true;
        }

        // If the search fails, increase the wait time and retry
        cout << "Search failed. Retrying in " << waitTime << " ms..." << endl;
        this_thread::sleep_for(chrono::milliseconds(waitTime));

        // double the wait time
        waitTime *= 2;
        retries++;
    }

    cout << "Search failed after " << maxRetries << " retries." << endl;
    return false;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    int maxRetries, initialWaitTime;
    cout << "Enter the maximum number of retries: ";
    cin >> maxRetries;
    cout << "Enter the initial wait time in milliseconds: ";
    cin >> initialWaitTime;

    bool result = exponentialBackoffSearch(maxRetries, initialWaitTime);

    if (!result) {
        cout << "The search operation could not be completed successfully." << endl;
    }

    return 0;
}
