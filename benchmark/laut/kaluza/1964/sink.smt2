(set-logic QF_S)
(declare-fun T_33 () Bool)
(declare-fun T_34 () Bool)
(declare-fun T_SELECT_7 () Bool)
(declare-fun PCTEMP_LHS_8 () Int)
(declare-fun T0_82 () String)
(declare-fun T1_82 () String)
(declare-fun T_31 () String)
(assert (= T_31 (str.++ T0_82 T1_82)))
(assert (not (str.in.re T1_82 (str.to.re "__utmz=169413169."))))
(assert T_34)
(assert (= T_34 (not T_33)))
(assert (= T_33 (< (- 1) PCTEMP_LHS_8)))
(assert (= PCTEMP_LHS_8 (- 1)))
(assert (= T_SELECT_7 (not (= PCTEMP_LHS_8 (- 1)))))
(assert (= 0 (str.len T0_82))) ; len 0
(assert (not T_SELECT_7))
(check-sat)
