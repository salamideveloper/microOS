# <p1>MicroOS</p1>
MicroOS is a minimal project and educational OS made for maximum compatibility. It can run on both old and new CPU's powered by the X86 assembly instruction set for maximum compatibility and flexibility. It is mainly made for education and to be a sweet asset on my portfolio and i don't plan on making it a full mainstream OS. 

Run this to test it in a VM:
(First make sure **qemu** is installed on your testing system I highly recomend testing this on WSL or Linux. But you can also run it on windows if youre feeling a bit jolly)

```chmod +x test.sh ``` <br>
```./test.sh```


<img width="713" height="337" alt="image" src="https://github.com/user-attachments/assets/9e4f90f4-5372-4d5f-b3bb-d521671e836c" /><br><br>

You are now in the **Assembly kernel**. Which doesn't have as many features. So to go to the real kernel you need to put in this command:
```
continue
```
<br>
After using the **continue** command, You should be seeing this:<br><br>
<img width="710" height="390" alt="image" src="https://github.com/user-attachments/assets/45c5107f-a3b6-441d-b79d-25ddaded3d7c" />



Now, Try using the: **help** command to see every command available. 
If youre interested for the assembly kernel commands, Theyre pretty limited but they're:

<details>
  <summary>Assembly kernel commands: </summary>
  <p>tinyhello | Prints out a little surprise. This is a test command.</p>
  <p>clear | Clears your screen</p>
  <p>echo <args> | Prints out whatever you typed after <b>echo</b>.</p>
  <p>loaddriver <args> <args> | Loads a driver </p>
  <p>Shutdown | Powers the CPU off.</p>
  <p>Microos_Driver_Test | Loads a testing driver to see if they work properly.</p>
  <p>chartest | Lists out every character supported by your BIOS graphics.</p>
  <p>continue | Goes to the C kernel, <b>Its recommended that you do this.</b></p>
</details>

# Contributing to microOS.

Feel free to send out contribution request whenever you want. I'm sure this wont get as popular as the linux kernel because the linux kernel is just too awesome but if this repo even gets 100 stars. It would mean the world.

# How to get to the assembly kernel?

The assembly kernel terminal is the first kernel i had ever written in my life. To acces it, Open "kernel.asm" and find this line: ```StartAtC db 1```. After finding it, Edit "1" to be "0", Which skips the part where it automatically goes to the C kernel and stays in assembly. You can toggle it whenever you want, it doesn't remove anything.

# Thanks!!

<p3> If youre willingly on this repo. Then i would like to massively thank you. Even a few people knowing and acknowledging this would make me very happy. </p3>

Think of this as a learning OS that you can read the code from to learn. i made the code specifically to be very easy to look and observe for beginners. I am looking forward to making this bigger and making this something real in my portfolio. 

If you want to download this for Personal/Commercial use: <b>Just do it, But make sure to credit me so i can get fame as a currency.</b>

<b> Note: I'm pretty young and a minor. So please do not do stuff that is in harm of me because that would just be a sad move for you and me. </b>

Have fun using this as an educational OS, Show this to your friends or learn from the code. You can learn how to make an assembly bootloader and simple kernel and drivers from this. (<b>X86</b>)

# Why i deleted and re uploaded the repo:
I realized some commits exposed personal information. And githubs dipshit ass did not let me remove the history of them. So i was forced to delete and re upload the whole repo. This repo was made about a month ago from when i reuploaded the repo. [2026-03-23].
