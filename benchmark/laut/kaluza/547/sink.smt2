(set-logic QF_S)
(declare-fun T_68 () Bool)
(declare-fun T_70 () Bool)
(declare-fun T_71 () Bool)
(declare-fun T_SELECT_10 () Bool)
(declare-fun T_SELECT_9 () Bool)
(declare-fun I0_207 () Int)
(declare-fun I1_207 () Int)
(declare-fun I2_207 () Int)
(declare-fun PCTEMP_LHS_11 () Int)
(declare-fun PCTEMP_LHS_12 () Int)
(declare-fun T_78 () Int)
(declare-fun T0_175 () String)
(declare-fun T1_175 () String)
(declare-fun T_66 () String)
(assert (= T_66 (str.++ T0_175 T1_175)))
(assert (not (str.in.re T1_175 (str.to.re "__utma=169413169."))))
(assert T_68)
(assert (= T_68 (< (- 1) PCTEMP_LHS_11)))
(assert (= T_SELECT_10 (not (= PCTEMP_LHS_12 (- 1)))))
(assert T_71)
(assert (= T_71 (not T_70)))
(assert (= T_70 (< PCTEMP_LHS_12 0)))
(assert (>= PCTEMP_LHS_12 T_78))
(assert (= I2_207 I1_207))
(assert (<= PCTEMP_LHS_12 I1_207))
(assert (= I0_207 (- PCTEMP_LHS_12 T_78)))
(assert (>= T_78 0))
(assert (= T_78 (+ PCTEMP_LHS_11 7)))
(assert (= PCTEMP_LHS_11 (- 1)))
(assert (= T_SELECT_9 (not (= PCTEMP_LHS_11 (- 1)))))
(assert (= 0 (str.len T0_175))) ; len 0
(assert (not T_SELECT_9))
(check-sat)
