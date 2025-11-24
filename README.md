# System Programming Lab 11 Multiprocessing
Program that invokes mandel 50 times and varies the scaling in an exponential fashion to allow for a 
smooth 50 frame .mpg video to be made. To specify number of processes, use -n and then the number of
processes wanting to use. To specify number of threads, use -t and then the number of threads wanting
to use.

## Number of Processes
The 50 images are divided by the number of processes that one wishes to run the program with. The 
remainder is then added onto the processes that finish first or the first few processes usually.

## Number of Threads
To implement multiple threads, I used a struct to pass image parameters and row ranges to each thread.
The image gets divided into the amount of rows. Each thread computes its assigned rows and the main
thread joins all the workers before continuing.

## Graph of Runtime Results (Lab 11)
![Lab11-graph](Lab11-graph.jpg)

## Discussion of Results (Lab 11)
As I had more and more processes, the runtime was shorter and shorter. It followed an n^2 correlation
the best out of every correlation. This makes sense because forking is n^2. Also, the numbers divisible
by 50 for child processes worked the best and had the most significant differences in runtime.

## Table of Runtime Results (Lab 12)
| Threads \ Processes | 1       | 2       | 5       | 10      | 20      |
|---------------------|---------|---------|---------|---------|---------|
| 1                   | 208.717 | 134.076 |  62.384 |  39.304 |  32.522 |
| 2                   | 112.982 |  74.664 |  38.680 |  28.687 |  38.812 |
| 5                   |  59.721 |  40.349 |  44.462 |  27.699 |  38.611 |
| 10                  |  36.139 |  32.469 |  39.906 |  28.149 |  41.457 |
| 20                  |  31.175 |  29.505 |  27.998 |  37.905 |  27.139 |

## Discussion of Results (Lab 12)
Increasing the number of threads and the number of processes had a similar effect, but increasing the
number of threads had a slightly better impact on runtime than increasing the number of processes did.
I think this is because it has a lower memory and faster context switchign with threads. Threads share
memory and do not crate more and more. This means they also have less creating cost.
There was a sweet spot where optimal runtime was achieved. This was at around 5 - 10 processes with
about 10 threads. My lowest time was 5 threads with 10 processes. I think this is because each process
runs a certain specified amount of threads so if you run too many overall threads than you have too much
overloading on processor for amount of cores.