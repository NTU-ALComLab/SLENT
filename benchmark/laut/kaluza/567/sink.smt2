(set-logic QF_S)
(declare-fun T_1c () Bool)
(declare-fun T_22 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_c () Bool)
(declare-fun I0_14 () Int)
(declare-fun I0_44 () Int)
(declare-fun I1_14 () Int)
(declare-fun I1_44 () Int)
(declare-fun I2_14 () Int)
(declare-fun I2_44 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun PCTEMP_LHS_6 () Int)
(declare-fun T_1f () Int)
(declare-fun T_20 () Int)
(declare-fun T_31 () Int)
(declare-fun T_32 () Int)
(declare-fun T_a () Int)
(declare-fun NEW_DGNode_100 () String)
(declare-fun NEW_DGNode_101 () String)
(declare-fun NEW_DGNode_17 () String)
(declare-fun NEW_DGNode_21 () String)
(declare-fun NEW_DGNode_48 () String)
(declare-fun NEW_DGNode_52 () String)
(declare-fun NEW_DGNode_97 () String)
(declare-fun NEW_DGNode_98 () String)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun PCTEMP_LHS_8 () String)
(declare-fun T1_69 () String)
(declare-fun T1_70 () String)
(declare-fun T2_69 () String)
(declare-fun T2_70 () String)
(declare-fun T_23 () String)
(declare-fun T_30 () String)
(declare-fun T_d () String)
(assert (= T_30 (str.++ T1_70 T2_70)))
(assert (= T1_70 (str.++ T1_69 T2_69)))
(assert (= T1_69 ""))
(assert (= T2_69 NEW_DGNode_98))
(assert (= NEW_DGNode_98 NEW_DGNode_97))
(assert (= NEW_DGNode_98 (str.++ T_d NEW_DGNode_21)))
(assert (= T_d (str.++ NEW_DGNode_17 PCTEMP_LHS_3)))
(assert (= NEW_DGNode_17 "__utma="))
(assert (not (str.in.re PCTEMP_LHS_3 (str.to.re "-"))))
(assert (= NEW_DGNode_21 ";+"))
(assert (not (str.in.re NEW_DGNode_97 (str.to.re "%"))))
(assert (= T2_70 NEW_DGNode_101))
(assert (= NEW_DGNode_101 NEW_DGNode_100))
(assert (= NEW_DGNode_101 (str.++ T_23 NEW_DGNode_52)))
(assert (= T_23 (str.++ NEW_DGNode_48 PCTEMP_LHS_8)))
(assert (= NEW_DGNode_48 "__utmz="))
(assert (not (str.in.re PCTEMP_LHS_8 (str.to.re "-"))))
(assert (= NEW_DGNode_52 ";+"))
(assert (not (str.in.re NEW_DGNode_100 (str.to.re "%"))))
(assert (= T_32 (- T_31 1)))
(assert (= T_31 (str.len T_30))) ; len 0
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert T_9)
(assert (= T_9 (not T_8)))
(assert (= T_8 (< PCTEMP_LHS_2 0)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert T_6)
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_a (+ PCTEMP_LHS_1 7)))
(assert (>= T_a 0))
(assert (>= PCTEMP_LHS_2 T_a))
(assert (= I2_14 I1_14))
(assert (<= PCTEMP_LHS_2 I1_14))
(assert (= I0_14 (- PCTEMP_LHS_2 T_a)))
(assert (= I0_14 (str.len PCTEMP_LHS_3))) ; len 1
(assert T_c)
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_6 (- 1)))))
(assert T_1c)
(assert (= T_1c (< (- 1) PCTEMP_LHS_6)))
(assert (= T_1f (+ PCTEMP_LHS_6 7)))
(assert (>= T_1f 0))
(assert (>= T_20 T_1f))
(assert (= I2_44 I1_44))
(assert (<= T_20 I1_44))
(assert (= I0_44 (- T_20 T_1f)))
(assert (= I0_44 (str.len PCTEMP_LHS_8))) ; len 2
(assert T_22)
(check-sat)
