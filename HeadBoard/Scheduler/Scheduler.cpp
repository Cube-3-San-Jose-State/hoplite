#include <chrono>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <thread>

using Time = std::chrono::time_point<std::chrono::system_clock>;

/**
 * Task struct to hold the task information
 * id: unique identifier for the task
 * time: time at which the task is to be executed
 * repeat_interval_ms: interval at which the task is to be repeated
 * repeat_count: number of times the task is to be repeated
 * function: function to be executed
 */
struct Task {
    Task(int id, Time time, std::chrono::milliseconds repeat_interval_ms, int repeat_count, void (*function)()) {
        this->id = id;
        this->time = time;
        this->repeat_interval_ms = repeat_interval_ms;
        this->repeat_count = repeat_count;
        this->function = function;
    }
    int id;
    Time time;
    std::chrono::milliseconds repeat_interval_ms;
    int repeat_count;
    void (*function)();
};

class Scheduler {
   private:
    // FIXME: current_id sometimes does not start at 1, but jumps to a random number
    int current_id;
    static bool TaskComparator(Task a, Task b) { return a.time > b.time; };
    std::priority_queue<Task, std::vector<Task>, decltype(&TaskComparator)> tasks{&TaskComparator};
    std::set<int> to_delete;

    /**
     * Execute the task and reschedule if needed.
     * @param task: task to be executed
     * @return true if the task was executed successfully
     */
    bool execute_task(Task task) {
        task.function();
        // reschedule if needed
        if (task.repeat_count > 1) {
            task.repeat_count--;
            task.time = std::chrono::system_clock::now() + task.repeat_interval_ms;
            tasks.push(task);
        }
        return true;
    }

   public:
    /**
     * Adds a one-time task to the scheduler.
     * @param execute_time: time at which the task is to be executed
     * @param function: function to be executed
     * @return id of the task
     */
    int schedule_once(Time execute_time, void (*function)()) {
        current_id++;
        // create task and add to queue
        Task* task = new Task(current_id, execute_time, std::chrono::milliseconds(0), 0, function);
        tasks.push(*task);
        return task->id;
    }

    /**
     * Adds a repeated task to the scheduler.
     * @param initial_time: time at which the task is to be executed
     * @param repeat_interval_ms: interval at which the task is to be repeated
     * @param function: function to be executed
     * @param repeat_count: number of times the task is to be repeated (default: INT32_MAX)
     */
    int schedule_interval(Time initial_time, std::chrono::milliseconds repeat_interval, void (*function)(), int repeat_count = INT32_MAX) {
        current_id++;
        // create task and add to queue
        Task* task = new Task(current_id, initial_time, repeat_interval, repeat_count, function);
        tasks.push(*task);
        return task->id;
    }

    /**
     * Adds a task to be deleted
     * @param id: id of the task to be deleted
     */
    void schedule_delete(int id) { to_delete.insert(id); }

    /**
     * Starts the scheduler
     */
    void start() {
        while (true) {
            if (!tasks.empty()) {
                // get next task
                Task task = tasks.top();
                // if task is ready
                if (task.time <= std::chrono::system_clock::now()) {
                    if (to_delete.find(task.id) != to_delete.end()) {
                        // delete and do not execute if needed
                        to_delete.erase(task.id);
                        std::cout << "Deleting task " << task.id << std::endl;
                    } else {
                        // execute if not deleted
                        execute_task(task);
                    }
                    tasks.pop();
                }
            }
        }
    }
};

int main() {
    // TEMP: example code
    Scheduler scheduler;

    std::chrono::time_point<std::chrono::system_clock> in_one_second = std::chrono::system_clock::now() + std::chrono::seconds(1);
    std::chrono::time_point<std::chrono::system_clock> in_three_seconds = std::chrono::system_clock::now() + std::chrono::seconds(3);

    // schedule 3 tasks
    int id1 = scheduler.schedule_once(in_one_second, []() { std::cout << "one second once" << std::endl; });
    int id2 = scheduler.schedule_interval(
        in_one_second, std::chrono::milliseconds(500), []() { std::cout << "one second repeating" << std::endl; }, 5);
    int id3 = scheduler.schedule_once(in_three_seconds, []() { std::cout << "three seconds once" << std::endl; });

    // delete first task
    scheduler.schedule_delete(id1);

    scheduler.start();

    return 0;
}