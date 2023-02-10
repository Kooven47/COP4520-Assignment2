// Kevin Alfonso
// COP4520

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>

#define NUM_GUESTS 100

std::vector<bool> guestsPicked(NUM_GUESTS);
std::mutex mutex;
bool isCupcakeThere = true;
int numGuestsPicked = 0;
int activeThreadID = 0;

int generateRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(mt);
}

void checkCupcake()
{
    while (numGuestsPicked < NUM_GUESTS)
    {
        mutex.lock();

        // First person chosen is the "manager"
        if (activeThreadID == 0)
        {
            // Account for first person being chosen not eating yet
            // They eat and immediately replace cupcake
            if (isCupcakeThere && !guestsPicked[0])
            {
                numGuestsPicked++;
                guestsPicked[activeThreadID] = true;
                std::cout << "Guest 0 ate the cupcake" << std::endl;
            }
            // Cupcake only there if someone's first time eating it
            // Update count and replace cupcake
            if (!isCupcakeThere)
            {
                numGuestsPicked++;
                isCupcakeThere = true;
            }
        }

        mutex.unlock();
    }
}

void visitLabyrinth(int currentThreadID)
{
    while (numGuestsPicked < NUM_GUESTS)
    {
        mutex.lock();

        // If cupcake is there and we haven't eaten it before, eat it
        if (activeThreadID == currentThreadID && isCupcakeThere && !guestsPicked[currentThreadID])
        {
            guestsPicked[activeThreadID] = true;
            isCupcakeThere = false;
            std::cout << "Guest " << currentThreadID << " ate the cupcake" << std::endl;
        }

        mutex.unlock();
    }
}

int main(void)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads(NUM_GUESTS);

    // First thread is the "manager"
    threads[0] = std::thread(checkCupcake);

    for (int i = 1; i < NUM_GUESTS; i++)
    {
        threads[i] = std::thread(visitLabyrinth, i);
    }

    // Keep choosing new people until everyone has been chosen
    while (numGuestsPicked < NUM_GUESTS)
    {
        activeThreadID = generateRandomNumber(0, NUM_GUESTS);
    }

    // Wait for all threads to finish
    for (auto& thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << numGuestsPicked << " guests picked" << std::endl;
    std::cout << "Time taken: " << duration << " milliseconds" << std::endl;
    
    return 0;
}