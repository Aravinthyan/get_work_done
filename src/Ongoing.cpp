// standard include files
#include <iostream>
#include <fstream>

// personal include files
#include "Ongoing.h"

// constructor
Ongoing::Ongoing():
Stage("ongoing"), max_allocated_time(0.0), current_allocated_time(0.0)
{

}

// methods

// print a single task
// should print without time taken as ongoing should not have this info
void Ongoing::print_task(unordered_map<size_t, Task *>::const_iterator current_task)
{
    const unsigned short horizontal_line_length = 50;

    // horizontal =================== for ammount specficed by horizontal line
    print_table_edge('=', horizontal_line_length);

    // print ID
    cout << "ID: "<< current_task->first << endl;

    // print -------------
    print_table_edge('-', horizontal_line_length);

    // print topic
    cout << "Topic: " << current_task->second->get_topic() << endl;
    
    // print -------------
    print_table_edge('-', horizontal_line_length);
    
    // print description
    cout << "Description: " << current_task->second->get_description() << endl;
    
    // print -------------
    print_table_edge('-', horizontal_line_length);
    
    // print time_allocated
    cout << "Time Allocated: " << current_task->second->get_time_allocated() << endl;
    
    // print -------------
    print_table_edge('-', horizontal_line_length);
    
    // print due_date
    cout << "Due Date: " << current_task->second->get_due_date()->get_date() << endl;
    
    // horizontal =================== for ammount specficed by horizontal line
    print_table_edge('=', horizontal_line_length);
}

// this will be used to load the tasks from disk
void Ongoing::load_tasks(void)
{
    // create string which holds file name
    string file_name = "." + this->get_stage() + ".data";

    // open in reading mode
    ifstream stage_file(file_name, ios_base::in);

    // if the file did not open then exit method and give appropriate message
    if(stage_file.is_open() == false)
    {
        cerr << "Error in opening file to save" << endl;
        return;
    }
    
    // holds the data read from the file
    string s_data;
    
    // holds the converted string as a float type which was read from file
    float f_data {0.0};

    // file structure
    // 1st line holds max_allocated_time, extract and then get all other tasks

    // get max_ongoing_time
    getline(stage_file, s_data);

    // convert string to float
    f_data = stof(s_data);

    // set max_allocated_time
    this->max_allocated_time = f_data;
    
    Task * new_task = nullptr;

    try
    {
        while(true)
        {
            // create a new instance of task
            new_task = new Task();
            

            // get topic from file
            getline(stage_file, s_data);

            // check to see if eof has reached
            if(stage_file.eof() == true)
            {
                // delete object since eof has reached and exit
                delete new_task;
                break;
            }

            // set topic
            new_task->set_topic(s_data);

            // get description from file
            getline(stage_file, s_data);

            // set description
            new_task->set_description(s_data);

            // get time_allocated
            getline(stage_file, s_data);

            // convert string to float
            f_data = stof(s_data);

            // set time_allocated
            new_task->set_time_allocated(f_data);

            // increment current_ongoing
            this->current_allocated_time += f_data;

            // get time_taken 
            getline(stage_file, s_data);

            // convert string to float
            f_data = stof(s_data);

            // set time_taken
            new_task->set_time_taken(f_data);

            // get due_date
            getline(stage_file, s_data);

            // set due_date
            new_task->set_due_date(s_data);

            // insert task
            insert_task(hash<string>{}(new_task->get_description()), new_task);
        }
    }
    catch(bad_alloc & exp) // this will take place if the task object cannot be created
    {
        cerr << exp.what() << endl;
        cerr << "Cannot create task, out of memory" << endl;
        cerr << "Cannot create anymore tasks" << endl;
    }
    catch(runtime_error & exp) // this will take place if date object cannot be created
    {
        cerr << exp.what() << endl;
        cerr << "Cannot create anymore tasks" << endl;
        delete new_task;
    }

    // close file
    stage_file.close();
}

// this will be used to save the tasks to disk
void Ongoing::save_tasks(void)
{
    // create string which holds file name
    string file_name = "." + this->get_stage() + ".data";
    
    // open in writing mode
    ofstream stage_file(file_name, ios_base::out);

    // if the file did not open then exit method and give appropriate message
    if(stage_file.is_open() == false)
    {
        cerr << "Error in opening file to save" << endl;
        return;
    }

    // first line will be max_allocated_time
    stage_file << this->max_allocated_time << endl;

    // iterate through the tasks and save each task
    // format to save
    /*
        topic
        description
        time_allocated
        time_taken
        due_date - value not pointer
    */
    // get pointer to ongoing tasks
    unordered_map<size_t, Task *> * ongoing_tasks = get_tasks();

    for(unordered_map<size_t, Task*>::const_iterator it = ongoing_tasks->begin(); it != ongoing_tasks->end(); it++)
    {
        stage_file << it->second->get_topic() << endl;
        stage_file << it->second->get_description() << endl;
        stage_file << it->second->get_time_allocated() << endl;
        stage_file << it->second->get_time_taken() << endl;
        stage_file << it->second->get_due_date()->get_date() << endl;
    }

    // close stage_file
    stage_file.close();
}

// print menu for current menu and perform action per user request
void Ongoing::menu(void)
{
    // holds the choice from the menu
    int choice {0};
    cout << "Ongoing Menu" << endl;
    cout << "1) Modify tasks" << endl;
    cout << "2) Set Maximum Allocated Time" << endl;
    cout << "3) Remove Task" << endl;
    cout << "4) Print stats" << endl; 
    cout << "5) Print" << endl;
    cout << "6) Go back" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    // holds the id of the task to modify
    size_t id {0};

    // hold data for max ongoing time
    float new_max_ongoing_time {0.0};
  
    switch(choice)
    {
    case 1:
        cout << "Enter ID of task to modify: ";
        cin >> id;
        modify_task(id);
        break;
    case 2:
    {
        cout << "Enter Maximum Allocated Time: ";
        cin >> new_max_ongoing_time;
        bool success = set_max_allocated_time(new_max_ongoing_time);
        // if the result of max_allocated_time is less than zero a false is returned as time cannot be negative
        if(success == false)
        {
            cout << "Value cannot be less than 0" << endl;
        }
    }
        break;
    case 3:
        cout << "ID of task to remove: ";
        cin >> id;
        remove_task(id);
        break;
    case 4:
        cout << "Maximum Allocated Time: " << this->max_allocated_time << endl;
        cout << "Current Allocated Time: " << get_current_allocated_time() << endl;
        break;
    case 5:
        print();
        break;
    case 6:
        cout << "Going back ..." << endl;
        break;
    default:
        cout << "Choice does not exist" << endl;
        break;
    }
}

// increase the current_allocated_time with the value provided as a parameter to the method
void Ongoing::increase_current_allocated_time(float time_allocated)
{
    // increase current_allocated_time by the value time_allocated
    this->current_allocated_time += time_allocated;
}

// decrease the current_allocated_time with the value provided as a parameter to the method
void Ongoing::decrease_current_allocated_time(float time_allocated)
{
    // decrease current_allocated_time by the value time_allocated
    this->current_allocated_time -= time_allocated;
}

// accessors

// setter max_allocated_time
bool Ongoing::set_max_allocated_time(float & max_allocated_time)
{
    if(max_allocated_time < 0)
    {
        return false;
    }
    this->max_allocated_time = max_allocated_time;
    return true;
}

// getter max_allocated_time
float Ongoing::get_max_allocated_time(void)
{
    return this->max_allocated_time;
}

// getter current_allocated_time
float Ongoing::get_current_allocated_time(void)
{
    return this->current_allocated_time;
}

//destructor
Ongoing::~Ongoing()
{

}