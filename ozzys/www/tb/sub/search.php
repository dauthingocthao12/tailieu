<?PHP
/*

	ozzys様
	PDA商品管理システム
		商品検索ページ表示

*/

function make_html() {

	set_data();

	if (ACTION == "check") {
		//	選択データーチェック
		check_data(&$ERROR);
	} else {
		unset($_SESSION['CHECK_PLUID']);
		unset($_SESSION['DEL_PLUID']);
	}

	//	検索ページ
	$html = default_html($ERROR);

	return $html;

}


//	データーセット
function set_data() {

	$s_page = 1;
	$s_kyoutu = 1;
	$s_words = "";
	$s_maker = "";
	$s_class = "";
	$s_goods_name = "";
	$s_view_num = 10;
	$s_type = "簡単検索";

	if ($_SESSION['SEARCH']['s_type']) {
		$s_type = $_SESSION['SEARCH']['s_type'];
	}

	if ($_POST['action'] == "" || $_POST['action'] == "クリア") {
		unset($_SESSION['SEARCH']);
	}

	if ($_POST['action'] == "search") {
		if ($_POST['s_page'] != "") { $s_page = $_POST['s_page']; }
		if ($_POST['s_kyoutu'] != 1) { $s_kyoutu = ""; }
		if ($_POST['s_words'] != "") { $s_words = $_POST['s_words']; }
		if ($_POST['s_words_b'] != "") { $s_words_b = $_POST['s_words_b']; }
		if ($_POST['s_maker'] != "") { $s_maker = $_POST['s_maker']; }
		if ($_POST['s_class'] != "") { $s_class = $_POST['s_class']; }
		if ($_POST['s_goods_name'] != "") { $s_goods_name = $_POST['s_goods_name']; }
		if ($_POST['s_view_num'] != "") { $s_view_num = $_POST['s_view_num']; }
		if ($_POST['s_type'] != "") { $s_type = $_POST['s_type']; }

	} elseif (($_POST['action'] == "page" || $_POST['action'] == "check") && $_SESSION['SEARCH']) {
		foreach ($_SESSION['SEARCH'] AS $key => $val) {
			$$key = $val;
		}
		if ($_POST['s_page']) {
			$s_page = $_POST['s_page'];
		}
	}

	## $s_words = mb_convert_kana($s_words,"asKV","EUC-JP");
	$s_words = mb_convert_kana($s_words,"asKV","UTF-8");
	$s_words = trim($s_words);
	$_SESSION['SEARCH']['s_page'] = $s_page;
	$_SESSION['SEARCH']['s_kyoutu'] = $s_kyoutu;
	$_SESSION['SEARCH']['s_words'] = $s_words;
	$_SESSION['SEARCH']['s_words_b'] = $s_words_b;
	$_SESSION['SEARCH']['s_maker'] = $s_maker;
	$_SESSION['SEARCH']['s_class'] = $s_class;
	$_SESSION['SEARCH']['s_goods_name'] = $s_goods_name;
	$_SESSION['SEARCH']['s_view_num'] = $s_view_num;
	$_SESSION['SEARCH']['s_type'] = $s_type;

}

//	検索ページ
function default_html($ERROR) {
	global $L_LIST_VIEW_NUM,$L_CLASS;

	$html = "";
	$INPUTS = array();
	$DEL_INPUTS = array();
	$L_CLASS_M = array();


	//	検索情報表示データーを変数に入れる	
	if ($_SESSION['SEARCH']) {
		$s_type = $_SESSION['SEARCH']['s_type'];
		$s_kyoutu = $_SESSION['SEARCH']['s_kyoutu'];
		$s_words = $_SESSION['SEARCH']['s_words'];
		$s_words_b = $_SESSION['SEARCH']['s_words_b'];
		$s_maker = $_SESSION['SEARCH']['s_maker'];
		$s_class = $_SESSION['SEARCH']['s_class'];
		$s_goods_name = $_SESSION['SEARCH']['s_goods_name'];
		$s_view_num = $_SESSION['SEARCH']['s_view_num'];
	}

	//	アクションセット
	if (!defined("ACTION")) { define("ACTION","search"); }

	//	検索タイプ

	$INPUTS['SEARCHTYPE'] = $s_type;	

	//	共通商品表示

	if ($s_kyoutu == 1) {	
		$INPUTS['LISTKYOUTU'] = " checked";

		//	検索情報削除	
	}
	unset($s_words);
	//	キーワード

	$INPUTS['WORDS'] = $s_words;
	if ($s_kyoutu != 1) {
		$INPUTS['WORDS2'] = $_SESSION['SEARCH']['s_words'];	
	}

	//検索候補出力
	$goods_name_candidacy =array();
	if ( $_SESSION['SEARCH']['s_words']){
		$words_parts = $_SESSION['SEARCH']['s_words'];
//echo "$words_parts<br><br>\n";

		$sql = "SELECT DISTINCT ps.goods_name FROM ".TB_PDA_SEARCH." ps".
				" WHERE ps.goods_name LIKE '%".$words_parts ."%'";
//echo $sql."<br><br>\n";

		if ($result = pg_query(DB,$sql)) {
			$i = 0;
			WHILE ($list = pg_fetch_array($result)) {
				$goods_name_candidacy[$i] = $list['goods_name'];
				$i++;
			}
		}
//print_r( $goods_name_candidacy);
		if(count($goods_name_candidacy) > 1 ){
			if( count($goods_name_candidacy) >= CANDIDACY_MAX){
			/* && mb_strlen ($words_parts)<= CANDIDACY_CHARACTER_MIN){*/
				$INPUTS['CANDIDACY'] = "候補が多すぎます。検索条件を増やしてください。";
			}else{
				$make_goods_name_btn ="";
				$candidacy_count=1;
				foreach($goods_name_candidacy as $candidacy){
					$make_goods_name_btn .="<button type='submit' value='{$candidacy}' class='candidacy-btn' name='s_words_b'><img src='image/search_ico.png'>{$candidacy}</button>";
						/*if($candidacy_count %CANDIDACY_ROW_MAX == 0){
							$make_goods_name_btn .="<br>";
						}*/
					$candidacy_count++;
				}
				$INPUTS['CANDIDACY'] = $make_goods_name_btn;
			}
		}
	}

	//	メーカー
	$list_maker = "";
	$selected = "";

	if ($s_maker == "") { $selected = "selected"; }	
	$list_maker .= "<option value=\"\">-------------</option>\n";
	$sql  = "SELECT DISTINCT maker_num, maker_name FROM ".TB_PDA_SEARCH.
			" ORDER BY maker_name;";
//echo $sql."<br><br>\n";

	if ($result = pg_query(DB,$sql)) {
		WHILE ($list = pg_fetch_array($result)) {
			$maker_num = $list['maker_num'];
			$maker_name = $list['maker_name'];
			$selected = "";

			if ($maker_num == $s_maker) { $selected = "selected"; }	
			$list_maker .= "<option value=\"".$maker_num."\" ".$selected.">".$maker_name."</option>\n";
		}
	}
	$INPUTS['LISTMAKER'] = $list_maker;

	//	分類
	$list_class = "";
	$selected = "";

	if ($s_class == "") { $selected = "selected"; }	
	$list_class .= "<option value=\"\">-------------</option>\n";
	$sql  = "SELECT class_m, class_m_n FROM ".TB_CLASS.
			" ORDER BY class_m;";
//echo $sql."<br><br>\n";

	if ($result = pg_query(DB,$sql)) {
		WHILE ($list = pg_fetch_array($result)) {
			$class_m = $list['class_m'];
			$class_m_n = $list['class_m_n'];
			$L_CLASS_M[$class_m] = $class_m_n;

			$class = "";
			if ($class_m) {
				$class = floor($class_m / 100);
			}

			$selected = "";

			if ($class_m == $s_class) { $selected = "selected"; }	
			$list_class .= "<option value=\"".$class_m."\" ".$selected.">".$L_CLASS[$class]." ".$class_m_n."</option>\n";
		}
	}
	$INPUTS['LISTCLASS'] = $list_class;

	//	商品名
	$list_goods_name = "";
	if ($_SESSION['SEARCH']['s_maker']) {
		$CHECK_GOODS_NAME = array();
		$selected = "";

		if ($s_goods_name == "") { $selected = "selected"; }	
		$list_goods_name .= "<option value=\"\">-------------</option>\n";
		$where = "";
		if ($_SESSION['SEARCH']['s_class']) {
			$where = " AND class_m='".$_SESSION['SEARCH']['s_class']."'";
		}
		$sql  = "SELECT pluid, goods_name, l_goods_name FROM ".TB_PDA_SEARCH.
				" WHERE maker_num='".$_SESSION['SEARCH']['s_maker']."'".
				$where.
				" ORDER BY l_goods_name, goods_name;";
//echo "<00>".$sql."<br><br>\n";

		if ($result = pg_query(DB,$sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				$pluid = $list['pluid'];
				$goods_name = $list['goods_name'];
				$l_goods_name = $list['l_goods_name'];
				if ($CHECK_GOODS_NAME[$goods_name]) { continue; }
				$name = $goods_name;
				if ($l_goods_name) { $name = $l_goods_name; }
				$selected = "";
				if ($goods_name == $_SESSION['SEARCH']['s_goods_name']) { $selected = "selected"; }
				$list_goods_name .= "<option value=\"".$goods_name."\" ".$selected.">".$name."</option>\n";
				$CHECK_GOODS_NAME[$goods_name] = 1;
			}
		}
	} else {
		$DEL_INPUTS['SEARCHGOODSNAME'] = 1;
	}
	$INPUTS['LISTGOODSNAME'] = $list_goods_name;

	//	検索表示数
	$list_view_num = "";
	foreach ($L_LIST_VIEW_NUM AS $key => $val) {
		$selected = "";

		if ($key == $s_view_num) { $selected = "selected"; }
		$list_view_num .= "<option value=\"".$key."\" ".$selected.">".$val."</option>\n";
	}
	$INPUTS['LISTVIEWNUM'] = $list_view_num;


	if ($s_type == "簡単検索") {
		$DEL_INPUTS['SEARCHMAKER'] = 1;
		$DEL_INPUTS['SEARCHCLASS'] = 1;
		$DEL_INPUTS['SEARCHGOODSNAME'] = 1;
	}


	if ($s_type == "簡単検索") {
		$DEL_INPUTS['SEARCHTYPEE'] = 1;
	} else {
		$DEL_INPUTS['SEARCHTYPED'] = 1;
	}


	//	絞り込み
	$where = "";
	//	キーワード 共通商品
	$word_flg = 0;
	if ($_SESSION['SEARCH']['s_words']) {
		$WORDS = explode(" ",$_SESSION['SEARCH']['s_words']);
		$WORD = array();
		if ($WORDS) {
			foreach ($WORDS AS $val) {
				##	$val = mb_convert_kana($val,"asKV","EUC-JP");
				$val = mb_convert_kana($val,"asKV","UTF-8");
				$val = trim($val);
				if (!$val) { continue; }
				$WORD[$val] = $val;
			}
		}

		if ($WORD) {
			//	共通商品名取得
			if ($_SESSION['SEARCH']['s_kyoutu'] == 1) {

				$SET_WORDS = array();
				foreach ($WORD AS $val) {
					if (preg_match("/[0-9]{8}/",$val)) {
						unset($WORD[$val]);
						$SET_WORDS[] = $val;
					}
				}
				if ($SET_WORDS) {
					$sql  = "SELECT goods_name, l_goods_name FROM ".TB_PDA_SEARCH;
					for ($i=0; $i<count($SET_WORDS); $i++) {
						if ($i == 0) {
							$sql .= " WHERE";
						} else {
							$sql .= " OR";
						}
						$sql .= " pluid like '".$SET_WORDS[$i]."%'";
					}
					$sql .= ";";
//echo "<0>".$sql."<br><br>\n";

					if ($result = pg_query(DB,$sql)) {
						WHILE ($list = pg_fetch_array($result)) {
							$goods_name = $list['goods_name'];
							$l_goods_name = $list['l_goods_name'];
							$WORD[$goods_name] = $goods_name;
							$WORD[$l_goods_name] = $l_goods_name;
						}
					}
				}
			}

			if ($WORD) {
				//	キーワード設置
				foreach ($WORD AS $val) {
					$where .= " AND ps.search_word like '%".$val."%'";
				}
				$word_flg = 1;
			}
		}
	}

	//候補ボタン
	if ( $_SESSION['SEARCH']['s_words_b']){
		if ($s_kyoutu != 1) {
			$INPUTS['WORDS2'] = $_SESSION['SEARCH']['s_words_b'];	
		}
		$where .= " AND ps.goods_name ='".$_SESSION['SEARCH']['s_words_b']."'";

	}

	//	メーカー
	if ($_SESSION['SEARCH']['s_maker']) {
		$where .= " AND ps.maker_num='".$_SESSION['SEARCH']['s_maker']."'";
	}

	//	分類
	if ($_SESSION['SEARCH']['s_class']) {
		$where .= " AND ps.class_m='".$_SESSION['SEARCH']['s_class']."'";
	}

	//	商品名
	if ($_SESSION['SEARCH']['s_goods_name']) {
		$where .= " AND ps.goods_name='".$_SESSION['SEARCH']['s_goods_name']."'";
	}

	if ($where) {
		$where = preg_replace("/^ AND/"," WHERE",$where);
	}


	//	キーワード 共通商品で、該当商品が無かった場合
	if ($_SESSION['SEARCH']['s_words'] && $_SESSION['SEARCH']['s_kyoutu'] == 1 && $word_flg == 0) {
		$count = 0;
	} else {
		//	検索
		$sql  = "SELECT count(*) AS count FROM ".TB_PDA_SEARCH." ps".
				$where.";";
//echo "<1>".$sql."<br><br>\n";

		if ($result = pg_query(DB,$sql)) {
			$list = pg_fetch_array($result);
			$count = $list['count'];
		}
	}

	$goods_list = "";
	$page_all = 0;
	if($count > GOODSLIST_MAX ){
		$DEL_INPUTS['GOODSD'] = 1;
		$DEL_INPUTS['GOODSN'] = 1;
	}elseif ($count > 0) {

		$make_list = "";
		$sql  = "SELECT ps.pluid, ps.stock, ps.price, ps.class_m, ps.maker, ps.goods_name, ps.color, ps.size,".
				" ps.list_num, ps.l_goods_name, ps.l_size, ps.l_color,".
				" ps.maker_name,".
				" pc.pluid as check".
				", ps.set_flag".
				", ps.ru_touki, ps.ru_zenki".
				" FROM ".TB_PDA_SEARCH." ps".
				" LEFT JOIN ".TB_PDA_CHECK." pc ON pc.pluid=ps.pluid AND pc.state='1'".
				$where.
				" ORDER BY ps.l_goods_name Limit ".GOODSLIST_MAX.";";
//echo "<2>".$sql."<br><br>\n";
		if ($result = pg_query(DB,$sql)) {
			$i = 1;
			WHILE ($list = pg_fetch_array($result)) {
				$line_color = $i % 2;
				$goods_list .= goods_list($list,$line_color,$L_CLASS_M,$i,$_SESSION['SEARCH']['s_kyoutu']);
				$i++;
			}
		}
		$INPUTS['GOODSLIST'] = $goods_list;
		$DEL_INPUTS['GOODSL'] = 1;
		$DEL_INPUTS['GOODSN'] = 1;
	} else {
		$DEL_INPUTS['GOODSL'] = 1;
		$DEL_INPUTS['GOODSD'] = 1;
	}

	//	ページ処理
	if ($_SESSION['SEARCH']['s_kyoutu'] == 1 && $_SESSION['SEARCH']['s_words']) {
		$INPUTS['NEXTVIEW1'] = "該当数：".$count."件";
	} else {
		$next_view = page_view($count);
		$INPUTS['NEXTVIEW1'] = $next_view;
	}

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);
	$make_html->set_file(TEMP_SEARCH);
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}


//	選択データーセット
function check_data(&$ERROR) {

	if (!$_POST['pluid'] && !$_POST['del_pluid']) {
		$ERROR[] = "保存するデーターが選択されておりません。";
		return;
	}

	//	新規登録
	$insert_sql = "";
	if ($_POST['pluid']) {
		foreach ($_POST['pluid'] AS $pluid => $val) {
			if (!$_SESSION['CHECK_PLUID'][$pluid]) {
				$insert_sql  .= "INSERT INTO ".TB_PDA_CHECK.
								" VALUES('".$pluid."',now(),'1');";
			}
		}
	}
	if ($insert_sql) {
		if (!pg_exec(DB,$insert_sql)) {
			$ERROR[] = "情報を登録できませんでした。";
		}
	}


	//	データー削除
	$delete_sql = "";
	if ($_POST['del_pluid']) {
		foreach ($_POST['del_pluid'] AS $pluid => $val) {
			if (!$_SESSION['DEL_PLUID'][$pluid]) {
				$delete_sql  .= "UPDATE ".TB_PDA_CHECK." SET".
								" state='0'".
								" WHERE pluid='".$pluid."';";
			}
		}
	}
	if ($delete_sql) {
		if (!pg_exec(DB,$delete_sql)) {
			$ERROR[] = "情報を削除できませんでした。";
		}
	}

	//	2重登録防止セッション埋め込み
	$_SESSION['CHECK_PLUID'] = array();
	$_SESSION['DEL_PLUID'] = array();
	if ($_POST['pluid']) {
		$_SESSION['CHECK_PLUID'] = $_POST['pluid'];
	}
	if ($_POST['del_pluid']) {
		$_SESSION['DEL_PLUID'] = $_POST['del_pluid'];
	}

}


//	商品リスト
function goods_list($list,$line_color,$L_CLASS_M,$i,$s_kyoutu) {	// change ookawara 2009/09/07 $i追加
	global $L_CLASS;

	$INPUTS = array();
	$del_pluid = "";
	$del_msg = "";

	$pluid = $list['pluid'];
	$stock = $list['stock'];
	$price = $list['price'];
	$class_m = $list['class_m'];
	$maker = $list['maker'];
	$goods_name = $list['goods_name'];
	$color = $list['color'];
	$size = $list['size'];
	$list_num = $list['list_num'];
	$l_goods_name = $list['l_goods_name'];
	$l_size = $list['l_size'];
	$l_color = $list['l_color'];
	$maker_name = $list['maker_name'];
	$check = $list['check'];
	$set_flag = $list['set_flag'];
	$ru_touki = $list['ru_touki'];
	$ru_zenki = $list['ru_zenki'];

	if ($maker_name) { $maker = $maker_name; }
	if ($class_m) {
		$class = floor($class_m / 100);
		$category = $L_CLASS[$class]." ".$L_CLASS_M[$class_m];
	} else {
		$category = "-----";
	}
	if ($l_goods_name) { $goods_name = $l_goods_name; }
	if ($l_color) { $color = $l_color; }
	if (!$color) { $color = "----"; }
	if ($l_size) { $size = $l_size; }
	if (!$size) { $size = "----"; }
	if (!$list_num) {
		$list_num = "----";
	} else {
		$list_num = "g".$list_num;
	}
	$price = number_format($price);

	if ($set_flag == "H" || $set_flag == "K") {
		$goods_line = "goods_line3";
	} elseif ($set_flag == "A") {
		$goods_line = "goods_line4";
	} elseif ($line_color == 1) {
		$goods_line = "goods_line1";
	} else {
		$goods_line = "goods_line2";
	}

	if ($check) {
		$del_pluid = "del_";
		$del_msg = "登録済み<br />削除：";
	}

	//	商品名
	$INPUTS['GOODSNAME'] = $goods_name;
	$INPUTS['MAKER'] = $maker;
	$INPUTS['CATEGORY'] = $category;
	$INPUTS['PLUID'] = $pluid;
	$INPUTS['LISTNUM'] = $list_num;
	$INPUTS['COLOR'] = $color;
	$INPUTS['SIZE'] = $size;
	$INPUTS['STOCK'] = $stock;
	$INPUTS['PRICE'] = $price;
	$INPUTS['GOODSLINE'] = $goods_line;
	$INPUTS['DELPLUID'] = $del_pluid;
	$INPUTS['DELMSG'] = $del_msg;
	$INPUTS['RUTOUKI'] = $ru_touki;
	$INPUTS['RUZENKI'] = $ru_zenki;

	//	html作成・置換

	$html = "";
	if ($s_kyoutu == 1) {
		$template = TEMP_SEARCH_LIST2;

		if ($i == 1) {
			$make_html = new read_html();
			$make_html->set_dir(INCLUDE_DIR);
			$make_html->set_file(TEMP_SEARCH_LIST2_HEAD);
			$make_html->set_rep_cmd($INPUTS);
			$html = $make_html->replace();
		}
	} else {
		$template = TEMP_SEARCH_LIST;
	}

	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);

	$make_html->set_file($template);
	$make_html->set_rep_cmd($INPUTS);

	$html .= $make_html->replace();

	return $html;
}
?>