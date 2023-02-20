// Kevin Alfonso
// COP4520

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>

#define NUM_GUESTS 100

int generateRandomNumber(int, int);
void checkCupcake();
void visitLabyrinth(int);

// Keep track of if a guest has been picked yet
std::vector<bool> guestsPicked(NUM_GUESTS);
int numGuestsPicked = 0;
bool isCupcakeThere = true;

// Avoid race conditions
std::mutex mutex;

// Current guest in the labyrinth
// Initialized to first guest picked
int activeThreadID = generateRandomNumber(0, NUM_GUESTS - 1);
// That first person picked will be the manager
int managerID = activeThreadID;

// Random integer generator, inclusive of min and max
// Used for randomly choosing next guest
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
        if (activeThreadID == managerID)
        {
            // Account for first person being chosen not eating yet
            // They eat and immediately replace cupcake
            if (isCupcakeThere && !guestsPicked[managerID])
            {
                numGuestsPicked++;
                guestsPicked[managerID] = true;
                std::cout << "Guest " << managerID << " ate the cupcake" << std::endl;
            }
            // Cupcake will only be eaten if it was someone's first time eating it
            // Manager must update count and replace cupcake
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

        // If cupcake is there and current guest has not eaten it before, eat it
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

    // Create thread for every guest
    std::vector<std::thread> threads(NUM_GUESTS);

    // Initialize chosen manager as the cupcake checker
    std::cout << "Guest " << managerID << " is the manager" << std::endl;
    threads[managerID] = std::thread(checkCupcake);

    // Every other thread/guest will be a normal visitor
    for (int i = 0; i < NUM_GUESTS; i++)
    {
        if (i == managerID)
        {
            continue;
        }
        threads[i] = std::thread(visitLabyrinth, i);
    }

    // Keep choosing new people until everyone has been chosen
    while (numGuestsPicked < NUM_GUESTS)
    {
        activeThreadID = generateRandomNumber(0, NUM_GUESTS - 1);
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