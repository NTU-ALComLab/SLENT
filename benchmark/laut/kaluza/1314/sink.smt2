(set-logic QF_S)
(declare-fun T_29 () Bool)
(declare-fun T_2a () Bool)
(declare-fun T_SELECT_6 () Bool)
(declare-fun I0_48 () Int)
(declare-fun PCTEMP_LHS_8 () Int)
(declare-fun T0_48 () String)
(declare-fun T1_48 () String)
(declare-fun T2_48 () String)
(declare-fun T3_48 () String)
(declare-fun T4_48 () String)
(declare-fun T5_48 () String)
(declare-fun var_0xINPUT_150472 () String)
(assert (= var_0xINPUT_150472 (str.++ T0_48 T1_48)))
(assert (= T1_48 (str.++ T2_48 T3_48)))
(assert (= T2_48 (str.++ T4_48 T5_48)))
(assert (not (str.in.re T4_48 (str.to.re "__utmx=16886264"))))
(assert (= T5_48 "__utmx=16886264"))
(assert T_2a)
(assert (= T_2a (not T_29)))
(assert (= T_29 (< (- 1) PCTEMP_LHS_8)))
(assert (= PCTEMP_LHS_8 (+ I0_48 0)))
(assert (= T_SELECT_6 (not (= PCTEMP_LHS_8 (- 1)))))
(assert (= 0 (str.len T0_48))) ; len 0
(assert (= I0_48 (str.len T4_48))) ; len 1
(assert T_SELECT_6)
(check-sat)