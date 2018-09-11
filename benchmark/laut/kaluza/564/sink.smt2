(set-logic QF_S)
(declare-fun T_11c () Bool)
(declare-fun T_11d () Bool)
(declare-fun T_SELECT_24 () Bool)
(declare-fun I0_420 () Int)
(declare-fun I0_424 () Int)
(declare-fun I1_420 () Int)
(declare-fun I1_424 () Int)
(declare-fun I2_420 () Int)
(declare-fun I2_424 () Int)
(declare-fun PCTEMP_LHS_110 () Int)
(declare-fun T_121 () Int)
(declare-fun NEW_DGNode_1174 () String)
(declare-fun NEW_DGNode_1175 () String)
(declare-fun PCTEMP_LHS_158 () String)
(assert (= PCTEMP_LHS_158 NEW_DGNode_1175))
(assert (= NEW_DGNode_1175 NEW_DGNode_1174))
(assert (not (str.in.re NEW_DGNode_1175 (str.to.re "+"))))
(assert (not (str.in.re NEW_DGNode_1174 (str.to.re "%"))))
(assert (= T_SELECT_24 (not (= PCTEMP_LHS_110 (- 1)))))
(assert T_11d)
(assert (= T_11d (not T_11c)))
(assert (= T_11c (= PCTEMP_LHS_110 (- 1))))
(assert (= I0_420 (- PCTEMP_LHS_110 0)))
(assert (>= PCTEMP_LHS_110 0))
(assert (= I2_420 I1_420))
(assert (<= PCTEMP_LHS_110 I1_420))
(assert (= T_121 (+ PCTEMP_LHS_110 1)))
(assert (>= T_121 0))
(assert (>= I2_424 T_121))
(assert (= I2_424 I1_424))
(assert (<= I2_424 I1_424))
(assert (= I0_424 (- I2_424 T_121)))
(assert (= I0_424 (str.len PCTEMP_LHS_158))) ; len 0
(check-sat)
