(set-logic QF_S)
(declare-fun T_10 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun I0_18 () Int)
(declare-fun I1_18 () Int)
(declare-fun I2_18 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T_b () Int)
(declare-fun T_c () Int)
(declare-fun NEW_DGNode_17 () String)
(declare-fun NEW_DGNode_18 () String)
(declare-fun NEW_DGNode_20 () String)
(declare-fun NEW_DGNode_21 () String)
(declare-fun NEW_DGNode_5 () String)
(declare-fun NEW_DGNode_6 () String)
(declare-fun NEW_DGNode_8 () String)
(declare-fun NEW_DGNode_9 () String)
(declare-fun PCTEMP_LHS_4 () String)
(declare-fun T1_18 () String)
(declare-fun T2_18 () String)
(declare-fun T3_18 () String)
(declare-fun var_0xINPUT_16865 () String)
(assert (= var_0xINPUT_16865 NEW_DGNode_8))
(assert (= NEW_DGNode_8 NEW_DGNode_9))
(assert (= NEW_DGNode_8 NEW_DGNode_5))
(assert (= NEW_DGNode_5 NEW_DGNode_6))
(assert (= NEW_DGNode_5 ""))
(assert (= NEW_DGNode_6 ""))
(assert (= NEW_DGNode_9 (str.++ T1_18 T2_18)))
(assert (= T2_18 (str.++ PCTEMP_LHS_4 T3_18)))
(assert (= PCTEMP_LHS_4 NEW_DGNode_20))
(assert (= NEW_DGNode_20 NEW_DGNode_21))
(assert (= NEW_DGNode_20 NEW_DGNode_18))
(assert (= NEW_DGNode_18 NEW_DGNode_17))
(assert (not (str.in.re NEW_DGNode_18 (str.to.re "-"))))
(assert (not (str.in.re NEW_DGNode_17 (str.to.re "-"))))
(assert (= NEW_DGNode_21 "-"))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert T_8)
(assert (= T_8 (< (- 1) PCTEMP_LHS_2)))
(assert (= T_b (+ PCTEMP_LHS_2 7)))
(assert (>= T_b 0))
(assert (>= T_c T_b))
(assert (= I0_18 (- T_c T_b)))
(assert (= I2_18 I1_18))
(assert (<= T_c I1_18))
(assert (= T_c (str.len var_0xINPUT_16865))) ; len 0
(assert (= I1_18 (str.len var_0xINPUT_16865))) ; len 0
(assert T_3)
(assert T_4)
(assert (= T_4 (not T_3)))
(assert T_5)
(assert T_6)
(assert (= T_6 (not T_5)))
(assert (= T_b (str.len T1_18))) ; len 1
(assert (= I0_18 (str.len PCTEMP_LHS_4))) ; len 2
(assert T_e)
(assert T_f)
(assert T_10)
(check-sat)
