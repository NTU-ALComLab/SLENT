(set-logic QF_S)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
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
(declare-fun NEW_DGNode_17 () String)
(declare-fun NEW_DGNode_21 () String)
(declare-fun NEW_DGNode_25 () String)
(declare-fun NEW_DGNode_29 () String)
(declare-fun NEW_DGNode_33 () String)
(declare-fun NEW_DGNode_37 () String)
(declare-fun PCTEMP_LHS_6 () String)
(declare-fun T_14 () String)
(declare-fun T_15 () String)
(declare-fun T_16 () String)
(declare-fun T_17 () String)
(declare-fun T_18 () String)
(declare-fun T_19 () String)
(assert (= T_19 (str.++ T_18 NEW_DGNode_37)))
(assert (= T_18 (str.++ T_17 NEW_DGNode_33)))
(assert (= T_17 (str.++ T_16 NEW_DGNode_29)))
(assert (= T_16 (str.++ T_15 NEW_DGNode_25)))
(assert (= T_15 (str.++ T_14 NEW_DGNode_21)))
(assert (= T_14 (str.++ NEW_DGNode_17 PCTEMP_LHS_6)))
(assert (= NEW_DGNode_17 "__utmb="))
(assert (= PCTEMP_LHS_6 "-"))
(assert (= NEW_DGNode_21 ";"))
(assert (= NEW_DGNode_25 "/"))
(assert (= NEW_DGNode_29 ";"))
(assert (= NEW_DGNode_33 ""))
(assert (= NEW_DGNode_37 ""))
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