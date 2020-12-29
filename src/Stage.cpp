// standard include files
#include <iostream>
#include <fstream>
#include <string>

// personal include files
#include "Stage.h"

using namespace std;
        
// constructor
Stage::Stage(string stage):
stage(stage)
{
    try
    {
        tasks = new unordered_map<size_t, Task *> ();
    }
    catch(const bad_alloc & exp)
    {
        cerr << "Not enough memory to create " + this->stage << endl;
    }
}

// methods
// insert new task
void Stage::insert_task(void)
{
    Task * new_task = nullptr;
    try
    {
        // create default Task
        new_task = new Task();

        // get info about task from stdin
        cout << "Set details of new task" << endl;
        new_task->set_task_stdin();

        // add task to unordered_map
        // key - hashed task->description
        // value - new_task
        cout << "Inserting task" << endl;
        tasks->insert(make_pair(hash<string>{}(new_task->get_description()), new_task));
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
}

// remove task
void Stage::remove_task(size_t id)
{
    // check to see if the key exists in the map
    unordered_map<size_t, Task*>::const_iterator item_to_remove = tasks->find(id);
    if(item_to_remove == tasks->end())
    {
        cerr << "Item does not exist" << endl;
    }
    else
    {
        // delete the memory allocated for the Task
        delete item_to_remove->second;

        // erase using key
        tasks->erase(id);
    }
}

// this will be used to load the tasks from disk
void Stage::load_tasks(void)
{
    // create string which holds file name
    string file_name = "." + this->stage + ".data";

    // open in reading mode
    ifstream stage_file(file_name, ios_base::in);

    // if the file did not open then exit method and give appropriate message
    if(stage_file.is_open() == false)
    {
        cerr << "Error in opening file to save" << endl;
        return;
    }
    
    Task * new_task = nullptr;

    try
    {
        while(true)
        {
            // create a new instance of task
            new_task = new Task();
            
            // holds the data read from the file
            string s_data;

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

            // holds the converted string read from file
            float f_data {0.0};

            // get time_allocated
            getline(stage_file, s_data);

            // convert string to float
            f_data = stof(s_data);

            // set time_allocated
            new_task->set_time_allocated(f_data);

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

            tasks->insert(make_pair(hash<string>{}(new_task->get_description()), new_task));
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
void Stage::save_tasks(void)
{
    // create string which holds file name
    string file_name = "." + this->stage + ".data";
    
    // open in writing mode
    ofstream stage_file(file_name, ios_base::out);

    // if the file did not open then exit method and give appropriate message
    if(stage_file.is_open() == false)
    {
        cerr << "Error in opening file to save" << endl;
        return;
    }

    // iterate through the tasks and save each task
    // format to save
    /*
        topic
        description
        time_allocated
        time_taken
        due_date - value not pointer
    */
    
    for(unordered_map<size_t, Task*>::const_iterator it = tasks->begin(); it != tasks->end(); it++)
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

// print a table edge with a specific character
void Stage::print_table_edge(const char character, const unsigned short horizontal_line_length)
{
    for(unsigned short i = 0; i < horizontal_line_length; i++)
    {
        cout << character;
    }
    // print a new line
    cout << endl;
}

// print a single task
void Stage::print_task(unordered_map<size_t, Task *>::const_iterator current_task)
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
    
    // print time_taken
    cout << "Time Taken: " << current_task->second->get_time_taken() << endl;
    
    // print -------------
    print_table_edge('-', horizontal_line_length);

    // print due_date
    cout << "Due Date: " << current_task->second->get_due_date()->get_date() << endl;
    
    // horizontal =================== for ammount specficed by horizontal line
    print_table_edge('=', horizontal_line_length);
}

// print all tasks
void Stage::print_all_tasks(void)
{
    for(unordered_map<size_t, Task*>::const_iterator it = tasks->begin(); it != tasks->end(); it++)
    {
        print_task(it);
    }
}

// destructor
Stage::~Stage()
{
    for(auto it = tasks->begin(); it != tasks->end(); it++)
    {
        delete it->second;
    }

    delete tasks;
}
