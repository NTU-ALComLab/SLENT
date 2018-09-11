(set-logic QF_S)
(declare-fun T_5 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun I0_14 () Int)
(declare-fun I1_14 () Int)
(declare-fun I2_14 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T_9 () Int)
(declare-fun T_a () Int)
(declare-fun T0_8 () String)
(declare-fun T1_8 () String)
(declare-fun var_0xINPUT_47771 () String)
(assert (= var_0xINPUT_47771 (str.++ T0_8 T1_8)))
(assert (not (str.in.re T1_8 (str.to.re ";"))))
(assert T_8)
(assert (= T_8 (not T_7)))
(assert (= T_7 (< PCTEMP_LHS_2 0)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert T_5)
(assert (= T_5 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_9 (+ PCTEMP_LHS_1 6)))
(assert (= T_a (+ T_9 1)))
(assert (>= T_a 0))
(assert (>= PCTEMP_LHS_2 T_a))
(assert (= I0_14 (- PCTEMP_LHS_2 T_a)))
(assert (= I2_14 I1_14))
(assert (<= PCTEMP_LHS_2 I1_14))
(assert (= PCTEMP_LHS_2 (- 1)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= PCTEMP_LHS_1 (str.len T0_8))) ; len 0
(assert (not T_SELECT_2))
(check-sat)
