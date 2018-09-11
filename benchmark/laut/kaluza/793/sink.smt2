(set-logic QF_S)
(declare-fun T_e () Bool)
(declare-fun T_d () Int)
(declare-fun PCTEMP_LHS_4 () String)
(assert T_e)
(assert (= T_e (< T_d 4)))
(assert (= T_d (str.len PCTEMP_LHS_4))) ; len 0
(check-sat)
