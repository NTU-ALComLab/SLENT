(set-logic QF_S)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun I0_4 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_4 () String)
(declare-fun T1_4 () String)
(declare-fun T2_4 () String)
(declare-fun T3_4 () String)
(declare-fun T4_4 () String)
(declare-fun T5_4 () String)
(declare-fun var_0xINPUT_302 () String)
(assert (= var_0xINPUT_302 (str.++ T0_4 T1_4)))
(assert (= T1_4 (str.++ T2_4 T3_4)))
(assert (= T2_4 (str.++ T4_4 T5_4)))
(assert (not (str.in.re T4_4 (str.to.re "#"))))
(assert (= T5_4 "#"))
(assert T_4)
(assert (= T_4 (not T_3)))
(assert (= T_3 (= PCTEMP_LHS_2 (- 1))))
(assert (= PCTEMP_LHS_2 (+ I0_4 0)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= 0 (str.len T0_4))) ; len 0
(assert (= I0_4 (str.len T4_4))) ; len 1
(assert T_SELECT_2)
(check-sat)
