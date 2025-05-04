********* Edge-Enabled ECG Signal Acquisition and Arrhythmia Detection using Arduino and Python **********

Team Members- 
1) Sanjeev C Achar, M.Tech INS, Dept. of IAP, IISc Bangalore
   
   Contribution: Did the data acquisition from the ECG leads, did pre-processing and converted them to .csv files using Python for uploading onto Edge                
   Impulse. After the training was done by Sumanth, compiled the C Code for deployment, modified the header files, added the necessary libraries, performed  
   calibration and tested the prototype.

2) Sumanth S P, M.Tech INS, Dept. of IAP, IISc Bangalore
   
   Contribution: Simulated heart anomalies in ECG by modifying the waveform using Signal Editor on Simulink for the case of Atrial Fibrillation and 1st-Degree    
   Atrio-Ventricular Block by distorting the ECG waveforms accurately as per official and valid medical observations, trained the model on Edge Impulse.

*****Information about codes*****
1) data_acqusition.py-- code to acquire data from Nano BLE and write to a .csv file.
2) Excelt2Arrayconverter.m.txt-- code to convert the excel data into MATLAB array used for anomaly simulation and to train the model.
3)  ECGAnalyzer.ino-- Main code to be run on Arduino after Edge Impulse procedure
4) ecg_analyzer.c and ecg_analyzer.h and ecg_inference.h-- to be included with the .ino file as headers and inferences.
