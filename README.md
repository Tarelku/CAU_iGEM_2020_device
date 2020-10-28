# CAU_iGEM_2020_device
This is code for our optogenetic autometed control system device this year.  
Hope this will help anyone who wants to build their own fermentation device.

## How to use this code
For green hands, first you need to buy an arduino or any other hardware which can run this code. After this, you need to design your own device and assemble it (you can find many of those "how to do" webs using google).   
I've list Pinouts in this picture, you can change them or just inserts wires in corresponding Pinouts.  
![](https://github.com/Tarelku/CAU_iGEM_2020_device/blob/main/img/hubs.jpg)  
You can find our device description in our iGEM wiki page: https://2020.igem.org/Team:CAU_China

`
You need to restart (shut the device down) to reset NTU[] counter and green light 
if you use this code because I forgot to design a reset button :(
`

## Some details
You may need to draw a picture use Excel (like this image) to find relationship between OD620 and values you received from sensor, the relationship could be strongly effected by light intensity of your LED and shape of your fermentation container. Please make sure you have got a right function.
![](https://github.com/Tarelku/CAU_iGEM_2020_device/blob/main/img/Relationship%20between%20OD620%20and%20sensor%20value.png)

## Notice
iGEM_Program_with_port.ino used serial. , which can print values to the port, **but also can cause some unexpected errors such as unstable working frequncy and slow response.**   
If you met those errors, try to delete all serial. commands or use iGEM_Program_without_port,ino.
