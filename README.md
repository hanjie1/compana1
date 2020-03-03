# compana1

1.  Run the decoder:

./compana

2.  The root file is save in Rootfiles
It has three trees:
'T' -- main event trees which have the FADC, VETROC and some scaler info (BCM, BPM, clock, CavPower)
'E' -- scaler tree, with 32 channels read out
'VTP' -- VTP data tree

The varibales of the trees can be found in "SetTreeVars.h". Some global parameters (e.g. FADC channels) can be found in 
"SetParams.h".


3.   To plot FADC raw mode ADC samples:

root -l PlotRawMode.C

The raw mode root files are saved in FADC_RawROOtfiles/

4.   To plot electron plane strip pattern:

root -l StripPattern.C


