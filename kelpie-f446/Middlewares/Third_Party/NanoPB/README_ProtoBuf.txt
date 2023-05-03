To update the proto files (DataTypes.pb.c/h, DataTypes.proto) you need to clone "https://github.com/nanopb/nanopb"
To make the .pb.c/h files, have DataTypes.proto and the cloned nanopb in the same repository
Once you have that, you can run "python ../nanopb/generator/nanopb_generator.py message.proto" from command line (I used Ubuntu)
This will generate the .pb.c/h files.
Replace the files in MCU's NanoPB folder with the new ones and update the callbacks and PiCommsDriver as needed, then test the code