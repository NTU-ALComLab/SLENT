(set-logic QF_S)
(declare-fun T_1c () Bool)
(declare-fun T_1e () Bool)
(declare-fun T_1f () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun I0_43 () Int)
(declare-fun I1_43 () Int)
(declare-fun I2_43 () Int)
(declare-fun PCTEMP_LHS_6 () Int)
(declare-fun PCTEMP_LHS_7 () Int)
(declare-fun T_20 () Int)
(declare-fun T0_38 () String)
(declare-fun T1_38 () String)
(declare-fun var_0xINPUT_37425 () String)
(assert (= var_0xINPUT_37425 (str.++ T0_38 T1_38)))
(assert (not (str.in.re T1_38 (str.to.re ";"))))
(assert T_1f)
(assert (= T_1f (not T_1e)))
(assert (= T_1e (< PCTEMP_LHS_7 0)))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_6 (- 1)))))
(assert T_1c)
(assert (= T_1c (< (- 1) PCTEMP_LHS_6)))
(assert (= T_20 (+ PCTEMP_LHS_6 7)))
(assert (>= T_20 0))
(assert (>= PCTEMP_LHS_7 T_20))
(assert (= I0_43 (- PCTEMP_LHS_7 T_20)))
(assert (= I2_43 I1_43))
(assert (<= PCTEMP_LHS_7 I1_43))
(assert (= PCTEMP_LHS_7 (- 1)))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_7 (- 1)))))
(assert (= PCTEMP_LHS_6 (str.len T0_38))) ; len 0
(assert (not T_SELECT_5))
(check-sat)
