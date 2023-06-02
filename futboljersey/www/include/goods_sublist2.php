<?PHP
/*

	ネイバーズスポーツ　サブ2カテゴリー一覧

*/
function goods_sublist2($VALUE, $CHECK) {
	global $conn_id; // mysql DB

	$cate1 = (int)preg_replace("/[^0-9]/", "", $CHECK['main']);
	$cate2 = (int)preg_replace("/[^0-9]/", "", $CHECK['s']);
	//	表示可能カテゴリー読み込み
	//	del ookawara 2016/02/04
	//$sql  = "SELECT cate1, cate2, cate3 FROM ".T_CATE.
	//		" WHERE cate1='".$cate1."'".
	//		" AND cate2='".$cate2."'".
	//		" AND display='1'".
	//		" AND state!='3'".
	//		" AND cate1 NOT IN ('99')".	//	add ookawara 2015/01/29
	//		" GROUP BY cate1, cate2, cate3;";

	//	add ookawara 2016/02/04
	$sql  = "SELECT category.cate1, category.cate2, category.cate3 FROM ".T_CATE." category".
			" LEFT JOIN goods goods ON  category.g_num=goods.g_num".
			" WHERE category.cate1='".$cate1."'".
			" AND category.cate2='".$cate2."'".
			" AND category.display='1'".
			" AND category.state!='3'".
			" AND category.cate1 NOT IN ('99')".
			" AND goods.g_num>0".
			" GROUP BY category.cate1, category.cate2, category.cate3;";

	if ($result = mysqli_query($conn_id, $sql)) {
		while ($list = mysqli_fetch_array($result)) {
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
		if (!$VAL) { continue; }
		list($h_num_, $mc_num_, $cate_name_, $cate_title_) = explode("<>", $VAL);
		if ($mc_num_ == 99) { continue; }	//	add ookawara 2015/01/29
		if ($cate1 && $cate1 != $mc_num_) { continue; }
		else {
			$mc_num = $mc_num_;
			$cate_name = $cate_name_;
			break;
		}
	}

	//	カテゴリー表示
	if ($mc_num) {
		if ($CATE[$mc_num]) {
			//	サブカテゴリー表示
			unset($LIST2);
			unset($sc_html);
			$file = "./".DIR_CATE."/".$mc_num.".dat";
			if (file_exists($file)) {
				$LIST2 = file($file);
			}
			if ($LIST2) { list($sc_html, $sc_name) = subcategory($CATE, $mc_num, $LIST2, $CHECK); }
		}
		if ($sc_html) {
			$html  = "<h2 class=\"title-nbs\">".$cate_name."　".$sc_name." カテゴリー 一覧</h2>\n";
			$html .= "<div class='categories'>\n";
			$html .= "	<ul>\n";
			$html .= $sc_html;
			$html .= "	</ul>\n";
			$html .= "</div>\n";
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
function subcategory($CATE, $mc_num, $LIST2, $CHECK) {

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	unset($check);
	if ($CHECK['s']) {
		$check = (int)preg_replace("/[^0-9]/", "", $CHECK['s']);
	}

	foreach ($LIST2 AS $VAL2) {
		$VAL2 = trim($VAL2);
		if (!$VAL2) { continue; }
		list($h_num2_, $sc_num_, $sc_name_, $cate_title2_) = explode("<>", $VAL2);
		if ($check && $check != $sc_num_) { continue; }
		else {
			$h_num2 = $h_num2_;
			$sc_num = $sc_num_;
			$sc_name = $sc_name_;
			$cate_title2 = $cate_title2_;
			break;
		}
	}

	if ($CATE[$mc_num][$sc_num]) {
		unset($LIST3);
		unset($ssc_html);
		$file = "./".DIR_CATE."/".$mc_num."_".$sc_num.".dat";
		if (file_exists($file)) {
			$LIST3 = file($file);
		}
		if ($LIST3) { $ssc_html = sub2category($CATE, $mc_num, $sc_num, $LIST3); }
	}

	$html .= $ssc_html;

	if (!$ssc_html) { unset($html); }

	return array($html, $sc_name);

}



//	サブカテゴリー2表示
function sub2category($CATE, $mc_num, $sc_num, $LIST3) {
	global $index;
	$html = ""; // return

	//	add ookawara 2015/01/29
	if ($mc_num == 99) {
		return;
	}

	if (!$index) { unset($index); }

	foreach ($LIST3 AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		list($h_num, $s2c_num, $s2c_name, $s2c_file) = explode("<>", $VAL);
		if ($CATE[$mc_num][$sc_num][$s2c_num]) {
			$link = (int)sprintf("%02d",$sc_num);
			$link2 = (int)sprintf("%02d",$s2c_num);
			$html .= "		<li><a href=\""."/".GOODS_SCRIPT."/".$mc_num."/".$link."/".$link2."/".$index."\" title=\"".$s2c_name."\">".$s2c_name."</a></li>\n";
		}
	}

	return $html;

}
