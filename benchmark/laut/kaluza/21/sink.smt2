(set-logic QF_S)
(declare-fun I0_2 () Int)
(declare-fun PCTEMP_LHS_2 () String)
(assert (= I0_2 (- 20 7)))
(assert (= I0_2 (str.len PCTEMP_LHS_2))) ; len 0
(check-sat)
