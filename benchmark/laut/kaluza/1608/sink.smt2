(set-logic QF_S)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_10 () String)
(declare-fun T1_10 () String)
(declare-fun var_0xINPUT_36617 () String)
(assert (= var_0xINPUT_36617 (str.++ T0_10 T1_10)))
(assert (not (str.in.re T1_10 (str.to.re "__utma=218069774."))))
(assert T_9)
(assert (= T_9 (< (- 1) PCTEMP_LHS_2)))
(assert (= PCTEMP_LHS_2 (- 1)))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (= 0 (str.len T0_10))) ; len 0
(assert (not T_SELECT_2))
(check-sat)
