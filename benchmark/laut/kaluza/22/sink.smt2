(set-logic QF_S)
(declare-fun T_3 () Bool)
(declare-fun T_2 () Int)
(declare-fun PCTEMP_LHS_1 () String)
(assert T_3)
(assert (= T_3 (< 0 T_2)))
(assert (= T_2 (str.len PCTEMP_LHS_1))) ; len 0
(check-sat)
