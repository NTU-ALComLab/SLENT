(set-logic QF_S)
(declare-fun T_33 () Bool)
(declare-fun T_3b () Bool)
(declare-fun T_SELECT_7 () Bool)
(declare-fun T_SELECT_8 () Bool)
(declare-fun I0_127 () Int)
(declare-fun I0_97 () Int)
(declare-fun I1_127 () Int)
(declare-fun I2_127 () Int)
(declare-fun PCTEMP_LHS_8 () Int)
(declare-fun PCTEMP_LHS_9 () Int)
(declare-fun T_42 () Int)
(declare-fun T_49 () Int)
(declare-fun T0_97 () String)
(declare-fun T1_97 () String)
(declare-fun T2_97 () String)
(declare-fun T3_97 () String)
(declare-fun T4_97 () String)
(declare-fun T5_97 () String)
(declare-fun T_39 () String)
(assert (= T_39 (str.++ T0_97 T1_97)))
(assert (= T1_97 (str.++ T2_97 T3_97)))
(assert (= T2_97 (str.++ T4_97 T5_97)))
(assert (not (str.in.re T4_97 (str.to.re ";"))))
(assert (= T5_97 ";"))
(assert T_3b)
(assert (= T_3b (< PCTEMP_LHS_9 0)))
(assert (= T_SELECT_7 (not (= PCTEMP_LHS_8 (- 1)))))
(assert T_33)
(assert (= T_33 (< (- 1) PCTEMP_LHS_8)))
(assert (>= T_49 T_42))
(assert (= I2_127 I1_127))
(assert (<= T_49 I1_127))
(assert (= I0_127 (- T_49 T_42)))
(assert (>= T_42 0))
(assert (= T_42 (+ PCTEMP_LHS_8 7)))
(assert (= PCTEMP_LHS_9 (+ I0_97 PCTEMP_LHS_8)))
(assert (= T_SELECT_8 (not (= PCTEMP_LHS_9 (- 1)))))
(assert (= PCTEMP_LHS_8 (str.len T0_97))) ; len 0
(assert (= I0_97 (str.len T4_97))) ; len 1
(assert T_SELECT_8)
(check-sat)