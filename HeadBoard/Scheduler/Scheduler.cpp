/*
- Schedule one-time tasks from ground control command: schedule_once(int time, String command)
- Schedule recurring tasks from ground control command: schedule_interval(int initial_time, int repeat_interval, int repeat_count, String command)
- Ability to delete scheduled/repeated tasks from ground control command schedule_delete(int id)
- Maintain a JSON structured log that contains info on past commands ran, and future commands scheduled.
  This function will only be called manually by the command handler, so for now just have it print the data out.
*/

#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <thread>

struct Task {
    Task(int id, int time, int repeat_interval, int repeat_count, void (*function)()) {
        this->id = id;
        this->time = time;
        this->repeat_interval = repeat_interval;
        this->repeat_count = repeat_count;
        this->function = function;
    }
    int id;
    int time;
    int repeat_interval;
    int repeat_count;
    void (*function)();
};

class Scheduler {
   private:
    int current_id;
    std::priority_queue<Task> tasks;

    bool execute_task(Task task) {
        task.function();
        if (task.repeat_count > 0) {
            task.repeat_count--;
            task.time = task.repeat_interval;
            tasks.push(task);
        }
    }

   public:
    int schedule_once(int execute_time, void (*function)()) {
        // TODO: figure out execute time
        current_id++;
        Task* task = new Task(current_id, execute_time, 0, 0, function);
        tasks.push(*task);
        return task->id;
    }

    int schedule_interval(int initial_time, int repeat_interval, void (*function)(), int repeat_count = INT8_MAX) {
        // TODO: figure out execute time
        current_id++;
        Task* task = new Task(current_id, initial_time, repeat_interval, repeat_count, function);
        tasks.push(*task);
        return task->id;
    }

    void schedule_delete(int id) {
        // TODO: implement
    }

    void start() {
        // Start the scheduler
        Task task = tasks.top();
        while (true) {
            if (task.time <= 0 && execute_task(task)) {
                // TODO: figure out execute time
                tasks.pop();
                task = tasks.top();
            }
        }
    }
};