(set-logic QF_S)
(declare-fun T_2 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun I0_12 () Int)
(declare-fun I0_6 () Int)
(declare-fun I1_12 () Int)
(declare-fun I2_12 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T_3 () Int)
(declare-fun T_6 () Int)
(declare-fun T_7 () Int)
(declare-fun T0_6 () String)
(declare-fun T1_6 () String)
(declare-fun T2_6 () String)
(declare-fun T3_6 () String)
(declare-fun T4_6 () String)
(declare-fun T5_6 () String)
(declare-fun var_0xINPUT_15383 () String)
(assert (= var_0xINPUT_15383 (str.++ T0_6 T1_6)))
(assert (= T1_6 (str.++ T2_6 T3_6)))
(assert (= T2_6 (str.++ T4_6 T5_6)))
(assert (not (str.in.re T4_6 (str.to.re ";"))))
(assert (= T5_6 ";"))
(assert T_5)
(assert (= T_5 (= PCTEMP_LHS_2 (- 1))))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert T_2)
(assert (= T_2 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (>= T_7 T_6))
(assert (= I2_12 I1_12))
(assert (<= T_7 I1_12))
(assert (= I0_12 (- T_7 T_6)))
(assert (>= T_6 0))
(assert (= T_6 (+ PCTEMP_LHS_1 20)))
(assert (= T_3 (+ PCTEMP_LHS_1 20)))
(assert (= PCTEMP_LHS_2 (+ I0_6 T_3)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= T_3 (str.len T0_6))) ; len 0
(assert (= I0_6 (str.len T4_6))) ; len 1
(assert T_SELECT_2)
(check-sat)
