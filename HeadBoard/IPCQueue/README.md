# IPCQueue

A simple to use bidirectional IPC class that uses message queues.

It is meant to provide an asynchronous communication link between 2 classes.
Queues are identified by name provided in the constructor.

## Usage

```cpp
// sample code:
IPCQueue ipcObject("queue_id");
ipcObject.write("some command");
string incommingCommand = ipcObject.read();
```

### Instantiation
- Provide a name for the the message queue. This is how we link queues together.
- This is meant for bidirectional behavior only, create more than 2 object that interface with the same queue.

### Writing
- `write(std::string command)`
- Will send command to message queue, which can be read by receiving object.
- This is non-blocking.
- Commands have a hardcoded character limit, can be read by viewing the `IPCQueue::MAX_MESSAGE_LENGTH` constant.


### Reading
- `read() -> std::string`
- non-blocking, if no command is present, will return an empty string.


## Misc info
- First object created creates the message queues, and also destroys them.
	Undefined behavior may result if the first object created is destroyed before the second object is.
	Therefore, you should always create an IPCQueue object in the parent before creating a child that also uses it.
	This may be fixed in future version
