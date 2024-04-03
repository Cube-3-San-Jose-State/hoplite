# IPCQueue

A simple to use bidirectional IPC class that uses message queues.

It is meant to provide an asynchronous communication link between 2 classes.
Queues are identified by name provided in the constructor.

## Usage

```cpp
// c++ sample code:
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

## Python usage

### Compile python module
Install the following requirements
```
pybind11
virtualenv
```
Then run
```bash
/path/to/IPCQueue$ chmod u+x compile_bindings.sh
/path/to/IPCQueue$ ./compile_bindings.sh
```
This will create a dynamic link library of name `IPCQueue.cpython... .so` in the project directory.

### Usage

If the `.so` module is in the same directory as the python file, the following sample code will work.
Usage is the same as the c++ version.

```python
# sample code:
from IPCQueue import IPCQueue

ipcObject = IPCQueue("queue_id")
ipcObject.write("some command")
incommingCommand = ipcObject.read()
```

### `.so` module file in a different directory
If the `.so` module file is in a different directoy than the python script, the python interpreter won't be able to find it.
The 2 fixes are shown below.

#### Scope module directly
Change the import line to:
```python
from path.to.IPCQueue import IPCQueue
```

#### Add directory to path
Append these line before the import:
```python
import sys
import os
sys.path.insert(0, os.path.abspath('path/to/IPCQueue/module'))
# then IPCQueue can be imported directly
from IPCQueue import IPCQueue
```

## Misc info
- First object created creates the message queues, and also destroys them.
	Undefined behavior may result if the first object created is destroyed before the second object is.
	Therefore, you should always create an IPCQueue object in the parent before creating a child that also uses it.
	This may be fixed in future version
