# analysis-e03045

The goal of these macros is to convert the original root files, which contains event class, to a simpler structure with arrays of momentum, energy, etc... The reason for this conversion is that one has to write the HiRAEvent and HiRAParticle class, which is clean but has to be compiled with ROOT (see below). For a simpler tree structure one can compile by simply
```bash
g++ main.cpp -o main -I`root-config --libs --cflags
```
and can be directly fed to RDataFrame !

For now, to convert the data to a structure such that for each event
- double mEtrans, ...
- int Z[multi]
- energy[multi]

```bash
root load.C read.cpp(\"${input_file}\", \"${output_file}\")
```