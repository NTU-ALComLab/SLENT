(set-logic QF_S)
(declare-fun T_SELECT_19 () Bool)
(declare-fun T_db () Bool)
(declare-fun T_dc () Bool)
(declare-fun I0_325 () Int)
(declare-fun I0_329 () Int)
(declare-fun I1_325 () Int)
(declare-fun I1_329 () Int)
(declare-fun I2_325 () Int)
(declare-fun I2_329 () Int)
(declare-fun PCTEMP_LHS_85 () Int)
(declare-fun T_e0 () Int)
(declare-fun NEW_DGNode_765 () String)
(declare-fun NEW_DGNode_766 () String)
(declare-fun PCTEMP_LHS_89 () String)
(assert (= PCTEMP_LHS_89 NEW_DGNode_766))
(assert (= NEW_DGNode_766 NEW_DGNode_765))
(assert (not (str.in.re NEW_DGNode_766 (str.to.re "+"))))
(assert (not (str.in.re NEW_DGNode_765 (str.to.re "%"))))
(assert (= T_SELECT_19 (not (= PCTEMP_LHS_85 (- 1)))))
(assert T_dc)
(assert (= T_dc (not T_db)))
(assert (= T_db (= PCTEMP_LHS_85 (- 1))))
(assert (= I0_325 (- PCTEMP_LHS_85 0)))
(assert (>= PCTEMP_LHS_85 0))
(assert (= I2_325 I1_325))
(assert (<= PCTEMP_LHS_85 I1_325))
(assert (= T_e0 (+ PCTEMP_LHS_85 1)))
(assert (>= T_e0 0))
(assert (>= I2_329 T_e0))
(assert (= I2_329 I1_329))
(assert (<= I2_329 I1_329))
(assert (= I0_329 (- I2_329 T_e0)))
(assert (= I0_329 (str.len PCTEMP_LHS_89))) ; len 0
(check-sat)
