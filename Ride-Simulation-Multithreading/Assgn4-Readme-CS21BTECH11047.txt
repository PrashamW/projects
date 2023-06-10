After extracting the files on the local system, make an input file named "inp-params.txt" which contains the following values in the specified format:
P C lambdaP lambdaC k
where P is number of passenger threads, C is number of car threads, lambdaP is the parameter for the exponential wait between 2 successive ride requests made by the passenger,
lambdaC is the parameter for the exponential wait between 2 successive ride request accepted by a car and k, the number of ride request made by each passenger.

To compile the code, type the following command in terminal:
g++ Assgn4-Src-CS21BTECH11047.cpp

To run the compiled code, type the following command in terminal:
./a.out

After running these commands, an "output.txt" file is generated, which contains the log of all rides. For example, it tells which Passengers enter the museum and when,
which passengers request rides, which passengers get accepted by a car and which car, the time of start of ride, the end time of ride, etc. for all the rides of all passengers
and all the cars. Time is measured from the moment the program started (as a reference) and in microseconds.

The values of parameters in input file are varied according to the requirement of this lab assignment, the corresponding values of average time noted and their corresponding graphs plotted.

To delete the ouput.txt file, type the following command in terminal:
rm output.txt