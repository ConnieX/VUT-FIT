#ISS project
#Autor: Monika Rosinská, xrosin00
#16.12.2019

from matplotlib import pyplot as plt 
import numpy as np
import soundfile as sf
from scipy.signal import spectrogram
from scipy.stats import pearsonr
import statistics
import math

#vzorkovaci frekvence
Fs = 16000
#delka jednoho ramce
wlen = int(25e-3 * Fs)
#prekryti dvou ramcu
wshift = int(15e-3 * Fs)

#vytvoreni a naplneni matice A pro features
def fill(mat, row, fromi):
    for x in range(fromi, fromi+16):
        mat[row][x] = 1

A = np.zeros((16, 256))
for i in range(16):
    fill(A, i, 16*i)

#funkce pro vypocet stredni hodnoty
def med(wav):
    mean = statistics.mean(wav)
    wav -= mean
    return wav

#funkce na vyplneni matice spektografu
def spect(s, fs):
    f, t, sgr = spectrogram(s, fs, nperseg=wlen, noverlap=wshift, nfft=511)
    sgr_log = 10 * np.log10(sgr+1e-20) 
    return f, t, sgr_log

#funkce pro vvykresleni spektogramu
def draw_spect(f, t, mat, durr, name):
    plt.figure(figsize=(9,1))
    plt.pcolormesh(t,f,mat)
   # plt.xticks(np.arange(0, durr+1, step=1))
    plt.gca().set_xlabel('Čas [s]')
    plt.gca().set_ylabel('features')
   # cbar = plt.colorbar()
    plt.gca().set_title(name)

#funkce pro vykresleni grafu signalu
def draw_signal(t, s, durr, name):
    plt.figure(figsize=(9,1))
    plt.plot(t, s)
    plt.gca().set_xlabel('Čas [t]')
    plt.gca().set_ylabel('signal')
    plt.gca().set_title(name)
 #   plt.xticks(np.arange(0, durr+1, step=1))  
  
#funkce pro vypocitani skore klicoveho slova
def score(QF, F, leng):
    width139 = F.shape[1]
    widthq1 = QF.shape[1]
    pear = 0
    S = np.zeros(leng)
    F_trans = F.transpose()
    QF_trans = QF.transpose()
    width = width139 - widthq1
    for column in range(width):
        for i in range(widthq1):
             corr, _ = pearsonr(QF_trans[i], F_trans[i+column])
             if not math.isnan(corr):
                 pear += corr             
        pear /= widthq1
        S[column] = pear
        pear = 0
    return S

def draw_score(S1, S2, durr, t):
    plt.figure(figsize=(9,1))
    plt.plot(t, S1, label='agglomeration')
    plt.plot(t, S2, label='bungalow')
    plt.legend()
    plt.ylabel('scores')
    plt.gca().set_xlabel('Čas [t]')
    plt.yticks(np.arange(0, 1, step=0.2))
    plt.show()
    
def sen_funct(s, fs, durr, name):
    s = med(s)
    f, t, P = spect(s, fs)
    F = np.dot(A, P)
    draw_spect(range(16), t, F, durr, name)
    return F, t

def quer_funct(q, fs, name):
    q = med(q)
    fq, tq, Q = spect(q, fs)
    QF = np.dot(A, Q)
    draw_spect(range(16), tq, QF, 1, name)
    return QF

#querie q1
q1, fs1 = sf.read('../queries/q1.wav')
QF1 = quer_funct(q1, fs1, "q1.wav")

#querie q1
q2, fs2 = sf.read('../queries/q2.wav')
QF2 = quer_funct(q2, fs2, "q2.wav")

#sentence sa1.wav
s1, fs1 = sf.read('../sentences/sa1.wav')

tt = np.arange(s1.size) / fs1
draw_signal(tt, s1, 4, "'bungalow' and 'agglomeration' VS sa1.wav")

F1, t1 = sen_funct(s1, fs1, 4, "sa1.wav")
S1 = score(QF1, F1, len(t1))
S2 = score(QF2, F1, len(t1))

draw_score(S1, S2, 4, t1)

print('--------------------------------------------------------------------------------------------')
#sentence sa2.wav
s2, fs2 = sf.read('../sentences/sa2.wav')

tt = np.arange(s2.size) / fs2
draw_signal(tt, s2, 4, "'bungalow' and 'agglomeration' VS sa2.wav")

F2, t2 = sen_funct(s2, fs2, 4, "sa2.wav")

S1 = score(QF1, F2, len(t2))
S2 = score(QF2, F2, len(t2))
1
draw_score(S1, S2, 4, t2)

print('--------------------------------------------------------------------------------------------')
#sentence si769.wav
s769, fs769 = sf.read('../sentences/si769.wav')

tt = np.arange(s769.size) / fs769
draw_signal(tt, s769, 4, "'bungalow' and 'agglomeration' VS si769.wav")

F769, t769 = sen_funct(s769, fs769, 4, "si769.wav")

S1 = score(QF1, F769, len(t769))
S2 = score(QF2, F769, len(t769))

draw_score(S1, S2, 4, t769)

print('--------------------------------------------------------------------------------------------')
#sentence si1399.wav
s1399, fs1399 = sf.read('../sentences/si1399.wav')

tt = np.arange(s1399.size) / fs1399
draw_signal(tt, s1399, 6, "'bungalow' and 'agglomeration' VS si1399.wav")

F1399, t1399 = sen_funct(s1399, fs1399, 6, "si1399.wav")

S1 = score(QF1, F1399, len(t1399))
S2 = score(QF2, F1399, len(t1399))

draw_score(S1, S2, 6, t1399)

print('--------------------------------------------------------------------------------------------')
#sentence si2029.wav
s2029, fs2029 = sf.read('../sentences/si2029.wav')

tt = np.arange(s2029.size) / fs2029
draw_signal(tt, s2029, 3, "'bungalow' and 'agglomeration' VS si2029.wav")

F2029, t2029 = sen_funct(s2029, fs2029, 3, "si2029.wav")

S1 = score(QF1, F2029, len(t2029))
S2 = score(QF2, F2029, len(t2029))

draw_score(S1, S2, 3, t2029)
print('--------------------------------------------------------------------------------------------')
#sentence sx49.wav
s49, fs49 = sf.read('../sentences/sx49.wav')

tt = np.arange(s49.size) / fs49
draw_signal(tt, s49, 4, "'bungalow' and 'agglomeration' VS sx49.wav")

F49, t49 = sen_funct(s49, fs49, 4, "sx49.wav")

S1 = score(QF1, F49, len(t49))
S2 = score(QF2, F49, len(t49))

draw_score(S1, S2, 4, t49)
print('--------------------------------------------------------------------------------------------')
#sentence sx139.wav
s139, fs139 = sf.read('../sentences/sx139.wav')

tt = np.arange(s139.size) / fs139
draw_signal(tt, s139, 4, "'bungalow' and 'agglomeration' VS sx139.wav")

F139, t139 = sen_funct(s139, fs139, 4, "sx139.wav")

S1 = score(QF1, F139, len(t139))
S2 = score(QF2, F139, len(t139))

draw_score(S1, S2, 4, t139)
print('--------------------------------------------------------------------------------------------')
#sentence sx229.wav
s229, fs229 = sf.read('../sentences/sx229.wav')

tt = np.arange(s229.size) / fs229
draw_signal(tt, s229, 3, "'bungalow' and 'agglomeration' VS sx229.wav")

F229, t229 = sen_funct(s229, fs229, 3, "sx229.wav")

S1 = score(QF1, F229, len(t229))
S2 = score(QF2, F229, len(t229))

draw_score(S1, S2, 3, t229)
print('--------------------------------------------------------------------------------------------')
#sentence sx319.wav
s319, fs319 = sf.read('../sentences/sx319.wav')

tt = np.arange(s319.size) / fs319
draw_signal(tt, s319, 5, "'bungalow' and 'agglomeration' VS sx319.wav")

F319, t319 = sen_funct(s319, fs319, 5, "sx319.wav")

S1 = score(QF1, F319, len(t319))
S2 = score(QF2, F319, len(t319))

draw_score(S1, S2, 5, t319)
print('--------------------------------------------------------------------------------------------')
#sentence sx409.wav
s409, fs409 = sf.read('../sentences/sx409.wav')

tt = np.arange(s409.size) / fs409
draw_signal(tt, s409, 5, "'bungalow' and 'agglomeration' VS sx409.wav")

F409, t409 = sen_funct(s409, fs409, 5, "sx409.wav")

S1 = score(QF1, F409, len(t409))
S2 = score(QF2, F409, len(t409))

draw_score(S1, S2, 5, t409)
print('--------------------------------------------------------------------------------------------')