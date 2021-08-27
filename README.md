T9 is a C program that reads in a dictionary in txt format and then creates an interactive prompt that a user can use to search words in the dictionary in a T9 format.

To run the code, download and move the files to a directory. The make file can be used to compile the code and clean the directory. The only argument needed to run the code is the .txt dictionary file, which must be formatted with one word per line. After a word is searched, a # can be entered to advance to the next 'T9onym', or next word that is represented by the same T9 representation.

ex. 323 == 'dad' and 323 == 'fad'

Compile: 
```
$ make t9
```

Execute:
```
$ ./t9 dictionary.txt
```

## Authors

* **Tom McDowell**
