// Kevin Alfonso
// COP4520

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>
#include <vector>
#include <algorithm>
#include <unordered_set>

#define NUM_GUESTS 100

std::unordered_set<std::thread::id> guestsViewed;
std::mutex mutex;
bool isRoomAvailable = true;
int activeThreadID;

int generateRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(mt);
}

void viewVase(int currentThreadID)
{
    std::thread::id currentGuest = std::this_thread::get_id();
    while (guestsViewed.size() < NUM_GUESTS)
    {
        mutex.lock();

        // if (activeThreadID == currentThreadID && isRoomAvailable && std::find(guestsViewed.begin(), guestsViewed.end(), currentGuest) == guestsViewed.end())
        if (activeThreadID == currentThreadID && isRoomAvailable)
        {
            isRoomAvailable = false;
            int viewingTime = generateRandomNumber(10, 100);
            // std::cout << "Guest " << currentThreadID << " is viewing the vase" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(viewingTime));
            isRoomAvailable = true;

            if (std::find(guestsViewed.begin(), guestsViewed.end(), currentGuest) == guestsViewed.end())
            {
                guestsViewed.insert(currentGuest);
                std::cout << "Guest " << currentThreadID << " is viewing the vase for the first time" << std::endl;
            }
        }

        mutex.unlock();
    }
}

int main(void)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads(NUM_GUESTS);

    for (int i = 0; i < NUM_GUESTS; i++)
    {
        threads[i] = std::thread(viewVase, i);
    }

    // Keep choosing new people until everyone has been chosen
    while (guestsViewed.size() < NUM_GUESTS)
    {
        activeThreadID = generateRandomNumber(0, NUM_GUESTS);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << guestsViewed.size() << " guests have viewed the vase" << std::endl;
    std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
    
    return 0;
}