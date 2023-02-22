# COP4520 Assignment 2
This project is a C++ implementation using std::thread of the problems below.

## Problem 1: Minotaur’s Birthday Party (50 points) 
The Minotaur invited N guests to his birthday party. When the guests arrived, he made 
the following announcement. 
The guests may enter his labyrinth, one at a time and only when he invites them to do 
so. At the end of the labyrinth, the Minotaur placed a birthday cupcake on a plate. When 
a guest finds a way out of the labyrinth, he or she may decide to eat the birthday 
cupcake or leave it. If the cupcake is eaten by the previous guest, the next guest will find 
the cupcake plate empty and may request another cupcake by asking the Minotaur’s 
servants. When the servants bring a new cupcake the guest may decide to eat it or leave 
it on the plate. 
The Minotaur’s only request for each guest is to not talk to the other guests about her or 
his visit to the labyrinth after the game has started. The guests are allowed to come up 
with a strategy prior to the beginning of the game. There are many birthday cupcakes, so 
the Minotaur may pick the same guests multiple times and ask them to enter the 
labyrinth. Before the party is over, the Minotaur wants to know if all of his guests have 
had the chance to enter his labyrinth. To do so, the guests must announce that they have 
all visited the labyrinth at least once. 
Now the guests must come up with a strategy to let the Minotaur know that every guest 
entered the Minotaur’s labyrinth. It is known that there is already a birthday cupcake left 
at the labyrinth’s exit at the start of the game. How would the guests do this and not 
disappoint his generous and a bit temperamental host? 
Create a program to simulate the winning strategy (protocol) where each guest is 
represented by one running thread. In your program you can choose a concrete number 
for N or ask the user to specify N at the start.

## Compilation Instructions
g++ -o BirthdayParty -pthread BirthdayParty.cpp
<br> ./BirthdayParty

## Proof of Correctness, Efficiency, and Experimental Evaluation
My approach:
Create N (in my case, N = 100) threads, with each thread representing a guest. 
We will keep randomly choosing guests to enter the labyrinth. The first guest chosen will be assigned as the manager. Their responsibility will be to keep the counter in their head of how many guests have eaten the cupcake. Every guest's responsibility (including the manager) will be to remember if they have eaten a cupcake before, and if not, eat it when they get picked if one is available. When the manager is picked, if the cupcake is not available, they will update their counter (as this means someone who has not eaten the cupcake before, has) and replace the cupcake for the next unique quest to eat. Once the manager has updated their count to reach N, the program ends.

For RNG, I used the Mersenne Twister algorithm due to [being faster, not compiler dependent, and less prone to bias compared to rand()](https://codeforces.com/blog/entry/61587?locale=en).

Runtimes averaged over ten trials
<br> For N = 10: 5.6ms
<br> For N = 20: 34ms
<br> For N = 50: 486.6ms
<br> For N = 100: 3419.2ms
<br> For N = 250: 50215.7ms
<br> The time taken between each guest was mostly consistent throughout the whole program, and the range of times taken throughout the ten trials was small.
The correctness was shown in class as this is very similar to one of the prisoner problems we went over during lecture.

## Problem 2: Minotaur’s Crystal Vase (50 points) 
The Minotaur decided to show his favorite crystal vase to his guests in a dedicated 
showroom with a single door. He did not want many guests to gather around the vase 
and accidentally break it. For this reason, he would allow only one guest at a time into 
the showroom. He asked his guests to choose from one of three possible strategies for 
viewing the Minotaur’s favorite crystal vase: 
1) Any guest could stop by and check whether the showroom’s door is open at any time 
and try to enter the room. While this would allow the guests to roam around the castle 
and enjoy the party, this strategy may also cause large crowds of eager guests to gather 
around the door. A particular guest wanting to see the vase would also have no 
guarantee that she or he will be able to do so and when. 
2) The Minotaur’s second strategy allowed the guests to place a sign on the door 
indicating when the showroom is available. The sign would read “AVAILABLE” or 
“BUSY.” Every guest is responsible to set the sign to “BUSY” when entering the 
showroom and back to “AVAILABLE” upon exit. That way guests would not bother trying 
to go to the showroom if it is not available. 
3) The third strategy would allow the quests to line in a queue. Every guest exiting the 
room was responsible to notify the guest standing in front of the queue that the 
showroom is available. Guests were allowed to queue multiple times. 
Which of these three strategies should the guests choose? Please discuss the advantages 
and disadvantages. 
Implement the strategy/protocol of your choice where each guest is represented by 1 
running thread. You can choose a concrete number for the number of guests or ask the 
user to specify it at the start.

## Compilation Instructions
g++ -o CrystalVase -pthread CrystalVase.cpp
<br> ./CrystalVase

## Proof of Correctness, Efficiency, and Experimental Evaluation
My approach: I used strategy 2, with a boolean value representing the sign. The advantages are ease of code compared to strategy 3, ordered/organized viewing of the vase compared to strategy 1, and wasting less of the guests time since they know when the viewing room is not available. The disadvantages are that guests can go into the showroom multiple times and guests don't know when they will be able to view the vase (compared to strategy 3 which does). Guests (which are represented by a thread) are randomly chosen to view the vase, marking the room as unavailable while they view the vase for a random number of time. After viewing it for the first time, that guest is marked as having seen the vase, with a counter variable increasing. The program terminates once the counter reaches N, similar to the previous problem just without one of the threads being a counter.

I used the same RNG for this problem as the birthday party problem.

Runtimes averaged over ten trials
<br> For N = 10: 829ms
<br> For N = 20: 2143.1ms
<br> For N = 50: 6094.7ms
<br> For N = 100: 18931.4ms
<br> For N = 250: 45842.6ms
<br> The time taken between each guest was not consistent throughout the whole program, as it took significantly longer time as the program progressed and unique guests were less likely to show up at the vase. The range of times taken throughout the ten trials was large for all N, especially compared to the birthday party problem. Additionally this program took significantly longer to terminate compared to the birthday party problem except for N = 250.

The correctness of this problem can be shown by its incredible similarity to the previous problem.
