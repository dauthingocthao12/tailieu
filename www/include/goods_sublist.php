<?PHP
/*

	ネイバーズスポーツ　サブカテゴリー一覧

*/
function goods_sublist($VALUE, $CHECK) {

	$cate1 = (int)preg_replace("/[^0-9]/", "", $CHECK['main']);
	//	表示可能カテゴリー読み込み
	//	del ookawara 2016/02/04
	//$sql  = "SELECT cate1, cate2, cate3 FROM ".T_CATE.
	//		" WHERE cate1='".$cate1."'".
	//		" AND display='1'".
	//		" AND state!='3'".
	//		" AND cate1 NOT IN ('99')".	//	add ookawara 2015/01/29
	//		" GROUP BY cate1, cate2, cate3;";

	//	add ookawara 2016/02/04
	$sql  = "SELECT category.cate1, category.cate2, category.cate3 FROM ".T_CATE." category".
			" LEFT JOIN goods goods ON  category.g_num=goods.g_num".
			" WHERE category.cate1='".$cate1."'".
			" AND category.display='1'".
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

	//	表示確認
	$file = "./".DIR_CATE."/category.inc";
	$LIST = file($file);
	foreach ($LIST AS $VAL) {
		$VAL = trim($VAL);
		list($h_num_, $mc_num_, $cate_name_, $cate_title_) = explode("<>", $VAL);
		if ($mc_num_ == 99) { continue; }	//	add ookawara 2015/01/29
		if ($CHECK['main'] && $CHECK['main'] != $mc_num_) {
			continue;
		} else {
			$mc_num = $mc_num_;
			$cate_name = $cate_name_;
			$cate_state = $cate_state_;
			break;
		}
	}

	//	カテゴリー表示
	if ($mc_num) {
		$html  = "<h2 class=\"title-nbs\">".$cate_name_." カテゴリー 一覧</h2>\n";
		if ($CATE[$mc_num]) {
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
			$html .= "<table id=\"category-list\">\n";
			$html .= $sc_html."\n";
			$html .= "</table>\n";
			// $html .= "<br />\n";
		} else {
			unset($html);
		}
	}

	if (!$html) {
		//$sent_url = "/".GOODS_SCRIPT."/";		//	del ookawara 2016/01/21
		//header ("Location: $sent_url\n\n");	//	del ookawara 2016/01/21
		header404();							//	add ookawara 2016/01/21
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

	$SUB_CATEGORY = array();
	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2, $sc_num, $sc_name, $cate_title2) = explode("<>", $VAL2);
		$SUB_CATEGORY[$h_num2] = $h_num2."<>".$sc_num."<>".$sc_name."<>".$cate_title2."<>";
	}
	krsort($SUB_CATEGORY);

	foreach ($SUB_CATEGORY AS $val) {
		list($h_num2, $sc_num, $sc_name, $cate_title2) = explode("<>",$val);
		if ($CATE[$mc_num][$sc_num]) {
			$link = (int)sprintf("%02d", $sc_num);
			$html .= "<tr>\n";
			$html .= "  <th colspan=\"3\" class=\"cate-2\"><a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$link."/".$index."\" title=\"".$sc_name."\">".$sc_name."</a></th>\n";
			$html .= "</tr>\n";

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
	$SUB_CATEGORY2 = array();
	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num, $s2c_num, $s2c_name, $s2c_file) = explode("<>", $VAL);
		$SUB_CATEGORY2[$h_num] = $h_num."<>".$s2c_num."<>".$s2c_name."<>".$s2c_file."<>";
	}
	krsort($SUB_CATEGORY2);

	foreach ($SUB_CATEGORY2 AS $val) {
		list($h_num, $s2c_num, $s2c_name, $s2c_file) = explode("<>",$val);
		if ($CATE[$mc_num][$sc_num][$s2c_num]) {
			$flag = 1;
			$amari = $i % 3;
			if ($amari == 1 && $amari != "$b_amari") {
					$html .= "  <tr>\n";
			}
			$link = (int)sprintf("%02d", $sc_num);
			$link2 = (int)sprintf("%02d", $s2c_num);
			$html .= "    <td>\n";
			$html .= "      <a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$link."/".$link2."/".$index."\" title=\"".$s2c_name."\">".$s2c_name."</a>\n";
			$html .= "    </td>\n";

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
	} elseif ($amari != 0 && $amari != "$b_amari") {
		$max = 3 - $amari;
		for ($ii=1; $ii<=$max; $ii++) {
			$html .= "    <td>&nbsp;</td>\n";
		}
		$html .= "</tr>\n";
	}

	return $html;
}
?>
