# Firex

Firex is a firefighting robot created for Trinity College International Firefighting Robot Contest 2019 (https://trinityrobotcontest.org/) by 4 students from George School, PA under guidance of robotics teacher Chris: Robin (Maze traversal), Sam (Hardware and Fire extinguishing), Andrew (Hardware and Fire detection) and Ian (Hardware and Microphone).

This repository contains all 3 Arduino projects, each labeled with the level that the project has been implemented on the robot. However, the codes are currently disorganized, and the current version of the project was DISCONTINUED after the competition. So, description and written algorithms will be updated here.

## Maze traversing on Level 1 & 2

For both levels, the robot is expected to complete the level without arbitrary starting points, with returning to the starting point and without furniture.
Algorithm for the first two levels of the competition shares similar core ideas as follow:

Assuming that the robot is facing in the direction that the right wall exists.
The robot will follow the right wall until it searched 3 rooms, then it will follow the left wall instead after the robot made two 90-degree turns after exiting the third room searched. If it faces and going near the dog, the side will be switched again, then it will treat the dog as a wall. This algorithm is working on all possible maze and doll configurations (in the scope of the competition).

If the robot faces and going near the wall, the robot will turn to let the "wall following" side faces the wall (if following the right wall, then turn left 90 degrees, ...)

If the robot enters the room (by detecting the white tape using IR ground sensors), the robot will move past the tape for a few steps, and turn around for several times to search for the candle using IR sensors. If no candle is found, the robot will turn around and move forward to exit the room. Otherwise, the robot will moves toward the candle while keeping the candle directly in front using three IR sensors--if the left sensor detects more intense light than the center, then it will turn left a bit to make the central sensor detects most light and the left and right sensors balanced. If it gets close too the candle, it will stop and spread CO2 for several times and angles to extinguish the fire. After that, it will switch the wall following side to exit the room and return to the starting point. If it enters the room after the fire has been extinguished, the robot will turn around and exit the room without searching for the fire again.

There is a case on the third room (searched) where the returning won't work since there are 2 doors. We tried to detect that case by recording the angle that the robot detected the fire, but the implementation is still bugging and not work as expected. However, the robot somehow miraculously get back to the starting point in a pretty weird and unexpected way.

Since the starting point is a white circle, it could be detected using the same method as white tape detection. What makes the difference is the number of times white things have been detected by the sensors (if the fire was found in the first room, it will pass exactly 2 white things (tape 1 and starting circle 1) before stopping, ...)

However, the white things detection will crashes if level 2 carpets have whites on it. The method we used is to turn off the detector while the robot is walking on the places where the carpet is possible to be there (if I not misremembered).

## Balancing

Another issue is balancing. The imperfection of the physical stuff has made these ideas become idealistic, even it was created to suit the non-ideal environments. Therefore, small adjustments and balancing are needed. Some of them have to be hard-coded and make the code become more disorganized.

## Maze traversing on Level 3

We didn't intend to do this level in the first place. However, we decided to do it after seeing an opportunity to collect the smallest set of points by moving through the central hallway.

We didn't have much time at that point and develop an improvised algorithm that has a 50% success rate, depends on the starting point we got. 

That algorithm is dumb right wall following. That's right, just follow the right wall then it might (50%) move through the hallway.

## Final words

This description is written based on Robin's (the author of this repository) 2-month old memories. Mistakes are highly possible here. So I encourage you to visualize and try these ideas on paper (or some drawable spaces) to see if it works. You may contact me and other team members if you have any questions.

Also, the previous versions of the codes are available in the past commits of this repository.
