# Do-It Roadmap

## A storage for Todo & its manipulation
- Insertion always at last
- Deletion is random Access is sequential usually and random for edits
## File Parsers, Saving Loading
- Read from CSV
- Make texts valid for CSV
## Ui
- one panel for task lists
- one panale for task details
- one panel to ask for confirmations and shit
- one panel to show active filters
- one panel for creation of taks
- one panel for notification

### Files needed
- todo.cpp          --> for the storage and manip of todo items
- ui.cpp            --> for displaying the panels
- viewHandling.cpp  --> for handling the viewable range of to-do
- fileHandling.cpp  --> for handling file reading and file saving
- theming.cpp       --> to handle the theming capabailities
- classes and shit

## Features

- [x] can store the task, category, due date, status (incomplete, ongoing, complete) 
- [ ]  custom time limit for auto task deletion
- [ ]  sub tasks
- [ ]  filter categories/date/etc
- [ ]  search (fuzzy if possible)
- [ ]  task that renews after a set amount of time
- [ ]  quick toggle between having and not having completed tasks

## Architecture


### Task properties
- id
- Task
- Category
- status
- renewing (bool)
- due date

- creation date

### Storage for the tasks

# Linked List 
- O(a) access time where a is the task number
- O(a) deletion and insertion for middle parts, O(1) for end and start
- O(n) search

# Vector
- O(n) space complexity, n is the number of tasks
- O(1) access if the index is known
- O(logn + n) deletion (requires us to search the vector for the task)
- O(logn) search for sorted
- O(n) serach for unsorted
- O(n) space complexity, n is number of tasks

Vector will be used to store the tasks then,
We will use a vector of unique pointers that points to the task
Insertion will happen at the end of the vector always (will take O(1) at avg)
To edit, we will take the task id from the UI and then find that, its a O(logn) solution since we will not do reindexing but the vector will be sorted


#### Function needed
- insert(taskDetails) - O(1)
    - create a task from the details
    - move ownership to vector
    - return status
- delete(taskId) - O(n) + O(logn)
    - perform binary search to find the index of the task 
    - reset the unique_ptr
    - erase the index
    - return status
- edit(newData, taskId) - O(logn)
    - perform binart search to find the task
    - edit the values
    - return status
- changeStatus(taskId, status) - O(logn)
    - find the task
    - edit the status feild
    - return status

### FileHandling

Parsing CSV and reading it 
Making values CSV compatible
Writing the data to the file

#### Funtions Needed

- LoadFile
    - Load each line of the file and pass it to ParseCSVAndReturnTask
    - get the ownership of the task
    - transfer ownership to vector
    - return status

- ParseCSVandREturnTask
    - Parse the string_view and store the values in the correct feilds
    - Create an object with the correct values and return the object

- WriteToFile
    - Loop throught the vector
    - Send every feild to parseToCSV() to get the correct value for CSV files
    - Append the values separated by commas 
    - return status

### UI Section

- Global Data
    - start index
    - current task index
    - active filter
    - prev filter
    - update_vector
    - *pointer1* - to the first task displayed
    - *pointer2* - to the last task displayed
- Main
    - Data
        - Main vector to store all tasks 
        - FilteredVector to store the displaying tasks, initialized to the main vector on declaration
        - The window for the display
        - User Input storage
    - Operation
        - Initialize the window
        - Load the file
        - Call display  function
        - start listening to the user input
        - on user input, send it to inputHandler
        - Go back to call display function

- Display function 
    - Call createDisplayableSubvector to get the subvector to display - overwrite the passed filteredVector
    - Call displayTasks function and pass the subvector to it
    - Return status

- createDisplayableSubvector
    - Filter Exits && is not the same as prev filter
        - Create subvector 
        - Loop through each element in vector and check if the condition matches
        - If condition matches then get a raw pointer to that element and push it to the new vector
        - Continue till the last element of vector
        - Set active filter to empty
    - Return the subvector

- Display funtion
    - Takes the filtered subVector
    - Calculates the maximum number of tasks that can be displayed
    - start from the **start** *global variable* and assign that task to *pointer1*
    - display the tasks and highlight the task at the **current index** *global variable*   
    - continue till the `start + maxDisplaybaleTasks` index and assign it to *pointer2* 
    - Call displayTaskDetail fn and pass the current index to it
    - return status

- Display Task Detail
    - Get the task at **current index** *global variable* from the subvectored passed
    - Display the deatils of the task in the panel
    - return status

- Input Handler
    - Abstract Data (data not stored but present)
        - q -> quit 
        - h -> help
        - i -> insert
        - d -> delete
        - c -> toggle complete
        - f -> filter
        - s -> search
        - e -> edit
        - j -> down
        - k -> up
        - t -> toggle complete/incomplete->add incomplete to filter / remove it
        - Actions for the inputs
        q -> call saveTasks->break main loop->delete the windows->gracefully exit
        h -> call displayHelpMenu()
        i -> call insertTask()->update **update_vector** to true 
        d -> call deleteTask()->update **update_vector** to true 
        c -> call statusMenu()->call changeStatus()->update **update_vector** to true
        f -> call filterMenu()->update **update_vector** to true
        s -> call searchMenu()
        e -> call editMenu()->call edit()->update **update_vector** to ture 
        j -> is the *current_index*>= maxDisplaybaleTasks - 3  
                                    |
                                    |-> No --> *current_index++* 
                                    |
                                    |-> Yes --> *pointer2* == *lastElementInFilteredVector*  
                                                            |
                                                            |-> No --> *start_index++* 
                                                            |
                                                            |-> yes --> *current_index>=lastIndex* ? return : *current_index++*
                                                            
        k -> is the *current_index* <=  3  
                    |
                    |-> No --> *current_index--*
                    |
                    |-> Yes --> *pointer1* == *firstElementInFilteredVector*  
                                            |
                                            |-> No --> *start_index--* 
                                            |
                                            |-> yes --> *current_index <= 0* ? return : *current_index--*

- displayHelpMenu --> Simply display all the keybinds

- insertTask
    - Ask task, category, renewing
    - Call the createTask
