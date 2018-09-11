(set-logic QF_S)
(declare-fun T_2 () Bool)
(declare-fun T_1 () Int)
(declare-fun var_0xINPUT_358162 () String)
(assert T_2)
(assert (= T_2 (= T_1 0)))
(assert (= T_1 (str.len var_0xINPUT_358162))) ; len 0
(check-sat)
