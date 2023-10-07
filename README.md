2048 - NeuralNetwork
====

## Thank you note
This is an AI that uses machine learning to play the game 2048.  
Original game code made by jvaelen: https://github.com/jvaelen/2048  
  
## My Introduction to Neural Networking
My first initial experience and research regarding Neural Networks was by watching a 4 part explanation of a youtube channel called '3Blue1Brown'.  
Their Deep learning playlist: https://www.youtube.com/playlist?list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi  

### Specifics ###
This project was made in:
* Visual Studio 17.7.3
* C++ 20 
* Qt version 6.4.1

This was a passion project and is not intended to be used or worked on by others.

## Summary 
The neural network itself works, when prompted to predict the value of the XOR of two values,
it was able to correctly guess the correct answer after a series of attempts.

When combining the Neural Network with the QApplication of the project mentioned above, 
that's where things went a bit off the rails.

The initial idea was to let the Network run on a seperate thread, calculating the best move while
the main thread would take in that input and update the GUI layout of the game.
The standard way of starting a new thread: **std::jthread** was almost immediately thrown out since 
Qt works with their own ways of making and managing multiple threads.

This lead to the research of the documentation of what Qt had to offer.
Even after attempting their way of starting a thread, the issue of not being able to update
the GUI layout from a seperate thread persisted.

We circumvented the issues in someways by binding the space bar to the GUI update method
and virtually simulating a spacebar press within the program itself, but that was too cheesy.

To make a long story short, the Network works, it's just not very compatible with this application
in particular, though that being said, the game made by **jvaelen** is still very well made and worth
taking a look at, wether it's to play it or expand on it like I tried to to do.

I'm taking both a win and a loss here, a win since the network was a succes.
A loss since I wasn't able to properly connect it with what I wanted it to.
