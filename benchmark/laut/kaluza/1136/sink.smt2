(set-logic QF_S)
(declare-fun T_SELECT_12 () Bool)
(declare-fun T_ae () Bool)
(declare-fun I0_345 () Int)
(declare-fun I1_345 () Int)
(declare-fun I2_345 () Int)
(declare-fun PCTEMP_LHS_16 () Int)
(declare-fun T_bd () Int)
(declare-fun T_c4 () Int)
(declare-fun T0_300 () String)
(declare-fun T1_300 () String)
(declare-fun T_ac () String)
(assert (= T_ac (str.++ T0_300 T1_300)))
(assert (not (str.in.re T1_300 (str.to.re "__utmz=169413169."))))
(assert T_ae)
(assert (= T_ae (< (- 1) PCTEMP_LHS_16)))
(assert (>= T_c4 T_bd))
(assert (= I2_345 I1_345))
(assert (<= T_c4 I1_345))
(assert (= I0_345 (- T_c4 T_bd)))
(assert (>= T_bd 0))
(assert (= T_bd (+ PCTEMP_LHS_16 7)))
(assert (= PCTEMP_LHS_16 (- 1)))
(assert (= T_SELECT_12 (not (= PCTEMP_LHS_16 (- 1)))))
(assert (= 0 (str.len T0_300))) ; len 0
(assert (not T_SELECT_12))
(check-sat)
