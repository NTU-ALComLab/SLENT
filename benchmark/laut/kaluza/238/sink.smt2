(set-logic QF_S)
(declare-fun T_SELECT_1 () Bool)
(declare-fun I0_8 () Int)
(declare-fun I1_8 () Int)
(declare-fun I2_8 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T_4 () Int)
(declare-fun T0_2 () String)
(declare-fun T1_2 () String)
(declare-fun var_0xINPUT_302 () String)
(assert (= var_0xINPUT_302 (str.++ T0_2 T1_2)))
(assert (not (str.in.re T1_2 (str.to.re "?"))))
(assert (>= I2_8 T_4))
(assert (= I2_8 I1_8))
(assert (<= I2_8 I1_8))
(assert (= I0_8 (- I2_8 T_4)))
(assert (>= T_4 0))
(assert (= T_4 (+ PCTEMP_LHS_1 1)))
(assert (= PCTEMP_LHS_1 (- 1)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (= 0 (str.len T0_2))) ; len 0
(assert (not T_SELECT_1))
(check-sat)
