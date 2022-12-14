<?PHP
/*

	ネイバーズスポーツ　メインカテゴリー2一覧

*/
function goods_mainlist2($VALUE, $CHECK) {
	global $m_cate_file,$index;

	//	表示確認
	unset($CLASS_M_L);
	$file = "./".DIR_CATE."/category.inc";
	$LIST = file($file);
	foreach ($LIST AS $VAL) {
		$VAL = trim($VAL);
		list($h_num, $mc_num, $cate_name, $cate_title) = explode("<>", $VAL);
		if ($mc_num == 99) { continue; }	//	add ookawara 2015/01/29
		$CLASS_M_L[$h_num]['mc_num'] = $mc_num;
		$CLASS_M_L[$h_num]['cate_name'] = $cate_name;
	}

	//	カテゴリー表示
	if ($CLASS_M_L) {
		$html  = "<h2 class=\"title-nbs\">カテゴリー 一覧</h2>\n";

		//	表示可能カテゴリー読み込み
		//	del ookawara 2016/02/04
		//$sql  = "SELECT cate1, cate2, cate3 FROM ".T_CATE.
		//		" WHERE display='1' AND state!='3'".
		//		" AND cate1 NOT IN ('99')".	//	add ookawara 2013/08/23
		//		" GROUP BY cate1, cate2, cate3;";

		//	add ookawara 2016/02/04
		$sql  = "SELECT category.cate1, category.cate2, category.cate3 FROM ".T_CATE." category".
				" LEFT JOIN goods goods ON  category.g_num=goods.g_num".
				" WHERE category.display='1'".
				" AND category.state!='3'".
				" AND category.cate1 NOT IN ('99')".
				" AND goods.g_num>0".
				" GROUP BY category.cate1, category.cate2, category.cate3;";

		if ($result = pg_query(DB, $sql)) {
			while ($list = pg_fetch_array($result)) {
				$cate1_ = $list['cate1'];
				$cate2_ = $list['cate2'];
				$cate3_ = $list['cate3'];
				$CATE[$cate1_][$cate2_][$cate3_] = 1;
			}
		}

		//	サブカテゴリー読込 表示
		foreach ($CLASS_M_L AS $KEY => $VAL) {
			unset($mc_html);
			$flag = 0;
			$mc_num = (int)$CLASS_M_L[$KEY]['mc_num'];
			$cate_name = $CLASS_M_L[$KEY]['cate_name'];
			if ($CATE[$mc_num]) {
				//	メインカテゴリー表示
				$moji = 0;
				$moji = strlen($cate_name);
				$c_html  = "  <tr>\n";
				$c_html .= "    <th colspan=\"3\" class=\"cate1\"><a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$index."\" title=\"".$cate_name."\">".$cate_name." (".$moji.")</a></th>\n";
				$c_html .= "  </tr>\n";

				//	サブカテゴリー表示
				unset($LIST2);
				unset($sc_html);
				$file = "./".DIR_CATE."/".$mc_num.".dat";
				if (file_exists($file)) {
					$LIST2 = file($file);
				}
				if ($LIST2) { $sc_html = subcategory($CATE, $mc_num, $LIST2); }
			}
			if ($sc_html) {
				$html .= "<table id=\"category-table\">\n";
				$html .= $c_html;
				$html .= $sc_html;
				$html .= "</table>\n";
				$html .= "<br />\n";

				$flag = 1;
			}
		}
	}

	if ($flag == 0) {
		$html .= "<br />\n";
		$html .= "今現在表示出来るカテゴリーはありません。<br />\n";
	}

	return $html;

}



//	サブカテゴリー表示
function subcategory($CATE, $mc_num, $LIST2) {
	global $index;

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2, $sc_num, $sc_name, $cate_title2) = explode("<>", $VAL2);
		if ($CATE[$mc_num][$sc_num]) {
			$link = (int)sprintf("%02d", $sc_num);

			$moji = 0;
			$moji = strlen($sc_name);

			$html .= "  <tr>\n";
			$html .= "    <th colspan=\"3\" class=\"cate2\"><a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$link."/".$index."\" title=\"".$sc_name."\">".$sc_name." (".$moji.")</a></th>\n";
			$html .= "  </tr>\n";

			unset($LIST3);
			unset($ssc_html);
			$file = "./".DIR_CATE."/".$mc_num."_".$sc_num.".dat";
			if (file_exists($file)) {
				$LIST3 = file($file);
			}
			if ($LIST3) { $ssc_html = sub2category($CATE, $mc_num, $sc_num, $LIST3); }
			$html .= $ssc_html;
		}
	}

	if (!$ssc_html) { unset($html); }

	return $html;
}



//	サブカテゴリー2表示
function sub2category($CATE, $mc_num, $sc_num, $LIST3) {
	global $index;

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	$i = 1;
	$flag = 0;
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num, $s2c_num, $s2c_name, $s2c_file) = explode("<>", $VAL);
		if ($CATE[$mc_num][$sc_num][$s2c_num]) {
			$flag = 1;
			$amari = $i % 3;
			if ($amari == 1 && $amari != "$b_amari") {
					$html .= "  <tr>\n";
			}
			$link = (int)sprintf("%02d",$sc_num);
			$link2 = (int)sprintf("%02d",$s2c_num);

			$moji = 0;
			$moji = strlen($s2c_name);
			$html .= "<td><a href=\""./".GOODS_SCRIPT."/".$mc_num."/".$link."/".$link2."/".$index."\" title=\"".$s2c_name."\">".$s2c_name." (".$moji.")</a></td>\n";
			if ($amari == 0 && $amari != "$b_amari") {
				$html .= "  </tr>\n";
			}
		} else {
			continue;
		}

		$b_amari = $amari;
		$i++;
	}
	++$b_amari;
	if ($b_amari == 3) { $b_amari = 0; }
	if ($flag != 1) {
		unset($html);
	}
	elseif ($amari != 0 && $amari != $b_amari) {
		$max = 3 - $amari;
		for ($ii=1; $ii<=$max; $ii++) {
			$html .= "    <td>&nbsp;</td>\n";
		}
		$html .= "  </tr>\n";
	}

	return $html;
}
?>
