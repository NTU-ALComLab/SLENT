(set-logic QF_S)
(declare-fun T_102 () Bool)
(declare-fun T_66 () Bool)
(declare-fun T_67 () Bool)
(declare-fun T_SELECT_10 () Bool)
(declare-fun I0_154 () Int)
(declare-fun I0_158 () Int)
(declare-fun I1_154 () Int)
(declare-fun I1_158 () Int)
(declare-fun I2_154 () Int)
(declare-fun I2_158 () Int)
(declare-fun PCTEMP_LHS_40 () Int)
(declare-fun T_6b () Int)
(declare-fun NEW_DGNode_705 () String)
(declare-fun NEW_DGNode_706 () String)
(declare-fun NEW_DGNode_708 () String)
(declare-fun NEW_DGNode_709 () String)
(declare-fun NEW_DGNode_780 () String)
(declare-fun NEW_DGNode_781 () String)
(declare-fun PCTEMP_LHS_103 () String)
(assert (= PCTEMP_LHS_103 NEW_DGNode_781))
(assert (= NEW_DGNode_781 NEW_DGNode_780))
(assert (= NEW_DGNode_781 NEW_DGNode_709))
(assert (= NEW_DGNode_709 NEW_DGNode_708))
(assert (= NEW_DGNode_709 NEW_DGNode_706))
(assert (= NEW_DGNode_706 NEW_DGNode_705))
(assert (not (str.in.re NEW_DGNode_706 (str.to.re ""))))
(assert (not (str.in.re NEW_DGNode_705 (str.to.re "+"))))
(assert (not (str.in.re NEW_DGNode_708 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_780 (str.to.re "%"))))
(assert (= T_SELECT_10 (not (= PCTEMP_LHS_40 (- 1)))))
(assert T_67)
(assert (= T_67 (not T_66)))
(assert (= T_66 (= PCTEMP_LHS_40 (- 1))))
(assert (= I0_154 (- PCTEMP_LHS_40 0)))
(assert (>= PCTEMP_LHS_40 0))
(assert (= I2_154 I1_154))
(assert (<= PCTEMP_LHS_40 I1_154))
(assert (= T_6b (+ PCTEMP_LHS_40 1)))
(assert (>= T_6b 0))
(assert (>= I2_158 T_6b))
(assert (= I2_158 I1_158))
(assert (<= I2_158 I1_158))
(assert (= I0_158 (- I2_158 T_6b)))
(assert (= I0_158 (str.len PCTEMP_LHS_103))) ; len 0
(assert T_102)
(check-sat)
