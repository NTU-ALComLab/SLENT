(set-logic QF_S)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_a () Bool)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_12 () String)
(declare-fun T1_12 () String)
(declare-fun T_7 () String)
(assert (= T_7 (str.++ T0_12 T1_12)))
(assert (not (str.in.re T1_12 (str.to.re "utm_id="))))
(assert T_a)
(assert (= T_a (not T_9)))
(assert (= T_9 (< (- 1) PCTEMP_LHS_1)))
(assert (= PCTEMP_LHS_1 (- 1)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (= 0 (str.len T0_12))) ; len 0
(assert (not T_SELECT_1))
(check-sat)