To update picomsDataTypes_KR23.pb.c/h
first make your changes to picomsDataTypes_KR23.proto
then cd to this directory and run "python nanopb/generator/nanopb_generator.py picomsDataTypes_KR23.proto" from command line (I used Ubuntu)
This will generate the .pb.c/h files.
Replace the .pb.c/h and .proto files in MCU's NanoPB folder with the new ones and update the callbacks and PiCommsDriver as needed, then test the code

//updating the .proto ifle in MCU is not technically necessary, but please do it so we know what is being used



To generate a test buffer to send mcu PI_RX, cd to this directory and run:
gcc -o main *.c -I.
./main
This will print a buffer of ints to send. Ex:
10, 14, 8, 128, 1, 24, 0, 32, 57, 40, 63, 64, 255, 1, 112, 6, 18, 8, 16, 45, 32, 78, 40, 2, 112, 0

You can use the DebugMessage() function in InterCommsTask.c to send this data out of PI_TX.
I recomend comenting out ICommsTransmitRoutine() when doing this and shorting PI_TX and PI_RX
To stop all the other tasks (which are also writing to PI_TX) comment out all the RunTaskManager() inits but DataAggregatorInit and InitInternalCommsTask()
Once you do this, the message will be recieved every 2 seconds and every other message will be scrapped since it will contain the debug along with the transmitted message.

You could alternatively send it by bit using RealTerm