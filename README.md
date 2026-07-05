# To-Do Manager

## Description

A to-do manager made in C++ for linux, used to keep track of to-dos for you. 
<img width="1897" height="1036" alt="image" src="https://github.com/user-attachments/assets/d678a436-9eb3-4e92-af35-b9872fb39b3c" />

---

## How to download

### Dependencies

- Git
- CMake -- version 4.3
- Ncurses library

Install these before proceeding with the installation steps.

### How To Install

Clone the repository

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

You can delete the folder after this

```bash
cd ..
rm -rf to-do

```

---

## About the Tool

The tasks are stored in your home folder as `.tasks.csv` in CSV format. If updating from previous major version run `todo --updateFile` to update the CSV to the latest format.

### Features
- Auto-Renewing Task - A task can be set to renew everyday.
- Search - Search tasks/categories to find a phrase.
- Toggle Completed Tasks - Want to go through the tasks remaining, you can toggle between seeing competed and uncompleted tasks.

---

## Future Features

### 1. Custom Theming

To be able to set each indivisual colour of the application yourself.

### 2. Auto-Installer & Updater

Making you install the program and transfer is not very nice of me :(
I'll make a installation script that you can run and the program will automatically download and move to the bin folder.
Also an automatic update message when opening the app if there is an update.

---

If you want to contribute please read through the comments once to make sure everything is according to the guideline, and please explain why you have made changes.

Thats all. Thank You for reading all the way through. If you would like to suggest someting to add please feel free too. <3
