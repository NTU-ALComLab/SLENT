(set-logic QF_S)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_a () Bool)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_4 () String)
(declare-fun T1_4 () String)
(declare-fun var_0xINPUT_133872 () String)
(assert (= var_0xINPUT_133872 (str.++ T0_4 T1_4)))
(assert (not (str.in.re T1_4 (str.to.re "__utmb=16886264"))))
(assert T_a)
(assert (= T_a (not T_9)))
(assert (= T_9 (< (- 1) PCTEMP_LHS_2)))
(assert (= PCTEMP_LHS_2 (- 1)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= 0 (str.len T0_4))) ; len 0
(assert (not T_SELECT_2))
(check-sat)
