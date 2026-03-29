z# <p1>MicroOS</p1>
<img width="1080" height="1080" alt="micro os logo gimp" src="https://github.com/user-attachments/assets/07d2af49-5dbb-4661-bc4c-820b6020a2b7" />





 <br>
------------------------------------------------------<br>
MicroOS is a minimal project and educational OS made for maximum compatibility. It can run on both old and new CPU's powered by the X86 assembly instruction set for maximum compatibility and flexibility. It is mainly made for education and to be a sweet asset on my portfolio and i don't plan on making it a full mainstream OS. 

<br><br><br>

<h2> Stages: </h2>
<h3> 1, Bootloader to load the further stages. This is at adress 0x7C00 because of the intel 8086, This was kept like that and its still the norm today.</h3>
<h3> 2, Extrabootloader (Assembly kernel terminal in microOS).</h3>
<h3> 3, Raw instructions for entering the fourth stage.</h3>
<h3> 4, C kernel terminal.</h3>

<br><br><br>

Run this to test it in a VM:
(First make sure **qemu** is installed on your testing system I highly recomend testing this on WSL or Linux. But you can also run it on windows if youre feeling a bit jolly)

```chmod +x test.sh ``` <br>
```./test.sh```


<img width="697" height="404" alt="image" src="https://github.com/user-attachments/assets/de8a5366-2370-4b81-87af-4489343ce099" /><br><br>


IF you see this ^, Then be sure to type the command:
```
continue
```
OR you can go into kernel.asm and turn <b>"StartAtC db 0" into "StartAtC db 1"</b> To make it boot into the C kernel automatically without typing the "continue" command. You can toggle this whenever you want.

<br>
After using the **continue** command, You should be seeing this:<br><br>
<img width="710" height="390" alt="image" src="https://github.com/user-attachments/assets/45c5107f-a3b6-441d-b79d-25ddaded3d7c" />



Now, Try using the: **help** command to see every command available. 
If youre interested for the assembly kernel commands, Theyre pretty limited but they're:

---------

Is this kind of like dos? Yes. it is kinda like dos. But i do think it is cool. And i really like cool things like this, But eventually i will probably make GUI and advanced drivers and support for lots of devices. If you want to see this grow, Then give this project a star and a watch if youre really jolly about this, I'm not sure about investing but my discord profile is: @salamidev 
So feel free to DM me to go into the further advanced options. 

If youre a good programmer and want to help the project out (You could be the one of the first 10 contributors if this blows up) If you want to, I'm willing to happily accept contributions. I don't know how they work yet tho but i'll figure it out

---------

<br><br><br>

# Contributing to microOS.

Feel free to send out contribution request whenever you want. I'm sure this wont get as popular as the linux kernel because the linux kernel is just too awesome but if this repo even gets 100 stars. It would mean the world.

---------

<br><br><br>

# How to get to the assembly kernel?

The assembly kernel terminal is the first stage of the whole OS past the bootloader, Technically its still not even in the OS. To access it, Open "kernel.asm" and find this line: ```StartAtC db 1```. After finding it, Edit "1" to be "0", Which skips the part where it automatically goes to the C kernel and stays in assembly. You can toggle it whenever you want, it doesn't remove anything.

<img width="729" height="92" alt="image" src="https://github.com/user-attachments/assets/5fc8c11d-8e81-4f36-9606-cb339dbd3ef0" />

<b> ^, Remove the "1" and replace it with "0", You can undo this whenever you want.</b>

And after you turn it to 0:

<img width="200" height="44" alt="image" src="https://github.com/user-attachments/assets/f7e42f83-0686-453b-bcb2-097e311cde6c" />

You get here:

<img width="697" height="404" alt="image" src="https://github.com/user-attachments/assets/de8a5366-2370-4b81-87af-4489343ce099" /><br><br>

---------

<br><br><br><br>

# Thanks!!

<p3> If youre willingly on this repo. Then i would like to massively thank you. Even a few people knowing and acknowledging this would make me very happy. </p3>

Think of this as a learning OS that you can read the code from to learn. i made the code specifically to be very easy to look and observe for beginners. I am looking forward to making this bigger and making this something real in my portfolio. 

If you want to download this for Personal/Commercial use: <b>Just do it, But make sure to credit me so i can get fame as a currency.</b>

<b> Note: I'm pretty young and a minor. So please do not do stuff that is in harm of me because that would just be a sad move for you and me. </b>

Have fun using this as an educational OS, Show this to your friends or learn from the code. You can learn how to make an assembly bootloader and simple kernel and drivers from this. (<b>X86</b>)

---------

# Why i deleted and re uploaded the repo:
I realized some commits exposed personal information. And github's dilly dally did not let me remove the history of them. So i was forced to delete and re upload the whole repo. This repo was made about a month ago from when i reuploaded the repo. [2026-03-23].
