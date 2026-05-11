# To-Do Manager

## Description

A to-do manager made in C++ for linux, used to keep track of to-dos for you. The to-do last until you complete them and are deleted automatically 1 day after setting them to complete.

---

## How to download

To install this tool, you first need to clone the repository or download the code

```
git clone https://github.com/SkylerOnRadio/to-do.git
```

after which you will have to compile the tool using a C++ compiler

```
g++ -O3 -o todo main.cpp
```

A brief explanation of the compiler statement:

- **g++** : calls the g++ compiler
- **-O3**: this is flag that tells the compiler to perform aggressive optimization on the code, this makes compile time slower but makes the runtime speed significantly faster
- **-o todo**: this tells the compiler to save the binary output as todo

After doing all of this you have to move this to the local directory of either the user or the system as a whole

```
# For local user install
mv todo ~/.local/bin/

# For system wide install
sudo mv todo /usr/local/bin/
```

---

## How it works

The tasks are stored in your home folder as `.tasks.csv`. The tasks are stored in this folder in CSV format.

When you call the tool the tasks are loaded from the file first and then the operations specified are performed.

---

## Next Features

### Improved UI

The UI currently is very lacking not taking into account the size of the terminal and using hardcoded output values. The aim is to make the display terminal size dependent. In addition to that the tool also does not look visually apealling, while currently I do not have a specific aim towards this aspect, it is something I have in my mind. Will be using ncurses to make a full on terminal UI.

### Auto-Renewing Tasks

I have a need for tasks that need to be repeated everyday so that is something that I would like to add

---

Thats all. Thank You for  reading all the wa through. If you would like to suggest someting to add please feel free too. <3
