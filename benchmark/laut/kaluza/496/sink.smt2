(set-logic QF_S)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_22 () Bool)
(declare-fun T_23 () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun T_c () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun I0_23 () Int)
(declare-fun I1_23 () Int)
(declare-fun I2_23 () Int)
(declare-fun PCTEMP_LHS_4 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun T_10 () Int)
(declare-fun NEW_DGNode_41 () String)
(declare-fun NEW_DGNode_45 () String)
(declare-fun NEW_DGNode_49 () String)
(declare-fun NEW_DGNode_53 () String)
(declare-fun NEW_DGNode_57 () String)
(declare-fun NEW_DGNode_61 () String)
(declare-fun NEW_DGNode_65 () String)
(declare-fun NEW_DGNode_66 () String)
(declare-fun PCTEMP_LHS_6 () String)
(declare-fun T_1c () String)
(declare-fun T_1d () String)
(declare-fun T_1e () String)
(declare-fun T_1f () String)
(declare-fun T_20 () String)
(declare-fun T_21 () String)
(assert (= T_21 NEW_DGNode_65))
(assert (= NEW_DGNode_65 NEW_DGNode_66))
(assert (= NEW_DGNode_65 ""))
(assert (= NEW_DGNode_66 (str.++ T_20 NEW_DGNode_61)))
(assert (= T_20 (str.++ T_1f NEW_DGNode_57)))
(assert (= T_1f (str.++ T_1e NEW_DGNode_53)))
(assert (= T_1e (str.++ T_1d NEW_DGNode_49)))
(assert (= T_1d (str.++ T_1c NEW_DGNode_45)))
(assert (= T_1c (str.++ NEW_DGNode_41 PCTEMP_LHS_6)))
(assert (= NEW_DGNode_41 "__utmb="))
(assert (= PCTEMP_LHS_6 "-"))
(assert (= NEW_DGNode_45 ";"))
(assert (= NEW_DGNode_49 "/"))
(assert (= NEW_DGNode_53 ";"))
(assert (= NEW_DGNode_57 ""))
(assert (= NEW_DGNode_61 ""))
(assert T_22)
(assert T_23)
(assert (= T_23 (not T_22)))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_5 (- 1)))))
(assert T_f)
(assert (= T_f (not T_e)))
(assert (= T_e (< PCTEMP_LHS_5 0)))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_4 (- 1)))))
(assert T_c)
(assert (= T_c (< (- 1) PCTEMP_LHS_4)))
(assert (= T_10 (+ PCTEMP_LHS_4 7)))
(assert (>= T_10 0))
(assert (>= PCTEMP_LHS_5 T_10))
(assert (= I2_23 I1_23))
(assert (<= PCTEMP_LHS_5 I1_23))
(assert (= I0_23 (- PCTEMP_LHS_5 T_10)))
(assert (= I0_23 (str.len PCTEMP_LHS_6))) ; len 0
(assert T_12)
(assert T_13)
(assert (= T_13 (not T_12)))
(check-sat)
