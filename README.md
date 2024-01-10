Purpose:
The purpose of this project is to use what we learned in this entire class relating to PWMs, sensors, motors, and various other tools, including MATLAB GUI, and combine them into one project.
Discussion:
First, I had to measure the angle limits of the ultrasonic sensor, I did this incrementally in 5 degrees. For the horizontal plot, I was not able to get a good reading, but for vertical I got from -45 degrees to 45 degrees. Plots are below.

I had to make large changes to the previous project’s code. I had to add functionality for a temperature sensor, RFID scanner, IR remote, and an LCD display. 
I also had to add interlocks for when the temperature goes too high or for when the coolant level is too low. Unfortunately, my water level sensor was defective, so I had to edit that part out of my code for it to still work.
Something else that gave me a lot of trouble, but I was able to fix was libraries using the same PWM timer. Even though I didn’t activate any timers, the timer that was used for the buzzer as well as the timer for the IR receiver conflicted (timer 2), making it so my IR receiver was not working. To fix this, I began my IR receiver right after I was done using my buzzer in the code, which meant they were no longer sharing the same timer.
 
The budget for the total current being used by my design is as follows:
•	Total Available from Arduino: 50mA
•	LCD display: 0.4mA
•	Motor controller / Motor(max): 40mA – given to by the motor power supply.
•	Temperature sensor: 50uA
•	Ultrasonic sensor: 15mA
•	IR receiver: 0.3mA
•	RFID scanner (max): 26mA 
•	Power supply module: 700mA
•	Total current left over: 668.25mA

I also had to make a GUI that would continuously update and display the sensor values coming from the car, as well as the headlight status and speed percentage via serial print. I did this by sending over two bytes of a string to the Arduino, then the Arduino took those bytes and printed all the related data into the COM port separated with commas. My GUI then parsed out the data, made sure it was valid, and then updated the information on the GUI. This process repeats until you exit the program.

Conclusion
I was able to create an Arduino based vehicle using a multitude of sensors and devices using everything I learned in this course. Using MATLAB, I was able to create a UI that was able to take information from the Arduino through the COM ports, parse, and display the data and be able to manipulate it. The only thing I am missing is the water level sensor, and that component simply did not work, even using the example code to use it. Another thing I would say is that I need better wire management. I ran into a lot of errors and made a lot of mistakes completing this project, but ultimately, I was able to finish it. 