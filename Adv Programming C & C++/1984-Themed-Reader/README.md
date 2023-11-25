README for The Great Firewall of Santa Cruz

This program evaluates words from STDIN and evaluates
if they appear in a list of old speak or bad speak 
words. It uses both a bloom filter and a hash table
to keep track of the keys and values and their ocurrences
To use the program do the following:

- Use "make" or "make all" to compile the program
- Call the excecutable "newspeak" 
- Use one of the following options:
	-h   Defines size of hash table - default set to 10000
	-f   Defines size of bloom filter - default set to 2^20
	-m   Use move-to-front algorithm (Sets elements from each list to the front if they are looked up)
	-b   Not use move-to-front (Run normally)

- Type into STDIN the words that you want to check
- A message from the program will explain your word usage, hopefully you only used
words that conform to our government policies. We don't want to send you to JoyCamp
but Big Brother will do so if needed.
