# Goals
I will never write this renderer from scratch again, I want to make it simple to use! I never want to go through this again! 

...
...
...

To my future self, your welcome!

# Resources
https://registry.khronos.org/vulkan/specs/1.3/html/
https://registry.khronos.org/vulkan/specs/1.1/html/vkspec.html#introduction

https://github.com/SaschaWillems/Vulkan-Samples/tree/main/framework/core

https://vulkan-tutorial.com/

# Obseravations

- Device, Instance, Physical Device are good starting points for the renderer
- Commands, Queues, and Creating Command Buffers will be most likely the next step
- Will continue reading the specification as go

## Instance
The Vulkan `Instance`, simply put, when a `VkInstance` is created, the Vulkan library is initialized.

The `Instance` is responsible for the following:
- Enabling layers
- Enabling extensions
- Finding Physical Devices (GPUs)
- Setting up debug callbacks

### Layers
- When `Layers` are enabled, they are inserted into the call chain for Vulkan commands. They are useful for things like logging, tracing, validation etc.

### Extensions
- When `Extensions` are enabled, they provide new Vulkan commands, structures, and enumerants. Extensions are part of layers.


## Devices

Vulkan has two concepts of devices. A `PhysicalDevice` and a `LogicalDevice`. 

The `PhysicalDevice` represents a single complete implementation of Vulkan or in other words represents a GPU. A `LogicalDevice` represents an instance of that implementaion with its own state and resources.

### Physical Devices
Looked up by an Instance, a PhysicalDevice can be selected based on its avaialble properties, version, etc. Its up to the developer to select the PhysicalDevice.

Each PhysicalDevice will have properties on its avaialble the `Queues` and their `QueueFamilies` which can be queried.

### Logical Device
Once a PhysicalDevice has been selected a LogicalDevice can then be created. That corresponding LogicalDevice can now be used as the interface of the PhysicalDevice.

Note: A single LogicalDevice can be created from a group of PhysicalDevice. Groups of PhysicalDevice using a single LogicalDevice must support iddentical extensions, features, and properties.

When a Logical Device is created all of the GPUs queues are created as well. This is where you have an opportunity to define the queue priorities based on the needs of the application and what is avaialble on the GPU.

> **Note:** Queues with the same priority do not guarantee order, they may come in any order

> **Note:** Queues are created and destroyed along with the Logical Device. (Their lifetime are coupled)

### Command Buffers
Command Buffers are objects that can record commands and send them to a Logical Device Queue to be executed.

There are two levels for Command Buffers. There are `Primary` Command Buffers, which can execute `Secondary` Command Buffers, and can be submitted to Queues. There are `Secondary` Command Buffers, which can be executed by `Primary` Command Buffers, which are not directly submitted to Queues.

Each Command Buffer can be in one of the following states:
- `Initial`, when a Command Buffer is allocated, it is in the initial state.
- `Recording`, in this state Commands can be recorded to the Command Buffer
- `Executable`, the recording state has ended, now the Command Buffer can be submitted to a queue, recorded to another command buffer, or reset to its initial state
- `Pending`, when submitted to a queue, the buffer will enter the pending state. During which, Command Buffers must not be modified, since the device could be processing the commands recorded to it. After completion a command buffer can revert back to its Executbale or Invalid state.
- `Invalid`, when modifying or deleting a resource recorded by a command buffer, will transition the command buffer to the invalid state. Command Buffers in this state can only freed or reset.

The Lifecycle of a Command Buffer:

![Lifecycle of a Command Buffer](resources/TheLifeOfACommandBuffer.png)

> **Note:** If a secondary command buffer moves to initial or invalid state, then all primary command buffers that is recorded in will move to the invalid state. A primary command buffer changing to any other state does not affect the secondary command buffers recorded on it.

### Command Pools
Command Pools are opaque objects that Command Buffers Memory is allocated from. This is done to reduce the cost over time for the resource creation of Command Buffers.

# Next Up
- Command Buffers
- Command Pool 