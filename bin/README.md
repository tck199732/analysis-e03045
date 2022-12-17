# analysis-e03045

The goal of these macros is to convert the original root files, which contains event class, to a simpler structure with arrays of momentum, energy, etc... The reason for this conversion is that one has to write the HiRAEvent and HiRAParticle class, which is clean but has to be compiled with ROOT (see below). For a simpler tree structure one can compile by simply
```bash
g++ main.cpp -o main -I`root-config --libs --cflags
```
and can be directly fed to RDataFrame ! You can find the data in `/data/chajecki/EXP_DATA` in WMU LAB PC. 

#### 1. How to use the macro
For now, to convert the data to a structure such that for each event we have branches 
- double mEtrans, multiplicity, etc...
- int Z[multiplicity]
- energy[multiplicity]
```bash
root load.C read.cpp(\"${input_file}\", \"${output_file}\")
```
To run through all files, do 
```bash
python main.py
```
You can find these files in `/data/kin/e03045`.

#### 2. Problem
In the above process, it is found that some files have corrupted entries, possibly some error occurs when transferring files or even related to the root version. To reproduce the issue, one can run
```bash
root load.C a.C
```
For some file, it produces an error :
```bash
Error in <TBasket::ReadBasketBuffers>: fNbytes = 15025, fKeylen = 93, fObjlen = 22184, noutot = 0, nout=0, nin=14932, nbuf=22184
```
which indicates file corruption. A list of corrupted files are written in **del.sh**. It is very probable that only a few entries are problematic in the same file. It is tempted to use all the data and just ingore those corrupted entries. For now, since we already have enough statistics, I will only use the 'good' files and will only come back to modify when I need more data.