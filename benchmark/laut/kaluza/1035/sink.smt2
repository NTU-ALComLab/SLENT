(set-logic QF_S)
(declare-fun T_25 () Bool)
(declare-fun T_26 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun I0_59 () Int)
(declare-fun I0_63 () Int)
(declare-fun I1_59 () Int)
(declare-fun I1_63 () Int)
(declare-fun I2_59 () Int)
(declare-fun I2_63 () Int)
(declare-fun PCTEMP_LHS_15 () Int)
(declare-fun T_2a () Int)
(declare-fun NEW_DGNode_1041 () String)
(declare-fun NEW_DGNode_1042 () String)
(declare-fun PCTEMP_LHS_19 () String)
(assert (= PCTEMP_LHS_19 NEW_DGNode_1042))
(assert (= NEW_DGNode_1042 NEW_DGNode_1041))
(assert (not (str.in.re NEW_DGNode_1042 (str.to.re "+"))))
(assert (not (str.in.re NEW_DGNode_1041 (str.to.re "%"))))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_15 (- 1)))))
(assert T_26)
(assert (= T_26 (not T_25)))
(assert (= T_25 (= PCTEMP_LHS_15 (- 1))))
(assert (= I0_59 (- PCTEMP_LHS_15 0)))
(assert (>= PCTEMP_LHS_15 0))
(assert (= I2_59 I1_59))
(assert (<= PCTEMP_LHS_15 I1_59))
(assert (= T_2a (+ PCTEMP_LHS_15 1)))
(assert (>= T_2a 0))
(assert (>= I2_63 T_2a))
(assert (= I2_63 I1_63))
(assert (<= I2_63 I1_63))
(assert (= I0_63 (- I2_63 T_2a)))
(assert (= I0_63 (str.len PCTEMP_LHS_19))) ; len 0
(check-sat)
