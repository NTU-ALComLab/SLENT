(set-logic QF_S)
(declare-fun T_3 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_3 () String)
(declare-fun T1_3 () String)
(declare-fun var_0xINPUT_5121 () String)
(assert (= var_0xINPUT_5121 (str.++ T0_3 T1_3)))
(assert (not (str.in.re T1_3 (str.to.re "?"))))
(assert T_3)
(assert (= T_3 (= PCTEMP_LHS_1 (- 1))))
(assert (= PCTEMP_LHS_1 (- 1)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (= 0 (str.len T0_3))) ; len 0
(assert (not T_SELECT_1))
(check-sat)
