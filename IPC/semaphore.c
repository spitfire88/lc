// Given a resource, use two named semaphore to synchronize access to the resource
// Whats the difference between using named semaphores and named pipe?

/*
Pipes vs Named Semaphores for Synchronization
Pipes can provide synchronization through:
Blocking behavior: Reading from an empty pipe blocks until data is available
Data flow control: Writing to a full pipe blocks until space is available
Message passing: Can send actual data along with synchronization signals

Key differences:
Aspect	    Named Semaphores	            Pipes
Purpose	    Pure synchronization/counting	Data transfer + synchronization
Overhead	Lower (just counter operations)	Higher (data copying)
Flexibility	Simple increment/decrement	    Can pass complex messages
Persistence	Survive process death	        Named pipes persist, anonymous don't
Performance	Faster for pure sync	        Slower due to data movement
*/

