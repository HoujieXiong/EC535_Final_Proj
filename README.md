# EC535_Final_Proj
EC535 Final Project
 
Smart Garden System 
Technical Report


Houjie Xiong xhj@bu.edu
Xiang Jin xjin1@bu.edu



	

Abstract: 
This project presents a smart garden system that automates plant care and offers remote monitoring. It continuously monitors temperature and humidity, regulating the heating and cooling cycles according to preset growing conditions. An image of the plants are taken and streamed on a local web server periodically so that users can check up on the physical welfare of their plants. Various areas that can be ventured to improve the experiences of the users include implementing a watering system that can automatically water the plants are preset times, as well as allowing for port forwarding so that the web server can be accessed outside the local network, allowing for truly remote, real-time monitoring from anywhere that has Internet access. Building the project required first building the wooden frame, then developing the software on the BeagleBone Black and the Raspberry Pi 5, both embedded Linux devices used for this project. Results showed that the TMP36 temperature sensors had lots of noise of a couple of degrees Celsius in either direction, from the accepted temperature value, but this may be acceptable depending on the plants being used. The image feed had a latency that was not noticeable to the human user, and is more than what is required to make sure that there were enough updates on the visual properties of the plants being monitored. Overall improvements could include better insulation for the enclosure, more stable temperature sensors as well as a video stream instead of an image feed that can be accessed by any device on the Internet for ease of plant monitoring.


Link to Video Demo: https://drive.google.com/file/d/11G9czH57KOBptVe-_WRcRWMBcMGLRPSH/view?usp=sharing
