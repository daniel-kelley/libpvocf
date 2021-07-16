;linear swept chirp
;adapted from http://www.csounds.com/journal/issue14/CsoundWorkbench.html
<CsoundSynthesizer>
<CsOptions>

csound -s -d -m0 -W -ochirp.wav -b800 -B800

</CsOptions>
<CsInstruments>

sr	=	48000
nchnls  =       1
0dbfs   = 32767

instr	1
  kpitch linseg  0.000, p3, 1.00

  aout oscili 0dbfs, sr/2 * kpitch, 1
  out aout

endin


</CsInstruments>
<CsScore>
; One period of a sine wave
f 1	0	8192	10	1

; chirp 0-2 seconds
i1	0	2.00
e


</CsScore>
</CsoundSynthesizer>
