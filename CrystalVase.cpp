// Kevin Alfonso
// COP4520

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>

#define NUM_GUESTS 100

int generateRandomNumber(int, int);
void viewVase(int);

// Keep track of if a guest has viewed the vase yet
std::vector<bool> guestsPicked(NUM_GUESTS);
int numGuestsViewed = 0;
bool isRoomAvailable = true;

// Avoid race conditions
std::mutex mutex;

// Current guest in the room
int guestInRoom = generateRandomNumber(0, NUM_GUESTS - 1);

// Random integer generator, inclusive of min and max
// Used for randomly choosing next guest
int generateRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(mt);
}

void viewVase(int currentGuest)
{
    while (numGuestsViewed < NUM_GUESTS)
    {
        mutex.lock();

        if (currentGuest == guestInRoom && isRoomAvailable)
        {
            isRoomAvailable = false;

            // Simulate viewing the vase (even if for short amount of time)
            int viewingTime = generateRandomNumber(10, 50);
            std::this_thread::sleep_for(std::chrono::milliseconds(viewingTime));

            isRoomAvailable = true;

            // Check if this is the first time the guest has viewed the vase
            if (!guestsPicked[currentGuest])
            {
                numGuestsViewed++;
                guestsPicked[currentGuest] = true;
                std::cout << "Guest " << currentGuest << " viewed the vase for the first time" << std::endl;
            }
        }

        mutex.unlock();
    }
}

int main(void)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads(NUM_GUESTS);

    // For this problem, every thread/guest is the same
    for (int i = 0; i < NUM_GUESTS; i++)
    {
        threads[i] = std::thread(viewVase, i);
    }

    // Keep choosing new people until everyone has been chosen
    while (numGuestsViewed < NUM_GUESTS)
    {
        guestInRoom = generateRandomNumber(0, NUM_GUESTS - 1);
    }
    
    // Wait for all threads to finish
    for (auto& thread: threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << numGuestsViewed << " guests have viewed the vase" << std::endl;
    std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
    
    return 0;
}