<?PHP
/*

	ozzys様
	PDA商品管理システム
		確認商品ページ表示

*/

function make_html() {

	set_data();

	if (ACTION == "削除") {
		//	選択データー削除
		del_data(&$ERROR);
	} elseif (ACTION == "全削除") {
		//	選択データー全削除
		del_all_data(&$ERROR);
	}

	//	検索ページ
	$html = default_html($ERROR);

	return $html;

}


//	データーセット
function set_data() {

	$s_page = 1;
	$s_words = "";
	$s_maker = "";
	$s_class = "";
	$s_goods_name = "";
	$s_view_num = 10;
	$s_type = "簡単検索";

	if ($_SESSION['CHECK']['s_type']) {
		$s_type = $_SESSION['CHECK']['s_type'];
	}

	if ($_POST['action'] == "" || $_POST['action'] == "クリア") {
		unset($_SESSION['CHECK']);
	}

	if ($_POST['action'] == "search") {
		if ($_POST['s_page'] != "") { $s_page = $_POST['s_page']; }
		if ($_POST['s_words'] != "") { $s_words = $_POST['s_words']; }
		if ($_POST['s_maker'] != "") { $s_maker = $_POST['s_maker']; }
		if ($_POST['s_class'] != "") { $s_class = $_POST['s_class']; }
		if ($_POST['s_goods_name'] != "") { $s_goods_name = $_POST['s_goods_name']; }
		if ($_POST['s_view_num'] != "") { $s_view_num = $_POST['s_view_num']; }
		if ($_POST['s_type'] != "") { $s_type = $_POST['s_type']; }

	} elseif (($_POST['action'] == "page" || $_POST['action'] == "check") && $_SESSION['CHECK']) {
		foreach ($_SESSION['CHECK'] AS $key => $val) {
			$$key = $val;
		}
		if ($_POST['s_page']) {
			$s_page = $_POST['s_page'];
		}
	}

	## $s_words = mb_convert_kana($s_words,"asKV","EUC-JP");
	$s_words = mb_convert_kana($s_words,"asKV","UTF-8");
	$s_words = trim($s_words);
	$_SESSION['CHECK']['s_page'] = $s_page;
	$_SESSION['CHECK']['s_words'] = $s_words;
	$_SESSION['CHECK']['s_maker'] = $s_maker;
	$_SESSION['CHECK']['s_class'] = $s_class;
	$_SESSION['CHECK']['s_goods_name'] = $s_goods_name;
	$_SESSION['CHECK']['s_view_num'] = $s_view_num;
	$_SESSION['CHECK']['s_type'] = $s_type;

}

//	検索ページ
function default_html($ERROR) {
	global $L_LIST_VIEW_NUM,$L_CLASS;

	$html = "";
	$INPUTS = array();
	$DEL_INPUTS = array();
	$L_CLASS_M = array();

	//	アクションセット
	if (!defined("ACTION")) { define("ACTION","search"); }

	//	検索タイプ
	$INPUTS['SEARCHTYPE'] = $_SESSION['CHECK']['s_type'];

	//	キーワード
	$INPUTS['WORDS'] = $_SESSION['CHECK']['s_words'];


	//	メーカー
	$list_maker = "";
	$selected = "";
	if ($_SESSION['CHECK']['s_maker'] == "") { $selected = "selected"; }
	$list_maker .= "<option value=\"\">-------------</option>\n";
	$sql  = "SELECT DISTINCT ps.maker_num, ps.maker_name FROM ".TB_PDA_SEARCH." ps".
			" LEFT JOIN ".TB_PDA_CHECK." pc ON pc.pluid=ps.pluid".
			" WHERE pc.state='1'".
			" AND pc.pluid>'0'".
			" ORDER BY ps.maker_name;";
	if ($result = pg_query(DB,$sql)) {
		WHILE ($list = pg_fetch_array($result)) {
			$maker_num = $list['maker_num'];
			$maker_name = $list['maker_name'];
			$selected = "";
			if ($maker_num == $_SESSION['CHECK']['s_maker']) { $selected = "selected"; }
			$list_maker .= "<option value=\"".$maker_num."\" ".$selected.">".$maker_name."</option>\n";
		}
	}
	$INPUTS['LISTMAKER'] = $list_maker;

	//	分類
	$list_class = "";
	$selected = "";
	if ($_SESSION['CHECK']['s_class'] == "") { $selected = "selected"; }
	$list_class .= "<option value=\"\">-------------</option>\n";
	$sql  = "SELECT class_m, class_m_n FROM ".TB_CLASS.
			" ORDER BY class_m;";
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
			if ($class_m == $_SESSION['CHECK']['s_class']) { $selected = "selected"; }
			$list_class .= "<option value=\"".$class_m."\" ".$selected.">".$L_CLASS[$class]." ".$class_m_n."</option>\n";
		}
	}
	$INPUTS['LISTCLASS'] = $list_class;

	//	商品名
	$list_goods_name = "";
	if ($_SESSION['CHECK']['s_maker']) {
		$CHECK_GOODS_NAME = array();
		$selected = "";
		if ($_SESSION['CHECK']['s_goods_name'] == "") { $selected = "selected"; }
		$list_goods_name .= "<option value=\"\">-------------</option>\n";
		$where = "";
		if ($_SESSION['CHECK']['s_class']) {
			$where = " AND ps.class_m='".$_SESSION['CHECK']['s_class']."'";
		}
		$sql  = "SELECT ps.pluid, ps.goods_name, ps.l_goods_name FROM ".TB_PDA_SEARCH." ps".
				" LEFT JOIN ".TB_PDA_CHECK." pc ON pc.pluid=ps.pluid".
				" WHERE ps.maker_num='".$_SESSION['CHECK']['s_maker']."'".
				$where.
				" AND pc.state='1'".
				" AND pc.pluid>'0'".
				" ORDER BY ps.goods_name, ps.goods_name;";
		if ($result = pg_query(DB,$sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				$pluid = $list['pluid'];
				$goods_name = $list['goods_name'];
				$l_goods_name = $list['l_goods_name'];
				if ($CHECK_GOODS_NAME[$goods_name]) { continue; }
				$name = $goods_name;
				if ($l_goods_name) { $name = $l_goods_name; }
				$selected = "";
				if ($goods_name == $_SESSION['CHECK']['s_goods_name']) { $selected = "selected"; }
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
		if ($key == $_SESSION['CHECK']['s_view_num']) { $selected = "selected"; }
		$list_view_num .= "<option value=\"".$key."\" ".$selected.">".$val."</option>\n";
	}
	$INPUTS['LISTVIEWNUM'] = $list_view_num;

	if ($_SESSION['CHECK']['s_type'] == "簡単検索") {
		$DEL_INPUTS['SEARCHMAKER'] = 1;
		$DEL_INPUTS['SEARCHCLASS'] = 1;
		$DEL_INPUTS['SEARCHGOODSNAME'] = 1;
	}

	if ($_SESSION['CHECK']['s_type'] == "簡単検索") {
		$DEL_INPUTS['SEARCHTYPEE'] = 1;
	} else {
		$DEL_INPUTS['SEARCHTYPED'] = 1;
	}


	//	絞り込み
	$where = "";
	//	キーワード
	if ($_SESSION['CHECK']['s_words']) {
		$WORDS = explode(" ",$_SESSION['CHECK']['s_words']);
		if ($WORDS) {
			foreach ($WORDS AS $val) {
				##	$val = mb_convert_kana($val,"asKV","EUC-JP");		//	add ookawara 2015/06/04
				$val = mb_convert_kana($val,"asKV","UTF-8");	
				$val = trim($val);									//	add ookawara 2015/06/04
				if (!$val) { continue; }
				$where .= " AND ps.search_word like '%".$val."%'";
			}
		}
	}

	//	メーカー
	if ($_SESSION['CHECK']['s_maker']) {
		$where .= " AND ps.maker_num='".$_SESSION['CHECK']['s_maker']."'";
	}

	//	分類
	if ($_SESSION['CHECK']['s_class']) {
		$where .= " AND ps.class_m='".$_SESSION['CHECK']['s_class']."'";
	}

	//	商品名
	if ($_SESSION['CHECK']['s_goods_name']) {
		$where .= " AND ps.goods_name='".$_SESSION['CHECK']['s_goods_name']."'";
	}

	//	検索
	$sql  = "SELECT count(*) AS count FROM ".TB_PDA_SEARCH." ps".
			" LEFT JOIN ".TB_PDA_CHECK." pc ON pc.pluid=ps.pluid".
			" WHERE pc.state='1'".
			" AND pc.pluid>'0'".
			$where.";";
	if ($result = pg_query(DB,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}
	$goods_list = "";
	$page_all = 0;
	if ($count > 0) {
		$page = $_SESSION['CHECK']['s_page'];
		$view_num = $_SESSION['CHECK']['s_view_num'];
		$page_all = ceil($count / $view_num);
		//	ページセット
		if ($_POST['back_page']) {
			$page -= 1;
			if ($page < 1) { $page = 1; }
		} elseif ($_POST['next_page']) {
			$page += 1;
		}

		if ($page > $page_all) {
			$page = $page_all;
		}
		$_SESSION['CHECK']['s_page'] = $page;

		$offset = ($page - 1) * $view_num;
		$limit = $view_num;
		$display_start = $offset + 1;
		$display_end = $display_start + $view_num;
		if ($display_end > $count) { $display_end = $count; }
		$limit_num = " OFFSET ".$offset." LIMIT ".$limit;

		$limit_num = "";	//	ページ処理削除	//	add ookawara 2009/09/11

		$make_list = "";
		$sql  = "SELECT ps.pluid, ps.stock, ps.price, ps.class_m, ps.maker, ps.goods_name, ps.color, ps.size,".
				" ps.list_num, ps.l_goods_name, ps.l_size, ps.l_color,".
				" ps.maker_name,".
				" pc.pluid as check".
				", ps.set_flag".	//	add ookawara 2009/09/28
				", ps.ru_touki, ps.ru_zenki".	//	add ookawara 2010/11/04
				" FROM ".TB_PDA_SEARCH." ps".
				" LEFT JOIN ".TB_PDA_CHECK." pc ON pc.pluid=ps.pluid".
				" WHERE pc.state='1'".
				" AND pc.pluid>'0'".
				$where.
				" ORDER BY ps.l_goods_name".
				$limit_num.";";
		if ($result = pg_query(DB,$sql)) {
			$i = 1;
			WHILE ($list = pg_fetch_array($result)) {
				$line_color = $i % 2;
				$goods_list .= goods_list($list,$line_color,$L_CLASS_M);
				$i++;
			}
		}
		$INPUTS['GOODSLIST'] = $goods_list;
		$DEL_INPUTS['GOODSN'] = 1;
	} else {
		$DEL_INPUTS['GOODSD'] = 1;
	}

	//	ページ処理
	$INPUTS['NEXTVIEW1'] = "該当数：".$count."件";	//	add ookawara 2009/09/11
/*	//	del ookawara 2009/09/11
	$next_view = page_view($count,$page,$page_all);
	$INPUTS['NEXTVIEW1'] = $next_view;
	if ($count > 0) {
		$INPUTS['NEXTVIEW2'] = $next_view;
	}
*/

//	$INPUTS['CATEGORYTITLE'] = array(result=>'plane', 'value'=>$l1_name);
//	$INPUTS['CATEGORYLIST'] = array(result=>'plane', 'value'=>$category_list);

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);
	$make_html->set_file(TEMP_CHECK);
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}


//	選択データー削除
function del_data(&$ERROR) {

	if (!$_POST['del_pluid']) {
		$ERROR[] = "削除するデーターが選択されておりません。";
		return;
	}

	//	データー削除
	$delete_sql = "";
	if ($_POST['del_pluid']) {
		foreach ($_POST['del_pluid'] AS $pluid => $val) {
			$delete_sql  .= "UPDATE ".TB_PDA_CHECK." SET".
							" state='0'".
							" WHERE pluid='".$pluid."';";
		}
	}
	if ($delete_sql) {
		if (!pg_exec(DB,$delete_sql)) {
			$ERROR[] = "情報を削除できませんでした。";
		}
	}

}


//	選択データー全削除
function del_all_data(&$ERROR) {

	//	データー削除
	$sql  = "UPDATE ".TB_PDA_CHECK." SET".
			" state='0'".
			" WHERE state!='0';";
	if (!pg_exec(DB,$sql)) {
		$ERROR[] = "情報を全削除できませんでした。";
	}

}


//	商品リスト
function goods_list($list,$line_color,$L_CLASS_M) {
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
	$set_flag = $list['set_flag'];	//	add ookawara 2009/09/28
	//	add ookawara 2010/11/09
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

	//	del ookawara 2013/02/18
	//if ($set_flag != "N") {	//	add ookawara 2009/09/29
	//	$goods_line = "goods_line3";

	//	add ookawara 2013/02/18 start
	if ($set_flag == "H" || $set_flag == "K") {
		$goods_line = "goods_line3";
	} elseif ($set_flag == "A") {
		$goods_line = "goods_line4";
	//	add ookawara 2013/02/18 end
	} elseif ($line_color == 1) {
//	if ($line_color == 1) {	//	del ookawara 2009/09/29
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
	//	add ookawara 2010/11/09
	$INPUTS['RUTOUKI'] = $ru_touki;
	$INPUTS['RUZENKI'] = $ru_zenki;

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);
	$make_html->set_file(TEMP_CHECK_LIST);
	$make_html->set_rep_cmd($INPUTS);
	$html = $make_html->replace();

	return $html;
}
?>