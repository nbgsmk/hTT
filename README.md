
hTT - hex Terminal Tester
=========================

1 First install
---------------

- Connect to the remote machine using ip address or host name  
- clone repo  
- build the executable

	ssh pi@zero.local
	or
	ssh pi@192.168.....
	
	$ git clone https://github.com/nbgsmk/hTT.git		// clone the repository
	$ ls -l 						// check folder
	...
	drwxrwxr-x 1 user user 72 јул  4 20:58 hTT		// look for the repository name
	
	$ cd hTT/src						// enter the source folder
	$ pwd							// check where we are
	/home/pi/hTT/src					// expected result something like this
	
	$ g++ *.cpp -o hTT					// compile the application
	// this may take a while
	
	$ ls -la						// check the result
	...
	-rwxrwxr-x 1 user user 692088 јул  4 21:42 hTT		// and verify the executable exists

**NOTES on g++**


##### 1 - If compiler g++ is not installed, the error message will be like  

	Command 'g++' not found...

So try to install it

	$ sudo apt-get install build-essential			
	
Then repeat the command 'g++ ...' as described above



#####  2 - Compilation errors mentioning 'std::thread::thread'...  
Depending on the running OS version and architecture, try adding some combination of flags: -lpthread -pthread -static

	$ g++ *.cpp -lpthread -o hTT		// most of the time this is enough
	or
	$ g++ *.cpp -pthread -o hTT			// it depends...
	or
	$ g++ *.cpp -static -o hTT			// most of the time not needed
	
	or a combination of those

3 - If the compiled executable crashes on start, try a different combunation of above flags and run again.




2 Updating
----------

If hTT is already installed, and you need to update it from github, the steps are:

- connect to the remote machine
- enter the folder of cloned repo
- git pull
- compile the code

	ssh pi@zero.local				// again, connect, eg with ssh
	
	$ pwd							// check where we are
	/home/pi						// should be your home folder
	
	$ ls -l						// check the contents
	...
	drwxrwxr-x 1 user user 72 јул  4 20:58 hTT		// look for the name of previously cloned repo
	
	$ cd hTT						// enter the 'hTT' folder. Do _not_ go into 'hTT/src'
	/home/pi/hTT					// you should be here
	$ git pull						// pull updates from github to local machine
	
Application code is updated to the latest version.  
Compile with g++ as explained earlier.




3 Using hTT
-----------

Check available command line options: 

	$ ./hTT --help						// --help or -?
	$ cd /home/pi/hTT/src				// go to folder where hTT executable is
	
	$ ./hTT -d /dev/ttyS0 -b 9600 -a	// run in ascii mode
	or
	$ ./hTT -d /dev/ttyS0 -b 9600 -h	// run in hex mode

where:  
-d = specify serial device eg /dev/ttyS0 or /dev/ttyUSB4 or other  
-b = serial port speed  
-a = for testing ascii devices (eg. modems with AT commands)  
-h = for testing hex devices (eg sensors, communicating in binary mode only)  

### 3.1 HEX mode

Run hTT and enter hex codes, separated with spaces.  
Enter commands, one line at the time, press ENTER to send.  

For example: you type "5a 4f 4b 49" and press Enter. That is all.  
Reply will be shown just below.  

Exit the app with a single dot '.' in a new line, or with Ctrl-C


	$ ./hTT -d /dev/ttyUSB3 -b 115200 -h
	
	41 54 0d						// 'AT' and <newline> as hex characters. Press ENTER to send it
	hex str=  41  54  0d			// display what is sent in hex
	hex2int=  65  84  13			// display what is sent in decimal
	  ascii=   A   T    			// display what is sent in ascii
	
	(6) chars						// received 6 character response
	hex: 0d 0a 4f 4b 0d 0a 		// hex characters received
	ascii:      O  K  	     	// ascii equivalent, if any
	
	41 54 2b 43 47 4d 4d 0d							// entered command
	hex str=  41  54  2b  43  47  4d  4d  0d		// hex representation
	hex2int=  65  84  43  67  71  77  77  13		// decimal representation
	  ascii=   A   T   +   C   G   M   M    		// ascii representation
	
	
	// received response
	(27) chars
	  hex: 0d 0a 53 49 4d 43 4f 4d 5f 53 49 4d 37 36 30 30 45 2d 48 0d 0a 0d 0a 4f 4b 0d 0a 
	ascii:        S  I  M  C  O  M  _  S  I  M  7  6  0  0  E  -  H              O  K       
	
	// another example response
	(32) chars
	hex: 0d 0a 2b 43 4d 45 20 45 52 52 4f 52 3a 20 53 49 4d 20 6e 6f 74 20 69 6e 73 65 72 74 65 64 0d 0a 
	ascii:      +  C  M  E     E  R  R  O  R  :     S  I  M     n  o  t     i  n  s  e  r  t  e  d       
	
	.			// single dot exits the program
	


### ASCII mode

The same as in hex mode, but commands are entered as plain ascii text.  


$ ./hTT -d /dev/ttyUSB3 -b 115200 -a

	AT
	OK
	
	at+cgmm
	SIMCOM_SIM7600E-H
	OK
	
	at+csub
	+CSUB: B04V03
	+CSUB: MDM9x07_LE20_S_22_V1.03_210527
	
	.			// single dot to exit
	


### Read input from file

It is possible to redirect input from file, in both ascii or hex mode.  
Save the desired commands to a text file, one line at a time. The same as you would enter them into hTT.  

Run hTT with any flags you need, and append redirection at the end:

	./hTT _your_flags_ < example_commands.txt

The file will be sent to the device, one line at a time, with a few seconds between lines.

#### Finall thoughts

A usefull ASCII chart. Thanks to the author.

https://www.sciencebuddies.org/cdn/references/ascii-table.png

![ascii-table](https://github.com/nbgsmk/hTT/assets/60575976/3c2747f2-7f34-4800-bd51-9817a3f7b76e)




#### --------- Usefull ideas, some tested, some not --------  

https://stackoverflow.com/questions/76299250/how-to-use-getline-in-a-separate-thread-in-c  

Did not try these yet  
https://stackoverflow.com/a/76334993  

https://www.onlinegdb.com/E9Gwgje7T  

https://stackoverflow.com/a/42264216  

