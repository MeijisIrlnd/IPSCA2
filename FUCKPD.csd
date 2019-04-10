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
fFt pvsanal aInput*2, 256, 1024/8, 512, 1
fAft pvsanal aInput, 256, 1024/8, 512, 1
kFreq, kDiscard pvspitch fFt, 0.001
kDiscard, kAmp pvspitch fAft, 0.001
kSmoothed port kFreq, 0.031

outvalue "freq", kSmoothed/4

outch 1, aInput
endin

</CsInstruments>
<CsScore>

i"RootFind" 0 [60*60*24*7]
</CsScore>
</CsoundSynthesizer>