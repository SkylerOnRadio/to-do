# To-Do Manager

## Description

A to-do manager made in C++ for linux, used to keep track of to-dos for you. The to-do last until you complete them and are deleted automatically 1 day after setting them to complete.

---

## How to download

This tool needs ncurses to run, please install that first(though linux usually has these installed already since other tools also use ncurses)

After you have installed ncurses, clone the github repo

```
git clone https://github.com/SkylerOnRadio/to-do.git
```

after which you will have to compile the tool using a C++ compiler

```
g++ main.cpp ui.cpp model.cpp tasks.cpp -O3 -lncurses -o todo
```

A brief explanation of the compiler statement:

- **g++** : calls the g++ compiler
- **-O3**: this is flag that tells the compiler to perform aggressive optimization on the code, this makes compile time slower but makes the runtime speed significantly faster
- **-o todo**: this tells the compiler to save the binary output as todo
- **-lncurses**: this is a dependency for the program it is responsible for the ui of the program

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

The UI currently is doable for me, it is just enough to provide basic functions. It is missing things like being able to size more appropriately looking at the terminal size. In addition to this, I would like to be able to add a theming option to give the colours custom values so that users can customize them to their liking.

### Auto-Renewing Tasks

I have a need for tasks that need to be repeated everyday so that is something that I would like to add

---

Thats all. Thank You for  reading all the wa through. If you would like to suggest someting to add please feel free too. <3
