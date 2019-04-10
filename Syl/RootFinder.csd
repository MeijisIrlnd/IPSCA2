<CsoundSynthesizer>
<CsOptions>

</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

instr RootFind
aInput inch 1
fFt pvsanal aInput*1000, 1024, 1024/4, 1024, 1
kFreq, kAmp pvspitch fFt, 0.01

outvalue "freq", kFreq
outch 1, aInput
endin

</CsInstruments>
<CsScore>

i"RootFind" 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>
