<?PHP
/*

	ネイバーズスポーツ　パンくずリスト

*/

function read_pankuzu_list($CHECK) {
	global $conn_id, $db, $CLASS_L, $index, $cate_table, $NEWCATE;

	if (!$index) { unset($index); }

	$max = count($CHECK);
	$i = 1;
	$title = "";
	$url = "/goods/";
	$file_name = "";
	$check = 0;

	foreach ($CHECK AS $KEY => $VAL) {
		if ($KEY == "g") {
			$check = 1;
			$g_num = $VAL;
		}
	}

	if ($check == 1) {
		$g_num = preg_replace("/[^0-9]/", "", $g_num);
		$sql  = "SELECT cate1, cate2, cate3 FROM $cate_table".
				" WHERE g_num='".$g_num."'".
				" AND display='1'".
				" AND state!='3'".
				" AND cate1 NOT IN ('99')".			//	add ookawara 2013/08/23
				" ORDER BY cate1, cate2, cate3;";	//	add ookawara 2016/02/15
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$cate1 = $list['cate1'];
				$cate2 = $list['cate2'];
				$cate3 = $list['cate3'];

				$name = "";
				if ($NEWCATE[$cate1]['name']) {
					$name0 = $NEWCATE[$cate1]['name'];
					$url0 = $NEWCATE[$cate1]['url'];
					$name .= "<li><a href=\"".$url0."\" title=\"".$name0."\">".$name0."</a></li>\n";
					$name .= " &gt;";
				}

				$url_ = $url.$cate1."/";
				$name1 = m_name($cate1);
				if ($name0 == $name1) { $name = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name1."\">".$name1."</a></li>\n";
				$name .= " &gt;";

				$url_ .= "$cate2/";
				$file_name = $cate1;
				$name2 = s_name($cate2, $file_name);
				if ($name1 == $name2) { $name = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name2."\">".$name2."</a></li>\n";
				$name .= " &gt;";

				$url_ .= $cate3."/";
				$file_name .= "_".$cate2;
				$name3 = l_name($cate3, $file_name);
				if ($name2 == $name3) { $name = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name3."\">".$name3."</a></li>\n";

				if ($g_num) {
					$title = g_name($g_num);
				}

				if ($link) { $link .= "\n"; }

				$link .= "<ul class=\"pankuzu\">\n";
				$link .= $name;
				$link .= "</ul>\n";

			}
		}
	} else {
		unset($LIST);
		foreach ($CHECK AS $KEY => $VAL) {
			$VAL = (int)preg_replace("/[^0-9]/","",$VAL);
			if (!$VAL) { continue; }
			$name = "";
			if ($KEY == "main") {
				$LIST['cate1'] = $VAL;
			}
			elseif ($KEY == "s") {
				$LIST['cate2'] = $VAL;
			}
			elseif ($KEY == "l") {
				$LIST['cate3'] = $VAL;
			}
			elseif ($KEY == "g") {
				$LIST['g_num'] = $VAL;
			}
		}

		if ($LIST) {
			$cate1 = $LIST['cate1'];
			$cate2 = $LIST['cate2'];
			$cate3 = $LIST['cate3'];
			$g_num = $LIST['g_num'];

			$name = "";
			$title = "";
			if ($NEWCATE[$cate1]['name']) {
				$name0 = $NEWCATE[$cate1]['name'];
				$url0 = $NEWCATE[$cate1]['url'];
				$name .= "<li><a href=\"".$url0."\" title=\"".$name0."\">".$name0."</a></li>\n";
				$name .= " &gt;";
				$title = $name0;
				$title .= " &gt; ";
			}

			if ($cate1) {
				$url_ = $url.$cate1."/";
				$name1 = m_name($cate1);
				if ($name0 == $name1) { $name = $title = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name1."\">".$name1."</a></li>\n";
				$title .= $name1;
			}

			if ($cate2) {
				$name .= " &gt;";
				$title .= " &gt; ";

				$url_ .= $cate2."/";
				$file_name = $cate1;
				$name2 = s_name($cate2, $file_name);
				if ($name1 == $name2) { $name = $title = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name2."\">".$name2."</a></li>\n";
				$title .= $name2;
			}

			if ($cate3) {
				$name .= " &gt;";
				$title .= " &gt; ";

				$url_ .= $cate3."/";
				$file_name .= "_".$cate2;
				$name3 = l_name($cate3, $file_name);
				if ($name2 == $name3) { $name = $title = ""; }
				$name .= "<li><a href=\"".$url_.$index."\" title=\"".$name3."\">".$name3."</a></li>\n";
				$title .= $name3;
			}

			if ($g_num) {
				$title = g_name($g_num);
			}

			$link .= "<ul class=\"pankuzu\">\n";
			$link .= $name;
			$link .= "</ul>\n";
		}
	}

	$link = $link."\n";

	return array($title, $link);
}



//	メインカテゴリー読み込み
function m_name($mc_num) {
	global $DIR_CATE;

	$file = "./$DIR_CATE/category.inc";
	if (file_exists($file)) {
		$LIST = file($file);
		foreach ($LIST AS $VAL) {
			list($h_num_, $mc_num_, $cate_name_, $cate_title_) = explode("<>", $VAL);
			if ($mc_num == $mc_num_) {
				$cate_name = $cate_name_;
				break;
			}
		}
	}

	return $cate_name;
}



//	サブカテゴリー呼び込み
function s_name($sc_num, $file_name) {
	global $DIR_CATE;

	$file = "./$DIR_CATE/$file_name.dat";
	if (file_exists($file)) {
		$LIST = file($file);
		foreach ($LIST AS $VAL) {
			list($h_num2_, $sc_num_, $sc_name_, $cate_title2_) = explode("<>", $VAL);
			if ($sc_num == $sc_num_) {
				$sc_name = $sc_name_;
				break;
			}
		}
	}

	return $sc_name;
}



//	商品一覧呼び込み
function l_name($s2c_num,$file_name) {
	global $DIR_CATE;

	$file = "./$DIR_CATE/$file_name.dat";
	if (file_exists($file)) {
		$LIST = file($file);
		foreach ($LIST AS $VAL) {
			list($h_num_, $s2c_num_, $s2c_name_) = explode("<>", $VAL);
			if ($s2c_num == $s2c_num_) {
				$s2c_name = $s2c_name_;
				break;
			}
		}
	}

	return $s2c_name;
}



//	詳細商品名呼び込み
function g_name($num) {
	global $conn_id, $cate_table, $goods_table;

	//$sql =	"SELECT g_name FROM $goods_table".										//	del ookawara 2015/09/24
	$sql =	"SELECT coalesce(name_head, '') || coalesce(g_name, '') || coalesce(name_foot, '') AS g_name FROM $goods_table".	//	add ookawara 2015/09/24
			" WHERE g_num='$num'".
			" LIMIT 1;";
	if ($result = mysqli_query($conn_id, $sql)) {
		$list = mysqli_fetch_array($result);
		$g_name = $list['g_name'];
	}

	return $g_name;
}
?>
