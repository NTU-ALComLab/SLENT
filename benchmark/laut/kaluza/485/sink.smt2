(set-logic QF_S)
(declare-fun T_15 () Bool)
(declare-fun T_SELECT_3 () Bool)
(declare-fun I0_27 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun T0_27 () String)
(declare-fun T1_27 () String)
(declare-fun T2_27 () String)
(declare-fun T3_27 () String)
(declare-fun T4_27 () String)
(declare-fun T5_27 () String)
(declare-fun var_0xINPUT_10765 () String)
(assert (= var_0xINPUT_10765 (str.++ T0_27 T1_27)))
(assert (= T1_27 (str.++ T2_27 T3_27)))
(assert (= T2_27 (str.++ T4_27 T5_27)))
(assert (not (str.in.re T4_27 (str.to.re "__utmx=169413169"))))
(assert (= T5_27 "__utmx=169413169"))
(assert T_15)
(assert (= T_15 (< (- 1) PCTEMP_LHS_5)))
(assert (= PCTEMP_LHS_5 (+ I0_27 0)))
(assert (= T_SELECT_3 (not (= PCTEMP_LHS_5 (- 1)))))
(assert (= 0 (str.len T0_27))) ; len 0
(assert (= I0_27 (str.len T4_27))) ; len 1
(assert T_SELECT_3)
(check-sat)
