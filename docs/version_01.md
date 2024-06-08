# Goals
I will never write this renderer from scratch again, I want to make it simple to use! I never want to go through this again! 

...
...
...

To my future self, your welcome!

# Resources
https://registry.khronos.org/vulkan/specs/1.1/html/vkspec.html#introduction

https://github.com/SaschaWillems/Vulkan-Samples/tree/main/framework/core

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

Note: Queues with the same priority do not guarantee order, they may come in any order

Note: Queues are created and destroyed along with the Logical Device. (Their lifetime are coupled)

# Next Up
- Command Buffers