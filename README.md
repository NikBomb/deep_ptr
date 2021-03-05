# deep_ptr

Single header implementation of a smart pointer that allows deep copy

Deep_ptr allows the safety of a unique ptr with copy semantics.
When a copy is required the deep_ptr copies the resource. 
To avoid object slicing in a class hierarchy an object of type T
has to provide an accessible T* clone() method, which will be chosen over the copy constructor.
