## this is ARM CMSIS-DSP run on gcc

The purpose of this application is to use ARM-cmsis-DSP on non-ARM processors.

To use the examples of this repository, it is necessary to go to the desired folder and run the make command 

```sh
cd  arm_matrix_example
make build
make run
```

In the `gcc_fir_gnuplot` folder, there is a Python notebook file that simulate the FIR filter by used to cmsis_dsp wrapper and the C++ code to use the cmsis_dsp library is available.
We need to install [Anaconda](https://www.anaconda.com/download) for Python execution library or  use google colab for run python notebook. 

And to run the C++ program, we need to install GNU Plot. for install gnuplot on debian machine, please run this commnad

```sh
sudo apt install gnuplot

```
finally, when execute command `make build` and `make run` on `gcc_fir_gnuplot` folder, so show 
this image:

![ARM_CMSIS_DSP_FIR_Cpp](/assets/FIR_output_gnuplot.png "ARM CMSIS DSP FIR Cpp")

it is to similar scipy FIR and cmsis-dsp wrapper output in the notebook file

![ARM_CMSIS_DSP_FIR_python](/assets/FIR_output_py.png "ARM CMSIS DSP FIR python")

finally we can check bot mean and variance on jupyter notebook and c++

for python notebook:

```sh
sin noisy mean:  0.0
sin noisy var:  0.51948
fir mean:  0.010776961394674657
fir var:  0.465838732679487
```

for C++

```sh
Mean SIN: -0.00129333
Variance SIN: 0.520287
Mean FIR: 0.0116191
Variance FIR: 0.460044

```



