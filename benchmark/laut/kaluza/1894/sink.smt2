(set-logic QF_S)
(declare-fun T_6 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun I0_9 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_9 () String)
(declare-fun T1_9 () String)
(declare-fun T2_9 () String)
(declare-fun T3_9 () String)
(declare-fun T4_9 () String)
(declare-fun T5_9 () String)
(declare-fun var_0xINPUT_37381 () String)
(assert (= var_0xINPUT_37381 (str.++ T0_9 T1_9)))
(assert (= T1_9 (str.++ T2_9 T3_9)))
(assert (= T2_9 (str.++ T4_9 T5_9)))
(assert (not (str.in.re T4_9 (str.to.re ";"))))
(assert (= T5_9 ";"))
(assert T_8)
(assert (= T_8 (< PCTEMP_LHS_2 0)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert T_6)
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert (= PCTEMP_LHS_2 (+ I0_9 PCTEMP_LHS_1)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= PCTEMP_LHS_1 (str.len T0_9))) ; len 0
(assert (= I0_9 (str.len T4_9))) ; len 1
(assert T_SELECT_2)
(check-sat)
