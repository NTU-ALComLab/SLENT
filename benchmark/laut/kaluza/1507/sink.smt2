(set-logic QF_S)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun T_c () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun I0_18 () Int)
(declare-fun I0_23 () Int)
(declare-fun I1_23 () Int)
(declare-fun I2_23 () Int)
(declare-fun PCTEMP_LHS_4 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun T_10 () Int)
(declare-fun T0_18 () String)
(declare-fun T1_18 () String)
(declare-fun T2_18 () String)
(declare-fun T3_18 () String)
(declare-fun T4_18 () String)
(declare-fun T5_18 () String)
(declare-fun var_0xINPUT_12454 () String)
(assert (= var_0xINPUT_12454 (str.++ T0_18 T1_18)))
(assert (= T1_18 (str.++ T2_18 T3_18)))
(assert (= T2_18 (str.++ T4_18 T5_18)))
(assert (not (str.in.re T4_18 (str.to.re ";"))))
(assert (= T5_18 ";"))
(assert T_f)
(assert (= T_f (not T_e)))
(assert (= T_e (< PCTEMP_LHS_5 0)))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_4 (- 1)))))
(assert T_c)
(assert (= T_c (< (- 1) PCTEMP_LHS_4)))
(assert (= PCTEMP_LHS_5 (+ I0_18 PCTEMP_LHS_4)))
(assert (= T_10 (+ PCTEMP_LHS_4 7)))
(assert (>= T_10 0))
(assert (>= PCTEMP_LHS_5 T_10))
(assert (= I0_23 (- PCTEMP_LHS_5 T_10)))
(assert (= I2_23 I1_23))
(assert (<= PCTEMP_LHS_5 I1_23))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_5 (- 1)))))
(assert (= PCTEMP_LHS_4 (str.len T0_18))) ; len 0
(assert (= I0_18 (str.len T4_18))) ; len 1
(assert T_SELECT_5)
(check-sat)
