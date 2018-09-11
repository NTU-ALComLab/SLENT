(set-logic QF_S)
(declare-fun T_15 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_14 () Int)
(declare-fun T_7 () Int)
(declare-fun PCTEMP_LHS_4 () String)
(assert T_8)
(assert (= T_8 (< 0 T_7)))
(assert (= T_7 (str.len PCTEMP_LHS_4))) ; len 0
(assert T_15)
(assert (= T_15 (< 1 T_14)))
(assert (= T_14 (str.len PCTEMP_LHS_4))) ; len 0
(check-sat)
