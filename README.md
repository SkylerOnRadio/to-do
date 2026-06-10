# To-Do Manager

## Description

A to-do manager made in C++ for linux, used to keep track of to-dos for you. The to-do last until you complete them and are deleted automatically 1 day after setting them to complete.

---

## How to download

This tool needs ncurses to run and CMake to build, please install them first

After you have installed ncurses, clone the github repo

```bash
git clone https://github.com/SkylerOnRadio/to-do.git
```

Then make a build folder inside the cloned directory, and call cmake and make commands

```bash
mkdir build
cd build
cmake ..
make
```

After doing all of this you have to move this to the local directory of either the user or the system as a whole

```bash
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

## Timeline

### 1. Code Refactor

The current codebase has grown to be monstrous. The recent update that added categories really showed me how confusing the code is. I also recently have been reading **Grokking Simplicity** and the way of functional programming sticks up to me as quite an interesting and efficient manner of thinking and writing code, my current codebase is not really upto the standard, since it is filled with myriad of side-effects, actions and very little calculation. The aim for the code refactor is to make the code more hackable allowing for changes to be made faster.

### 2. Auto-Installer

Making you install the program and transfer is not very nice of me :(
I'll make a installation script that you can run and the program will automatically download and move to the bin folder.

### 3. Filter

Filter will allow for you to search through tasks to find on you want, filter to see a certain category of tasks or filter dates.

### 4. Auto-Renewing Tasks

The name is pretty self explanatory, there will be tasks that will repeat everyday.

### 5. Improving UI

Current UI colors and layout is hardcoded, the aim is to make the UI customisable to show what YOU want and need, handing the power for customization to you.

---

Thats all. Thank You for reading all the way through. If you would like to suggest someting to add please feel free too. <3
