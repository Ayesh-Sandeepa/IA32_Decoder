# IA32_Decoder

This is written using c++. Final executable can be built using the follwing command.

How to build
```
$ make

```

The final executable created is named **main**. In the test.txt file there are IA 32 instruction encoded according to the intel documentation. When the file is executed, it reads the encoded instructions in the test.txt file and displays the output
in terminal. After each instruction is executed, the content in the registers are also diplayed. Moreover, when accessing (reading from / writing to memory) 
memory, those interactions are logged in to a seperate file called **memory_accesses.txt**.

And to run the executable use following command.

To run:
```
./main
```
