(set-logic QF_S)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun T_c () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun I0_15 () Int)
(declare-fun I0_23 () Int)
(declare-fun I1_23 () Int)
(declare-fun I2_23 () Int)
(declare-fun PCTEMP_LHS_4 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun T_10 () Int)
(declare-fun T0_15 () String)
(declare-fun T1_15 () String)
(declare-fun T2_15 () String)
(declare-fun T3_15 () String)
(declare-fun T4_15 () String)
(declare-fun T5_15 () String)
(declare-fun var_0xINPUT_15467 () String)
(assert (= var_0xINPUT_15467 (str.++ T0_15 T1_15)))
(assert (= T1_15 (str.++ T2_15 T3_15)))
(assert (= T2_15 (str.++ T4_15 T5_15)))
(assert (not (str.in.re T4_15 (str.to.re "__utmb=169413169"))))
(assert (= T5_15 "__utmb=169413169"))
(assert T_c)
(assert (= T_c (< (- 1) PCTEMP_LHS_4)))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_5 (- 1)))))
(assert T_f)
(assert (= T_f (not T_e)))
(assert (= T_e (< PCTEMP_LHS_5 0)))
(assert (>= PCTEMP_LHS_5 T_10))
(assert (= I2_23 I1_23))
(assert (<= PCTEMP_LHS_5 I1_23))
(assert (= I0_23 (- PCTEMP_LHS_5 T_10)))
(assert (>= T_10 0))
(assert (= T_10 (+ PCTEMP_LHS_4 7)))
(assert (= PCTEMP_LHS_4 (+ I0_15 0)))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_4 (- 1)))))
(assert (= 0 (str.len T0_15))) ; len 0
(assert (= I0_15 (str.len T4_15))) ; len 1
(assert T_SELECT_4)
(check-sat)
