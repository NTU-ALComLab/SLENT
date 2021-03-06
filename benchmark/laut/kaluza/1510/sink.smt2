(set-logic QF_S)
(declare-fun T_18 () Bool)
(declare-fun T_19 () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun I0_40 () Int)
(declare-fun I0_44 () Int)
(declare-fun I1_40 () Int)
(declare-fun I1_44 () Int)
(declare-fun I2_40 () Int)
(declare-fun I2_44 () Int)
(declare-fun PCTEMP_LHS_10 () Int)
(declare-fun T_1d () Int)
(declare-fun NEW_DGNode_1039 () String)
(declare-fun NEW_DGNode_1040 () String)
(declare-fun NEW_DGNode_1054 () String)
(declare-fun NEW_DGNode_1055 () String)
(declare-fun NEW_DGNode_1201 () String)
(declare-fun NEW_DGNode_1202 () String)
(declare-fun NEW_DGNode_1264 () String)
(declare-fun NEW_DGNode_1265 () String)
(declare-fun NEW_DGNode_1327 () String)
(declare-fun NEW_DGNode_1328 () String)
(declare-fun NEW_DGNode_1390 () String)
(declare-fun NEW_DGNode_1391 () String)
(declare-fun NEW_DGNode_1453 () String)
(declare-fun NEW_DGNode_1454 () String)
(declare-fun NEW_DGNode_1516 () String)
(declare-fun NEW_DGNode_1517 () String)
(declare-fun NEW_DGNode_271 () String)
(declare-fun NEW_DGNode_272 () String)
(declare-fun PCTEMP_LHS_11 () String)
(declare-fun PCTEMP_LHS_177 () String)
(declare-fun T1_40 () String)
(declare-fun T1_44 () String)
(declare-fun T2_40 () String)
(declare-fun T2_44 () String)
(declare-fun T3_40 () String)
(declare-fun T3_44 () String)
(declare-fun T_1c () String)
(assert (= T_1c NEW_DGNode_1040))
(assert (= NEW_DGNode_1040 NEW_DGNode_1039))
(assert (= NEW_DGNode_1040 NEW_DGNode_271))
(assert (= NEW_DGNode_271 NEW_DGNode_272))
(assert (= NEW_DGNode_271 (str.++ T1_40 T2_40)))
(assert (= T2_40 (str.++ PCTEMP_LHS_11 T3_40)))
(assert (= NEW_DGNode_272 (str.++ T1_44 T2_44)))
(assert (= T2_44 (str.++ PCTEMP_LHS_177 T3_44)))
(assert (= PCTEMP_LHS_177 NEW_DGNode_1517))
(assert (= NEW_DGNode_1517 NEW_DGNode_1516))
(assert (= NEW_DGNode_1517 NEW_DGNode_1454))
(assert (= NEW_DGNode_1454 NEW_DGNode_1453))
(assert (= NEW_DGNode_1454 NEW_DGNode_1391))
(assert (= NEW_DGNode_1391 NEW_DGNode_1390))
(assert (= NEW_DGNode_1391 NEW_DGNode_1328))
(assert (= NEW_DGNode_1328 NEW_DGNode_1327))
(assert (= NEW_DGNode_1328 NEW_DGNode_1265))
(assert (= NEW_DGNode_1265 NEW_DGNode_1264))
(assert (= NEW_DGNode_1265 NEW_DGNode_1202))
(assert (= NEW_DGNode_1202 NEW_DGNode_1201))
(assert (= NEW_DGNode_1202 NEW_DGNode_1055))
(assert (= NEW_DGNode_1055 NEW_DGNode_1054))
(assert (not (str.in.re NEW_DGNode_1055 (str.to.re "+"))))
(assert (not (str.in.re NEW_DGNode_1054 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1201 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1264 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1327 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1390 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1453 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1516 (str.to.re "%"))))
(assert (not (str.in.re NEW_DGNode_1039 (str.to.re "&"))))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_10 (- 1)))))
(assert T_19)
(assert (= T_19 (not T_18)))
(assert (= T_18 (= PCTEMP_LHS_10 (- 1))))
(assert (= I0_40 (- PCTEMP_LHS_10 0)))
(assert (>= PCTEMP_LHS_10 0))
(assert (>= I2_44 T_1d))
(assert (= I2_44 I1_44))
(assert (<= I2_44 I1_44))
(assert (= I0_44 (- I2_44 T_1d)))
(assert (>= T_1d 0))
(assert (= T_1d (+ PCTEMP_LHS_10 1)))
(assert (<= PCTEMP_LHS_10 I1_40))
(assert (= I2_40 I1_40))
(assert (= I1_40 (str.len T_1c))) ; len 0
(assert (= I1_44 (str.len T_1c))) ; len 0
(assert (= 0 (str.len T1_40))) ; len 1
(assert (= I0_40 (str.len PCTEMP_LHS_11))) ; len 2
(assert (= T_1d (str.len T1_44))) ; len 3
(assert (= I0_44 (str.len PCTEMP_LHS_177))) ; len 4
(check-sat)
