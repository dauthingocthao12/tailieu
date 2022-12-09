<?PHP
/*

	楽天＆Yahooディレクトリーリスト作成ファイル
	Amazon追加	2015/07/24

*/

//	楽天プルダウンリスト
function rd_list($set_val) {

	$pl_list = "";

	$selected = "";
	if ($set_val == "") { $selected = "selected"; }
	$pl_list .= "<option value=\"\" ".$selected.">選択してください</option>\n";

	$sql  = "SELECT dir_id, dir_name FROM mool_rd".
			" WHERE flg='1'".
			" ORDER BY dir_name;";
	if ($result = pg_query(conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$dir_id = $list['dir_id'];
			$dir_name = $list['dir_name'];
			$selected = "";
			if ($set_val == $dir_id) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$dir_id."\" ".$selected.">".$dir_name."</option>\n";
		}
	}

	return $pl_list;
}



function rd_val($dir_id) {

	$dir_name = "";

	$sql  = "SELECT dir_name FROM mool_rd".
			" WHERE dir_id='".$dir_id."'".
			" LIMIT 1;";
	if ($result = pg_query(conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$dir_name = $list['dir_name'];
	}

	return $dir_name;
}



//	Yahooプルダウンリスト
function yd_list($set_val) {

	$pl_list = "";

	$selected = "";
	if ($set_val == "") { $selected = "selected"; }
	$pl_list .= "<option value=\"\" ".$selected.">選択してください</option>\n";

	$sql  = "SELECT id, path_name FROM mool_yd".
			" WHERE flg='1'".
			" GROUP BY id, path_name".
			" ORDER BY id;";
	if ($result = pg_query(conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$id = $list['id'];
			$path_name = $list['path_name'];
			$selected = "";
			if ($set_val == $id) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$id."\" ".$selected.">".$path_name."</option>\n";
		}
	}

	return $pl_list;
}



function yd_val($id) {

	$path_name = "";

	$sql  = "SELECT path_name FROM mool_yd".
			" WHERE id='".$id."'".
			" LIMIT 1;";
	if ($result = pg_query(conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$path_name = $list['path_name'];
	}

	return $path_name;
}



//	Amazonプルダウンリスト
function ad_list($set_val, $type = 1) {

	$pl_list = "";

	$selected = "";
	if ($set_val == "") { $selected = "selected"; }
	$msg = "選択してください";		//	add ookawara 2015/09/24
	if ($type != 1) {				//	add ookawara 2015/09/24
		$msg = "----------------";	//	add ookawara 2015/09/24
	}								//	add ookawara 2015/09/24
	$pl_list .= "<option value=\"\" ".$selected.">".$msg."</option>\n";

	//	add ookawara 2015/09/24
	$where = "";
	if ($type == 2) {
		$where = " AND node_type='2'";
	} elseif ($type == 99) {
		//	空の値を出させる為利用しない値99を設定
		$where = " AND node_type='99'";
	}

	$sql  = "SELECT node_id, node_name, node_type FROM mool_ad".
			" WHERE flg='1'".
			$where.					//	add ookawara 2015/09/24
			" GROUP BY node_type, node_id, node_name".
			" ORDER BY node_type, node_id;";
	if ($result = pg_query(conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$node_id = $list['node_id'];
			$node_name = $list['node_name'];
			$node_type = $list['node_type'];							//	add ookawara 2015/09/16
			$type_id = $node_type."_".$node_id;							//	add ookawara 2015/09/16
			$selected = "";
			if ($set_val == $type_id) { $selected = "selected"; }		//	add ookawara 2015/09/16
			//if ($set_val == $node_id) { $selected = "selected"; }		//	del ookawara 2015/09/16
			//$pl_list .= "<option value=\"".$node_id."\" ".$selected.">".$node_name."</option>\n";	//	del ookawara 2015/09/16
			$pl_list .= "<option value=\"".$type_id."\" ".$selected.">".$node_name."</option>\n";	//	add ookawara 2015/09/16
		}
	}

	return $pl_list;
}



function ad_val($node_id) {

	$path_name = "";

	//	add ookawara 2015/09/24
	$node_type = 1;
	if (preg_match("/^2_/", $node_id)) {
		$node_type = 2;
	} elseif (preg_match("/^3_/", $node_id)) {
		$node_type = 3;
	}
	$node_id = preg_replace("/^1_|^2_|^3_/", "", $node_id);

	$sql  = "SELECT node_name FROM mool_ad".
			" WHERE node_id='".$node_id."'".
			" AND node_type='".$node_type."'".	//	add ookawara 2015/09/24
			" LIMIT 1;";
	if ($result = pg_query(conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$node_name = $list['node_name'];
	}

	return $node_name;
}



//	Amazon商品タイプ
function amz_product_subtype($product_subtype, $type=2) {

	$LIST = array(Accessory,Bag,Blazer,Bra,Chanchanko,Dress,Hat,Jinbei,Kimono,Obi,Outerwear,Pants,Shirt,Shoes,Shorts,Skirt,Sleepwear,SocksHosiery,Suit,Sweater,Swimwear,Underwear,Yukata);

	$pl_list = "";

	$selected = "";
	if ($product_subtype == "") { $selected = "selected"; }
	$msg = "選択してください";
	if ($type != 2) {
		$msg = "----------------";
	}
	$pl_list .= "<option value=\"\" ".$selected.">".$msg."</option>\n";

	if ($type == 2) {
		foreach ($LIST AS $val) {
			$selected = "";
			if ($product_subtype == $val) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$val."\" ".$selected.">".$val."</option>\n";
		}
	}

	return $pl_list;
}



//	Amazon 対象年齢・性別
//	add ookawara 2015/09/27
function amz_department_name($department_name, $type = 1) {

	//	服＆シューズ
	$L_AP = array('ガールズ', 'ベビー', 'ボーイズ', 'メンズ', 'レディーズ');

	//	スポーツ
	$L_SP = array('ガールズ', 'ジュニア', 'ベビー', 'ボーイズ', 'メンズ', 'ユニセックス', 'レディーズ');

	$html = "";

	$SET_VAL = $department_name;
	if (!is_array($department_name)) {
		$SET_VAL = explode(",", $department_name);
	}

	$LIST = $L_AP;
	if ($type != 2) {
		//	スポーツの場合（チェックボックス）
		foreach ($L_SP AS $val) {
			$checked = "";
			if (in_array($val, $SET_VAL)) {
				$checked = " checked";
			}
			$html .= "<input type=\"checkbox\" name=\"department_name[]\" value=\"".$val."\"".$checked." />：".$val." ";
		}
		$html .= "<br>\n";
		$html .= "※最大5つまで選択可能です。<br>\n";
	} else {
		//	服＆シューズの場合（セレクトボックス）
		foreach ($L_AP AS $val) {
			$checked = "";
			if (in_array($val, $SET_VAL)) {
				$checked = " checked";
			}
			$html .= "<input type=\"radio\" name=\"department_name[]\" value=\"".$val."\"".$checked." />：".$val." ";
		}
	}



	return $html;
}



//	Amazon モデル年(発売年・発表年)
//	add ookawara 2015/10/01
function amz_model_year_list($model_year, $type = 2) {

	$pl_list = "";

	$selected = "";
	if ($model_year == "") { $selected = "selected"; }
	$msg = "選択してください";
	if ($type != 2) {
		$msg = "----------------";
	}

	if ($type != 2) {
		$pl_list .= "<option value=\"\" ".$selected.">".$msg."</option>\n";
	} else {
		$pl_list .= "<option value=\"\" ".$selected.">".$msg."</option>\n";
		$max_year = date("Y") + 1;
		for ($i=2015; $i<=$max_year; $i++) {
			$selected = "";
			if ($model_year == $i) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$i."\" ".$selected.">".$i."</option>\n";
		}
	}

	return $pl_list;
}



//	Amazon シーズン
//	add ookawara 2015/10/01
function amz_seasons_list($seasons, $ad_id = "") {

	$L_SEASON = array();
	if (preg_match("/^1_/", $ad_id)) {
		//	シューズバッグ
		$L_SEASON = array('定番', '春夏', '夏', '秋冬', '冬');
	} elseif (preg_match("/^3_/", $ad_id)) {
		//	服＆ファッション
		$L_SEASON = array('定番', '春', '春夏', '夏', '秋', '秋冬', '冬');
	}

	$pl_list = "";

	$selected = "";
	if ($seasons == "") { $selected = "selected"; }
	$msg = "選択してください";
	if (!$L_SEASON) {
		$msg = "----------------";
	}
	$pl_list .= "<option value=\"\" ".$selected.">".$msg."</option>\n";

	if ($L_SEASON) {
		foreach ($L_SEASON AS $val) {
			$selected = "";
			if ($seasons == $val) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$val."\" ".$selected.">".$val."</option>\n";
		}
	}

	return $pl_list;
}



//	yd　rd　埋め込み
function yd_rd_set() {

	if (file_exists("./data/ypath.dat")) {
		$Y_LIST = file("./data/ypath.dat");
		foreach ($Y_LIST AS $val) {
			list($y_name_,$ypath_,$del_,$live_num_,$raku_num_,$amazon_num_) = explode("<>",$val);
			if ($del_ == 1) { continue; }
			if ($product_category_list && $live_num_) { $product_category_list .= ","; }
			$product_category_list .= $live_num_;
			$L_YPATH[$y_name_] = $ypath_;
			$L_NEW_YPATH[$y_name_] = $live_num_;
			$L_RAKU[$y_name_] = $raku_num_;
		}
	}

	$sql  = "SELECT g_num, ypath FROM category".
			" GROUP BY g_num, ypath;";
	if ($result = pg_query(conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$g_num = $list['g_num'];
			$ypath = $list['ypath'];

			if ($g_num && $L_NEW_YPATH[$ypath]) {
				$rd_id = $L_RAKU[$ypath];
				$yd_id = $L_NEW_YPATH[$ypath];

				$sql  = "UPDATE goods SET".
						" rd_id='".$rd_id."',".
						" yd_id='".$yd_id."'".
						" WHERE g_num='".$g_num."';";
				pg_exec(conn_id,$sql);
			}
		}
	}

}

//	add ohkawara 2017/02/07
//	Wowmaプルダウンリスト
function wd_list($set_val) {

	$pl_list = "";

	$selected = "";
	if ($set_val == "") { $selected = "selected"; }
	$pl_list .= "<option value=\"\" ".$selected.">選択してください</option>\n";

	$sql  = "SELECT dir_id, dir_name FROM mool_wd".
			" WHERE flg='1'".
			" ORDER BY dir_name;";
	if ($result = pg_query(conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$dir_id = $list['dir_id'];
			$dir_name = $list['dir_name'];
			$selected = "";
			if ($set_val == $dir_id) { $selected = "selected"; }
			$pl_list .= "<option value=\"".$dir_id."\" ".$selected.">".$dir_name."</option>\n";
		}
	}

	return $pl_list;
}



function wd_val($dir_id) {

	$dir_name = "";

	$sql  = "SELECT dir_name FROM mool_wd".
			" WHERE dir_id='".$dir_id."'".
			" LIMIT 1;";
	if ($result = pg_query(conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$dir_name = $list['dir_name'];
	}

	return $dir_name;
}
?>