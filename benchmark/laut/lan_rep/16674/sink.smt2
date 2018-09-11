(set-logic QF_S)
(declare-fun sigmaStar_048 () String)
(declare-fun b_sigmaStar_048 () Bool)
(declare-fun literal_5 () String)
(declare-fun b_literal_5 () Bool)
(declare-fun atkPtn () String)
(declare-fun b_atkPtn () Bool)
(declare-fun x_4 () String)
(declare-fun b_x_4 () Bool)
(declare-fun x_6 () String)
(declare-fun b_x_6 () Bool)
(declare-fun sink () String)
(declare-fun b_sink () Bool)
(declare-fun atk_sigmaStar_1 () String)
(declare-fun atk_sigmaStar_2 () String)
(declare-fun atk_sink () String)

(assert (and b_literal_5 (= literal_5 "\x2e\x70\x68\x70")))
(assert (str.in.re atkPtn (str.to.re "/evil")))
(assert (= atk_sink (str.++ atk_sigmaStar_1 (str.++ atkPtn atk_sigmaStar_2))))
(assert (= b_x_4 (and (= x_4 (str.replace sigmaStar_048 X "") ) b_sigmaStar_048) ))
(assert (= b_x_6 (and (= x_6 (str.++ x_4 literal_5) ) b_x_4 b_literal_5) ))
(assert (and (= sink x_6) (= sink atk_sink) b_x_6))
(assert (> 40 (+ (str.len sigmaStar_048) (str.len sink)) ) )
(check-sat)
