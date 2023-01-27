#!/bin/sh

#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
# This script creates files of words that are used as examples, in
# the Votrax SC-01A datasheet, that illustrate how the phonems
# sound from the synthesizer.  The comments show the words with
# the emphasized vowels capitolized and delimited by underscores.
# This shows the portion of the speech that need to be extracted
# when creating the final PCM file of the phoneme representation.
# The duration indicates the phonem duration that is output from
# the Votrax synthesizer.
#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

# jack_E_t, Duration: 59ms
echo jacket > EH3

# E_nlist, Duration: 71ms 
echo enlist > EH2

# h_EA_vy, Duration: 121ms
echo heavy > EH1

# PA0: silence, Duration: 47ms

# bu_TT_er, Duration: 47ms
echo butter > DT

# m_A_de, Duration: 71ms
echo made > A2

# m_A_de, Duration: 103ms
echo made > A1

# a_Z_ure, Duration: 90ms
echo azure > ZH

# h_O_nest, Duration: 71ms
echo honest > AH2

# inhib_I_t, Duration: 55ms
echo inhibit > I3

# I_nhibit, Duration: 80ms
echo inhibit > I2

# inh_I_bit, Duration: 121ms
echo inhibit > I1

# M_at, Duration: 103ms
echo mat > M

# su_N, Duration: 80ms
echo sun > N

# B_ag, Duration: 71ms
echo bag > B

# V_an, Duration: 71ms
echo van > V

# CH_ip, Duration: 71ms
echo chip > CH

# SH_op, Duration: 121ms
echo shop > SH

# Z_oo, Duration: 71ms
echo zoo > Z

# l_AW_ful, Duration: 146ms
echo lawful > AW1

# thi_NG, Duration: 121ms
echo thing > NG

# f_A_ther, Duration: 146ms
echo father > AH1

# l_OO_king, Duration: 103ms
echo looking > OO1

# b_OO_k, Duration: 185ms
echo book > OO

# L_and, Duration: 103ms
echo land > L

# tri_CK, Duration: 80ms
echo trick > K

# J_u_DG_e, Duration: 47ms
echo judge > J

# H_ello, Duration: 71ms
echo hello > H

# G_et, Duration: 71ms
echo get > G

# F_ast, Duration: 103ms
echo fast > F

# pai_D, Duration: 55ms
echo paid > D

# pa_SS,Duration:  90ms
echo pass > S

# d_AY, Duration: 185ms
echo day > A

# d_AY, Duration: 65ms
echo day > AY

# Y_ard, Duration: 80ms
echo yard > Y1

# miss_IO_n, Duration: 47ms
echo mission > UH3

# m_O_p, Duration: 250ms
echo mop > AH

# p_A_st, Duration: 103ms
echo past > P

# c_O_ld, Duration: 185ms
echo cold > O

# p_I_n, Duration: 185ms
echo pin > I

# m_O_ve, Duration: 185ms
echo move > U

# an_Y, Duration: 103ms
echo any > Y

# T_ap, Duration: 71ms
echo tap > T

# R_ed, Duration: 90ms
echo red > R

# m_EE_t, Duration: 185ms
echo meet > E

# W_in, Duration: 80ms
echo win > W

# d_A_d, Duration: 185ms
echo dad > AE

# A_fter, Duration: 103ms
echo after > AE1

# s_A_lty, Duration: 90ms
echo salty > AW2

# A_bout, Duration: 71ms
echo about > UH2

# U_ncle, Duration: 103ms
echo uncle > UH1

# c_U_p, Duration: 185ms
echo cup > UH

# f_O_r, Duration: 80ms
echo for > O2

# ab_OA_rd, 121ms
echo aboard > O1

# y_OU, Duration: 59ms
echo you > IU

# y_OU, Duration: 90ms
echo you > U1

# TH_e, Duration: 80ms
echo the > THV

# TH_in, Duration: 71ms
echo thin > TH

# b_IR_d, Duration: 146ms
echo bird > ER

# g_E_t, Duration: 185ms
echo get > EH

# b_E, Duration: 121ms
echo be > E1

# c_A_LL, Duration: 250ms
echo call > AW

# PA1: silence, Duration: 185ms

# STOP: silence Duration: 47ms

exit 0

