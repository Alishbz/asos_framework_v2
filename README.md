# ASOS Framework

ASOS Framework is a custom general-purpose operating system designed using C/C++. This framework offers essential OS functionalities such as task and timing management, along with various data structures and design patterns. It is secure, portable, continuously expandable and greatly supports independent project development for your embedded software projects.

## Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Example Applications](#example-applications)
- [Contributing](#contributing)
- [License](#license)

## Features

ASOS Framework includes the following features and libraries:

### Task and Timing Management

- Functions for creating, deleting, and scheduling tasks.
- Timers and delay functions.
- Semaphore and Mutex support

### Data Structures

- Queue structures.
- List structures.

### Design Patterns

- **Observer Pattern**: Notify other objects when an object's state changes.
- **State Machine**: Create and manage state machines.
- **Bridge Pattern**: Develop abstraction and implementation independently.

### Memory Management

- **Fake Arena Heap Management**: Custom memory management functions.

### Command and Response Management

- **Command Response Manager**: Process commands and manage responses.

### Hardware Management

- **Basic Device Tree**: Define and manage hardware components.

## Example Applications

ASOS Framework comes with various example applications:

- **USB Application**: Example code to manage USB devices.
- **Blink LED**: A simple application to blink LEDs.
- **State Machine**: Examples to perform various operations using state machines.

## Installation

Follow these steps to include ASOS Framework in your projects:

1. **Clone the Project**:
    ```bash
    git clone https://github.com/your_username/asos-framework.git
    cd asos-framework
    ```

2. **Install Required Dependencies**:
    ASOS Framework depends on external libraries like Qt. Ensure these dependencies are installed on your system.

3. **Build the Project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

You can develop your own applications using ASOS Framework. Below is a simple usage example:



```cpp 
#include "asos_root/asos.h"

int main(int argc, char *argv[])
{ 
    // Initialize ASOS framework
    asos_create();

    // Example of creating and scheduling a app
    asos_app_create(...);
}


##Contributing

To contribute to ASOS Framework, please follow these steps:

Fork the project.
Create a new branch: git checkout -b my-feature
Commit your changes: git commit -am 'Add new feature'
Push to the branch: git push origin my-feature
Create a Pull Request.

##License
This project is licensed under the MIT License. See the LICENSE file for more information.
