<?PHP
//	商品変更・削除
$r_cate_table = "r_cate";

/*
	goodsテーブルにカラムを追加した場合、delgoodsテーブルにも追加した同じカラムを追加しないと
	商品削除ができなくなります。

	state
		0:
		1:新規登録
		2:更新
		3:削除
		4:新規登録（非表示時）


	//	2019/11/19	ohkawara
		オリジナル商品の親番号が設定されていた場合、更新時保存されるようにパラメーターや処理を追加



*/

function goods_3() {
# global	$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,
# 		$mode,$cate_table,$goods_table,$del_goods_table,$IMAGEF,$IMAGEB,$CATE_DIR,
# 		$mc,$sc,$lc;
# global $mc_r, $sc_r;	//	add ookawara 2015/06/23
# global $CATE_DIR;		//	add ookawara 2015/06/25
global $IMAGEF,$IMAGEB,$CATE_DIR,$CATE_DIR,$cate_table,$goods_table,
		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,$del_goods_table;
global  $mc_r, $sc_r, $mc,$sc,$lc,$ad_id;// _POST
global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract; //_POST
$mode=$_POST['mode'];
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$lc=$_POST['lc'];
$mc_r=$_POST['mc_r'];
$sc_r=$_POST['sc_r'];
$ad_id=$_POST['ad_id'];

$brand=$_POST['brand'];
$yahooshop=$_POST['yahooshop'];
$soldout=$_POST['soldout'];
$name=$_POST['name'];
$code=$_POST['code'];
$price=$_POST['price'];
$sale_price=$_POST['sale_price'];
$options=$_POST['options'];
$headline=$_POST['headline'];
$caption=$_POST['caption'];
$abstract=$_POST['abstract'];

	include("./r_y_dir.php");

	//	add ookawara 2015/06/22 start
	if ($_POST['action'] == "リセット") {
		$_POST['mc'] = $_POST['sc'] = $_POST['lc'] = "";
		$mc = $sc = $lc = "";
		$_POST['s_keyword'] = "";
		$_SESSION['master']['search'] = "";
		$_POST['s_soldout'] = "";											//	add ookawara 2015/09/14
		$_POST['s_yahooshop'] = "";											//	add ookawara 2015/09/14
		$_POST['s_amazonshop'] = "";										//	add ookawara 2015/09/14
		$_POST['s_wowmashop'] = "";											//	add ohkawara 2017/02/08
	//} elseif ($_POST['action'] == "list") {							//	del ookawara 2015/09/10
	} elseif ($_POST['action'] == "list" || $_POST['mode'] != "") {		//	add ookawara 2015/09/10
		//	変更・削除詳細ページから戻ってきた場合、絞込の値をセットする
		if ($_SESSION['master']['search']) { 
			$mc = $mc_r = $_SESSION['master']['search']['mc'];
			$sc = $sc_r = $_SESSION['master']['search']['sc'];
			$lc = $_SESSION['master']['search']['lc'];
			$_POST['s_keyword'] = $_SESSION['master']['search']['s_keyword'];

			$_POST['s_soldout'] = $_SESSION['master']['search']['s_soldout'];		//	add ookawara 2015/09/14
			$_POST['s_yahooshop'] = $_SESSION['master']['search']['s_yahooshop'];	//	add ookawara 2015/09/14
			$_POST['s_amazonshop'] = $_SESSION['master']['search']['s_amazonshop'];	//	add ookawara 2015/09/14
			$_POST['s_wowmashop'] = $_SESSION['master']['search']['s_wowmashop'];	//	add ohkawara 2017/02/08
		}
	}
	//	add ookawara 2015/06/22 end

	$CATE_DIR = "../category";
	$c_file = "$CATE_DIR/category.inc";
	$mc_file = "$CATE_DIR/$mc.dat";
	$sc_file = "$CATE_DIR/$mc" . "_$sc.dat";
	$lc_file = "$CATE_DIR/$mc" . "_$sc" . "_$lc.dat";
	$b_file = "../data/brand.dat";
	$list_file = "./data/ypath.dat";
	$IMAGEF = "../imagef";
	$IMAGEB = "../imageb";

	//	add ookawara 2013/07/03
	define("IMAGE03", "../image03");
	define("IMAGE04", "../image04");
	define("IMAGE05", "../image05");

	//	add ookawara 2015/10/018
	define("IMAGE06", "../image06");
	define("IMAGE07", "../image07");
	define("IMAGE08", "../image08");
	define("IMAGE09", "../image09");

	$cate_table = "category";
	$goods_table = "goods";
	$del_goods_table = "delgoods";

	if ($mode == "表示する") { hyouji(); }
	elseif ($mode == "非表示する") { hihyouji(); }
	elseif ($mode == "renew") { renew(); }
	elseif ($mode == "完全削除") { delete1(); }
	elseif ($mode == "カテゴリーから削除") { delete2(); }
	elseif ($mode == "写真更新") { picrenew(); }
	elseif ($mode == "↑") { up(); }
	elseif ($mode == "↓") { down(); }

	if ($mode == "変更" || $mode == "renew" || $mode == "showPost") { hen(); }
	elseif ($mode == "削除") { del(); }
	else { first00(); }

}


function first00() {
//  global	$PHP_SELF,$ERROR,$msg,$cate_table,$goods_table,
//  		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,
//  		$main,$goods,$IMAGEF,$IMAGEB,
//  		$mc,$sc,$lc,$mc_r,$sc_r,$brand,$ypath,
//  		$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract;
//  global $r_cate_table;	//	add ookawara 2015/06/22
global	$PHP_SELF,$ERROR,$cate_table,$goods_table,$IMAGEF,$IMAGEB,
		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,$r_cate_table,$GG_N;
global  $mc_r, $sc_r, $mc,$sc,$lc;// @source goods_3()<_POST
global $msg;  // @source renew(),delete1(),hen(),del()
global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract; //@source goods_03<_POST,renew(),delete1(),hen(),del() 
include ("../../cone.inc");
$main=$_POST['main'];
$goods=$_POST['goods'];
$ypath=$_POST['ypath'];

	if ($mc != $mc_r) {
		$sc = $lc = "";
	} elseif ($sc != $sc_r) {
		$lc = "";
	}

	//	add ookawara 2015/06/22
	$_SESSION['master']['search']['mc'] = $mc;
	$_SESSION['master']['search']['sc'] = $sc;
	$_SESSION['master']['search']['lc'] = $lc;
	$_SESSION['master']['search']['s_keyword'] = $_POST['s_keyword'];
	$_SESSION['master']['search']['s_soldout'] = $_POST['s_soldout'];			//	add ookawara 2015/09/14
	$_SESSION['master']['search']['s_yahooshop'] = $_POST['s_yahooshop'];		//	add ookawara 2015/09/14
	$_SESSION['master']['search']['s_amazonshop'] = $_POST['s_amazonshop'];		//	add ookawara 2015/09/14
	$_SESSION['master']['search']['s_wowmashop'] = $_POST['s_wowmashop'];		//	add ohkawara 2017/02/08

	$C_LIST = file($c_file);
	if ($mc) { $selected = "selected"; } else { $selected = ""; }
	$l_mc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
	foreach ($C_LIST AS $val) {
		list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
		if ($mc == $mc_num_) { $selected = "selected"; $mc_name = $mc_name_; } else { $selected = ""; }
		$l_mc .= "        <OPTION value=\"$mc_num_\" $selected>$mc_name_</OPTION>\n";
	}

	if ($mc && file_exists($mc_file)) {
		$SC_LIST = file($mc_file);
		if ($sc) { $selected = "selected"; } else { $selected = ""; }
		$l_sc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

		$SUB_CATEGORY = array();
		foreach ($SC_LIST AS $val) {
			list($h_num_,$num_,$sc_name_) = explode("<>",$val);
			$SUB_CATEGORY[$h_num_] = "$num_<>$sc_name_<>";
		}
		krsort($SUB_CATEGORY);

		foreach ($SUB_CATEGORY AS $val) {
			$list = explode("<>",$val);
			if ($sc == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
			$l_sc .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
		}
	} else {
		$l_sc = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
	}

	if ($sc && file_exists($sc_file)) {
		$LC_LIST = file($sc_file);
		if ($lc) { $selected = "selected"; } else { $selected = ""; }
		$l_lc = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";

		$SUB_CATEGORY2 = array();
		foreach ($LC_LIST AS $val) {
			list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
			$SUB_CATEGORY2[$h_num_] = "$lc_num_<>$lc_name_<>";
		}
		krsort($SUB_CATEGORY2);

		foreach ($SUB_CATEGORY2 AS $val) {
			$list = explode("<>",$val);
			if ($lc == $list[0]) { $selected = "selected"; $sc_name = $list[1]; } else { $selected = ""; }
			$l_lc .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
		}
	} else {
		$l_lc = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
	}

	if ($yahooshop == 1) { $y_checked = "checked"; } else { $y_checked = ""; }	//	add ookawara 2015/06/25
	if ($soldout == 1) { $s_checked = "checked"; } else { $s_checked = ""; }

	$name = stripslashes($name);
	$code = stripslashes($code);
	$options = stripslashes($options);
	$headline = stripslashes($headline);
	$abstract = stripslashes($abstract);

	if ($ERROR) { ERROR($ERROR); }

/*
	//	del ookawara 2015/06/22
	$sql = "SELECT count(*) FROM $cate_table WHERE state<='2'";
	if ($mc) { $sql .= " AND cate1='$mc'"; }
	if ($sc) { $sql .= " AND cate2='$sc'"; }
	if ($lc) { $sql .= " AND cate3='$lc'"; }
	$sql .= ";";
*/

	//	add ookawara 2015/06/22 start
	$s_keyword = "";
	$S_KEY_LIST = array();
	if ($_POST['s_keyword']) {
		$s_keyword = $_POST['s_keyword'];
#		$s_keyword = mb_convert_kana($s_keyword, "asKV", "EUC-JP");
		$s_keyword = mb_convert_kana($s_keyword, "asKV", "UTF-8");
		$s_keyword = trim($s_keyword);
		$S_KEY_LIST = explode(" ", $s_keyword);
	}

	$max = 0;
	$where = "";
	$where .= " WHERE ct.state<='2'";
	$where .= " AND gt.del_flg='0'";
	if ($mc) { $where .= " AND ct.cate1='".$mc."'"; }
	if ($sc) { $where .= " AND ct.cate2='".$sc."'"; }
	if ($lc) { $where .= " AND ct.cate3='".$lc."'"; }
	if (count($S_KEY_LIST) > 0) {
		$flg = 0;
		$where .= " AND (";
		foreach ($S_KEY_LIST AS $key => $val) {
			if ($flg != 0) {
				$where .= " AND";
			}
			$where .= " (";
			$where .= " gt.g_name like '%".$val."%'";
			$where .= " OR gt.code like '%".$val."%'";
			if (is_numeric($val)) {
				$where .= " OR gt.g_num='$val'";
			//	add ookawara 2018/01/19 start
			} elseif (preg_match("/^g([0-9])/",$val)) {
				$g_num_val = preg_replace('/[^0-9]/', '', $val);
				$where .= " OR gt.g_num='$g_num_val'";
			//	add ookawara 2018/01/19 end
			}
			$where .= " )";
			$flg = 1;
		}
		$where .= ")";
	}
	//	add ookawara 2015/09/14	start
	$checked_s_soldout = "";
	$checked_s_yahooshop = "";
	$checked_s_amazonshop = "";
	if ($_POST['s_soldout'] == 1) {
		$where .= " AND gt.soldout='1'";
		$checked_s_soldout = "checked";
	}
	if ($_POST['s_yahooshop'] == 1) {
		$where .= " AND gt.yahooshop='1'";
		$checked_s_yahooshop = "checked";
	}
	if ($_POST['s_amazonshop'] == 1) {
		$where .= " AND gt.amazonshop='1'";
		$checked_s_amazonshop = "checked";
	}
	//	add ookawara 2015/09/14	end

	//	add ohkawara 2017/02/08
	$checked_s_wowmashop = "";
	if ($_POST['s_wowmashop'] == 1) {
		$where .= " AND gt.wowmashop='1'";
		$checked_s_wowmashop = "checked";
	}

	$sql  = "SELECT count(*) FROM ".$cate_table." ct";
	$sql .= " LEFT JOIN $goods_table gt ON gt.g_num=ct.g_num";
	$sql .= $where.";";
	//	add ookawara 2015/06/22 end

	//	add ookawara 2015/09/16
	if (!$mc) {
		$sql  = "SELECT count(distinct gt.g_num) FROM ".$cate_table." ct";
		$sql .= " LEFT JOIN $goods_table gt ON gt.g_num=ct.g_num";
		$sql .= $where.";";
	}

 	if ($result = mysqli_query($conn_id,$sql)) {
		list($max) = mysqli_fetch_array($result);
		if (!$mc) {
			$tourokusu = "総登録数： $max 件<BR>\n<BR>\n";
		} else {
			$tourokusu = "このカテゴリーの総登録数： $max 件<BR>\n<BR>\n";
		}
	}

	echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc_r" value="$sc">
変更・削除をする商品のカテゴリー選択、もしくはキーワードを入力、その他を選択し検索ボタンを押して下さい。<BR>
<table border="0" style="font-size : 12px;" cellpadding="1" cellspacing="1" bgcolor="#666666">
	<tr bgcolor="#ffffff">
		<th bgcolor="#cccccc">検索キーワード</th>
		<td><input type="text" size="50" name="s_keyword" value="$s_keyword" /></td>
		<td rowspan="3"><input type="submit" value="検索" style="margin:10px;"></td>
		<td rowspan="3"><input type="submit" name="action" value="リセット" style="margin:10px;"></td>
	</tr>
	<tr bgcolor="#ffffff">
		<th bgcolor="#cccccc">カテゴリー</th>
		<td>
<select name="mc" onchange="submit();">
$l_mc
</select>
<select name="sc" onchange="submit();">
$l_sc
</select>
<select name="lc" onchange="submit();">
$l_lc
</select>
		</td>
	</tr>
	<tr bgcolor="#ffffff">
		<th bgcolor="#cccccc">その他</th>
		<td>
<input type="checkbox" name="s_soldout" value="1" {$checked_s_soldout} />：SoldOut 
<input type="checkbox" name="s_yahooshop" value="1" {$checked_s_yahooshop} />：YahooShop 
<input type="checkbox" name="s_amazonshop" value="1" {$checked_s_amazonshop} />：AmazonShop 
<input type="checkbox" name="s_wowmashop" value="1" {$checked_s_wowmashop} />：WowmaShop 


		</td>
	</tr>
</table>



</FORM>
$tourokusu

WAKABA;

/*
	//	del ookawara 2015/06/22
	if ($mc && $sc && $lc) {
		if (file_exists($lc_file)) {
			$LIST = file($lc_file);
			$i = 1;
			foreach ($LIST AS $val) {
				list($g_num_) = explode("<>",$val);
				$GOODS[$i] = $g_num_;
				$i++;
			}
		}
		$count = 0;
		$count = count($GOODS);
*/

	//	add ookawara 2015/06/25
	if (!$_POST['s_keyword'] && $mc && $sc && $lc) {
		if (file_exists($lc_file)) {
			$LIST = file($lc_file);
			$i = 1;
			foreach ($LIST AS $val) {
				list($g_num_) = explode("<>",$val);
				$GOODS[$i] = $g_num_;
				$i++;
			}
		}
	}
		//	del ookawara 2015/06/22
		//if ($count <= 0) {
		//	echo "今現在このカテゴリーには商品は登録されておりません。<BR>\n<BR>\n";
		//}

	//	add ookawara 2015/06/22 start
	if ($max < 1) {
		echo "該当する商品は登録されておりません。<br />\n";
	} elseif ($lc == "" && $max > 100) {
		//if ($mc != "" || $sc != "" || $lc != "" || $s_keyword != "") {
		//if ($mc != "" || $sc != "" || $lc != "" || $s_keyword != ""	|| $s_soldout != 1	|| $s_yahooshop != 1 || $s_amazonshop != 1 ) {	//	del ohkawara 2017/02/08
		if ($mc != "" || $sc != "" || $lc != "" || $s_keyword != ""	|| $s_soldout != 1	|| $s_yahooshop != 1 || $s_amazonshop != 1 || $s_wowmashop != 1 ) {	//	add ohkawara 2017/02/08
			echo "該当する商品が多すぎます。絞込の情報を増やして下さい。<br />\n";
		} else {
			echo "キーワードもしくは、カテゴリーを選択し検索ボタンを押して下さい。<br />\n";
		}
	//	add ookawara 2015/06/22 end
	} else {
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				$B_LINE[$b_num_] = $b_name_;
			}
		}

		//	del ookawara 2015/06/22
		//$sql =	"SELECT i.num, j.g_num, j.g_name, j.code, j.brand, j.price, j.sale_price, j.yahooshop, j.soldout, i.display" .
		//		" FROM $cate_table i,$goods_table j" .
		//		" WHERE  i.cate1='$mc' AND i.cate2='$sc' AND i.cate3='$lc' AND i.g_num=j.g_num;";

		//	add ookawara 2015/06/22 start
		$sql  = "SELECT ct.num, gt.g_num, gt.g_name, gt.code, gt.brand, gt.price, gt.sale_price, gt.yahooshop, gt.amazonshop, gt.soldout, ct.display,";
		$sql .= " rct1.c_name,  rct2.c_name,  rct3.c_name, gt.name_head,";	//	add name_head ookawara 2015/09/14
		$sql .= " gt.name_foot,";		//	add ookawara 2015/09/24
		$sql .= " gt.zaiko,";			//	add ookawara 2015/09/17
		$sql .= " gt.amz_price,";		//	add ookawara 2015/10/08
		$sql .= " gt.wowmashop,";		//	add ohkawara 2017/02/08
		$sql .= " gt.pg_num,";
		$sql .= " gt.parent_flag,";			//	add thao 2023/05/17
		$sql .= " gt.gp_option,";			//	add thao 2023/05/08
		$sql .= " gt.gp_option_name";			//	add thao 2023/05/12
		$sql .= " FROM ".$cate_table." ct";
		$sql .= " LEFT JOIN $goods_table gt ON gt.g_num=ct.g_num";
		$sql .= " LEFT JOIN $r_cate_table rct1 ON rct1.cate1=ct.cate1 AND rct1.cate2=0 AND rct1.cate3=0";
		$sql .= " LEFT JOIN $r_cate_table rct2 ON rct2.cate1=ct.cate1 AND rct2.cate2=ct.cate2 AND rct2.cate3=0";
		$sql .= " LEFT JOIN $r_cate_table rct3 ON rct3.cate1=ct.cate1 AND rct3.cate2=ct.cate2 AND rct3.cate3=ct.cate3";
		$sql .= $where;
	
		//	add ookawara 2015/09/18
		//if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1") {								//	del ohkawara 2017/02/08
		if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1" && $_POST['s_wowmashop'] != "1") {	//	add ohkawara 2017/02/08
			$sql .= ";";
		} else {
			$sql .= " ORDER BY gt.g_num DESC";
		}
		//	add ookawara 2015/06/22 end
		if (!$result = mysqli_query($conn_id,$sql)) {
			$ERROR[] = "登録商品情報を読み込みできませんでした。";
		} else {
			echo <<<WAKABA
商品の内容を変更する場合は変更を、削除する場合は削除ボタン、各作業のボタンを押して下さい。<BR>
<TABLE border="0" style="font-size : 12px;" cellspacing="1" bgcolor="#666666" cellpadding="3">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TH colspan="23" align="left">登録カテゴリー</TH>
    </TR>
    <TR bgcolor="#cccccc">
      <TH rowspan="2">↑</TH>
      <TH rowspan="2">↓</TH>
      <TH rowspan="2">管理商品番号</TH>
      <TH rowspan="2">商品名</TH>
	  <TH>商品番号</TH>
      <TH rowspan="2">商品グループ親商品</TH>
	  <TH>商品グループ選択肢</TH>
      <TH>金額</TH>
      <TH rowspan="2">Amazon<br>金額</TH>
      <TH>YahooShop</TH>
      <TH rowspan="2">Wowma<br>Shop</TH>
      <TH>SoldOut</TH>
      <TH>画像Ｆ</TH>
      <TH>画像Ｂ</TH>
      <TH>画像03</TH>
      <TH>画像04</TH>
      <TH>画像05</TH>
      <TH rowspan="2">表示</TH>
      <TH rowspan="2">変更</TH>
      <TH rowspan="2">写真更新</TH>
      <TH rowspan="2">削除</TH>
    </TR>
    <TR bgcolor="#cccccc">
      <TH>ブランド</TH>
	  <TH>商品グループ選択名</TH>
      <TH>特価</TH>
      <TH>AmazonShop</TH>
      <TH>一品限り</TH>
      <TH>画像06</TH>
      <TH>画像07</TH>
      <TH>画像08</TH>
      <TH>画像09</TH>
      <TH>--</TH>
    </TR>

WAKABA;

			$check_flg = 0;		//	add ookawara 2015/10/30
			$max = mysqli_num_rows($result);
			for($i=0; $i<$max; $i++) {
				//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$soldout,$display) = mysqli_fetch_array($result,$i);
				//$GOODS_L[$g_num] = "$num<>$g_num<>$name<>$code<>$brand<>$price<>$sale_price<>$yahooshop<>$soldout<>$display<>\n";
				//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$zaiko) = mysqli_fetch_array($result,$i);	//	add name_head ookawara 2015/09/14	//	add zaiko	ookawara 2015/09/17	//	del ookawara 2015/09/24
				//$GOODS_L[$g_num] = "$num<>$g_num<>$name<>$code<>$brand<>$price<>$sale_price<>$yahooshop<>$amazonshop<>$soldout<>$display<>$cate1_name<>$cate2_name<>$cate3_name<>$name_head<>$zaiko<>\n";	//	add name_head ookawara 2015/09/14	//	add zaiko	ookawara 2015/09/17	//	del ookawara 2015/09/24
				//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price) = mysqli_fetch_array($result,$i);	//	add ookawara 2015/09/24			//	add amz_price ookawara 2015/10/08
				//$GOODS_L[$g_num] = "$num<>$g_num<>$name<>$code<>$brand<>$price<>$sale_price<>$yahooshop<>$amazonshop<>$soldout<>$display<>$cate1_name<>$cate2_name<>$cate3_name<>$name_head<>$name_foot<>$zaiko<>$amz_price<>\n";	//	add ookawara 2015/09/24	//	add amz_price ookawara 2015/10/08	//	del ohkawara 2017/02/08
                
				list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price,$wowmashop,$pg_num,$parent_flag,$gp_option,$gp_option_name) = mysqli_fetch_array($result);				//	add ohkawara 2017/02/08
				$GOODS_L[$g_num] = "$num<>$g_num<>$name<>$code<>$brand<>$price<>$sale_price<>$yahooshop<>$amazonshop<>$soldout<>$display<>$cate1_name<>$cate2_name<>$cate3_name<>$name_head<>$name_foot<>$zaiko<>$amz_price<>$wowmashop<>$pg_num<>$parent_flag<>$gp_option<>$gp_option_name\n";		//	add ohkawara 2017/02/08
			}
			
			//	DBに保存されているデーターとファイルで保存されているデーターに差異がある場合ファイルに追加
			//	add ookawara 2015/10/30
			if (count($GOODS) < count($GOODS_L)) {
				$CHK_LIST = $GOODS_L;
				foreach ($GOODS AS $line) {
					list($g_num) = explode("<>", $line);
					if ($CHK_LIST[$g_num]) {
						unset($CHK_LIST[$g_num]);
					}
				}
				if ($CHK_LIST) {
					foreach ($CHK_LIST AS $key => $line) {
						$GOODS[] = $key;
						$check_flg = 1;
					}
				}
			}

			//$last = $count - 1;	//	del ookawara 2015/06/24
			$last = $max - 1;		//	add ookawara 2015/06/24
			$i = 0;
			//foreach ($GOODS AS $val) {	//	del ookawara 2015/06/22

			$SET_LIST = $GOODS_L;								//	add ookawara 2015/06/22

			//if (!$_POST['s_keyword'] && $mc && $sc && $lc) {	//	add ookawara 2015/06/22		//	del ookawara 2015/09/14
			//if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1") {
								//	del ohkawara 2017/02/08
			if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1" && $_POST['s_wowmashop'] != "1") {	//	add ohkawara 2017/02/08
				$SET_LIST = $GOODS;								//	add ookawara 2015/06/22
			}													//	add ookawara 2015/06/22

			$CHK_GOODS_LIST = array();	//	add ookawara 2015/10/29	DBに登録してあるg_numを入れておく
			foreach ($SET_LIST AS $val) {	//	add ookawara 2015/06/22
				//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$soldout,$display) = explode("<>",$GOODS_L[$val]);	//	del ookawara 2015/06/22
				//if (!$_POST['s_keyword'] && $mc && $sc && $lc) {																												//	del ookawara 2015/09/14
				//if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1") {				//	add ookawara 2015/09/14		//	del ohkawara 2017/02/08

				if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1" && $_POST['s_wowmashop'] != "1") {				//	add ohkawara 2017/02/08
					//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price) = explode("<>", $GOODS_L[$val]);	//	add ookawara 2015/06/22	//	add name_head ookawara 2015/09/14	//	add zaiko	ookawara 2015/09/17	//	add name_foot ookawara 2015/09/24	//	add amz_price ookawara 2015/10/08	//	del ohkawara 2017/02/08

					list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price,$wowmashop,$pg_num,$parent_flag,$gp_option,$gp_option_name) = explode("<>", $GOODS_L[$val]);	//	add ohkawara 2017/02/08
				} else {
					//list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price) = explode("<>", $val);	//	add ookawara 2015/06/22	//	add name_head ookawara 2015/09/14	//	add zaiko	ookawara 2015/09/17	//	add name_foot ookawara 2015/09/24	//	add amz_price ookawara 2015/10/08	//	del ookawara 2015/09/14

					list($num,$g_num,$name,$code,$brand,$price,$sale_price,$yahooshop,$amazonshop,$soldout,$display,$cate1_name,$cate2_name,$cate3_name,$name_head,$name_foot,$zaiko,$amz_price,$wowmashop,$pg_num,$parent_flag,$gp_option,$gp_option_name) = explode("<>", $val);	//	add ookawara 2015/09/14
				}
				//	add ookawara 2015/10/29
				if ($g_num == "") {
					$check_flg = 1;	//	add ookawara 2015/10/30
					continue;
				} else {
					$CHK_GOODS_LIST[$g_num] = $g_num;
				}

				if ($sale_price > 0) { $sales = "$sale_price 円"; } else { $sales = "---"; }
				if ($yahooshop == 1) { $ys = "○"; } else { $ys = "×"; }
				if ($amazonshop == 1) { $as = "○"; } else { $as = "×"; }
				if ($wowmashop == 1) { $ws = "○"; } else { $ws = "×"; }		//	add ohkawara 2017/02/08
				if ($soldout == 1) { $so = "○"; } else { $so = "×"; }
				if ($display == 0) { $sp = "非表示中 <INPUT type=\"submit\" name=\"mode\" value=\"表示する\">"; }
				else { $sp = "表示中 <INPUT type=\"submit\" name=\"mode\" value=\"非表示する\">"; }
				if ($i != 0) { $up = "<INPUT type=\"submit\" name=\"mode\" value=\"↑\">"; } else { $up = ""; }
				if ($i != $last) { $down = "<INPUT type=\"submit\" name=\"mode\" value=\"↓\">"; } else { $down = ""; }

				//	add ookawara 2015/06/22
				//if ($s_keyword != "" || $lc == "") {																									//	del ookawara 2015/09/14

				if ($s_keyword != "" || $lc == "" || $_POST['s_soldout'] == "1" || $_POST['s_yahooshop'] == "1" || $_POST['s_amazonshop'] == "1" ) {	//	add ookawara 2015/09/14
					$up = $down = "--";
				}

				$if_file = "$IMAGEF/$code.jpg";
				if (file_exists($if_file)) { $imagef = "○"; } else { $imagef = "×"; }
				$ib_file = "$IMAGEB/$code.jpg";
				if (file_exists($ib_file)) { $imageb = "○"; } else { $imageb = "×"; }
/*
				//	del ookawara 2015/10/08
				//	add ookawara 2013/07/03
				$i03_file = IMAGE03."/$code.jpg";
				if (file_exists($i03_file)) { $image03 = "○"; } else { $image03 = "×"; }
				$i04_file = IMAGE04."/$code.jpg";
				if (file_exists($i04_file)) { $image04 = "○"; } else { $image04 = "×"; }
				$i05_file = IMAGE05."/$code.jpg";
				if (file_exists($i05_file)) { $image05 = "○"; } else { $image05 = "×"; }
*/
				//	add ookawara 2015/10/08
				for ($ii=3; $ii<=9; $ii++) {
					$image_name = "image0".$ii;
					$$image_name = "×";
					$i_num_file = "../".$image_name."/$code.jpg";
					if (file_exists($i_num_file)) {
						$$image_name = "○";
					}
				}

				//	add ookawara 2013/02/07
				$bgcolor = "#ffffff";
				if ($_POST['bg_g_num'] == $g_num) {
					$bgcolor = "#ff6666";
				}

				//	add ookawara 2015/09/17
				$zaiko_msg = "--";
				if ($zaiko == 1) {
					$zaiko_msg = "○";
				}

				//	add ookawara 2015/10/08
				if (!$amz_price) {
					$amz_price = "--";
				}
				//	add thao 2023/05/08
				$gp_option=trim($gp_option);

				echo <<<WAKABA
    <TR bgcolor="{$bgcolor}">
      <th colspan="23" align="left">$cate1_name > $cate2_name > $cate3_name</th>
    </TR>
    <TR bgcolor="{$bgcolor}" align="center">
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="main" value="$main">
      <INPUT type="hidden" name="goods" value="$goods">
      <INPUT type="hidden" name="g_num" value="$g_num">
      <INPUT type="hidden" name="bg_g_num" value="$g_num">
      <INPUT type="hidden" name="num" value="$num">
      <INPUT type="hidden" name="s_keyword" value="$s_keyword">
      <INPUT type="hidden" name="s_soldout" value="{$_POST['s_soldout']}">
      <INPUT type="hidden" name="s_yahooshop" value="{$_POST['s_yahooshop']}">
      <INPUT type="hidden" name="s_amazonshop" value="{$_POST['s_amazonshop']}">
      <INPUT type="hidden" name="s_wowmashop" value="{$_POST['s_wowmashop']}">
      <TD rowspan="2">$up</TD>
      <TD rowspan="2">$down</TD>
      <TD rowspan="2" style="font-weight:bold;font-size:120%;">$g_num</TD>
      <TD rowspan="2"><a href='/goods/g{$g_num}/' target='_blank'>{$name_head}{$name}{$name_foot}</a></TD>
      <TD>$code</TD>
	  <TD rowspan="2">$pg_num</TD>
	  <TD>$GG_N[$gp_option]</TD>
      <TD>$price 円</TD>
      <TD rowspan="2">$amz_price 円</TD>
      <TD>$ys</TD>
      <TD rowspan="2">$ws</TD>
      <TD>$so</TD>
      <TD>$imagef</TD>
      <TD>$imageb</TD>
      <TD>$image03</TD>
      <TD>$image04</TD>
      <TD>$image05</TD>
      <TD rowspan="2">$sp</TD>
      <TD rowspan="2"><INPUT type="submit" name="mode" value="変更"></TD>
      <TD rowspan="2"><INPUT type="submit" name="mode" value="写真更新"></TD>
      <TD rowspan="2"><INPUT type="submit" name="mode" value="削除"></TD>
      </FORM>
    </TR>
    <TR bgcolor="{$bgcolor}" align="center">
      <TD>$B_LINE[$brand]</TD>
	  <TD>$gp_option_name</TD>
      <TD>$sales</TD>
      <TD>$as</TD>
      <TD>$zaiko_msg</TD>
      <TD>$image06</TD>
      <TD>$image07</TD>
      <TD>$image08</TD>
      <TD>$image09</TD>
      <TD>--</TD>
    </TR>

WAKABA;
				$i++;
			}
			echo <<<WAKABA
  </TBODY>
</TABLE>

WAKABA;

			//	add ookawara 2015/10/29
			//	ファイルカテゴリー登録商品不具合修正
			if ($check_flg == 1) {													//	add ookawara 2015/10/30
			//if ($CHK_GOODS_LIST && count($GOODS) != count($CHK_GOODS_LIST)) {		//	del ookawara 2015/10/30
				//if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1") {									//	del ohkawara 2017/02/08
				if (!$_POST['s_keyword'] && $mc && $sc && $lc && $_POST['s_soldout'] != "1" && $_POST['s_yahooshop'] != "1" && $_POST['s_amazonshop'] != "1" && $_POST['s_wowmashop'] != "1") {		//	add ohkawara 2017/02/08
					global $CATE_DIR;
					$lc_file = $CATE_DIR."/".$mc."_".$sc."_".$lc.".dat";
					if (file_exists($lc_file)) {
						$new_list = "";
						$LIST = file($lc_file);
						foreach ($LIST AS $val) {
							$val = trim($val);
							list($g_num_) = explode("<>",$val);
							if ($CHK_GOODS_LIST[$g_num_]) {
								$new_list .= $val."\n";
								unset($CHK_GOODS_LIST[$g_num_]);
							}
						}
						if ($CHK_GOODS_LIST) {
							foreach ($CHK_GOODS_LIST AS $g_num_) {
								$new_list .= $g_num_."<>\n";
							}
						}
						if ($new_list) {
							$OUT = fopen($lc_file,"w");
							fwrite($OUT,$new_list);
							fclose($OUT);
						} else {
							unlink($lc_file);
						}
					}
				}
			}

		}
	}


	//mysqli_close($conn_id);

}



function hen() {
// global	$PHP_SELF,$ERROR,$msg,$cate_table,$goods_table,$mode,
// 		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,
// 		$main,$goods,$mc,$sc,$lc,$g_num,$num,
// 		$mc_r,$sc_r,$brand,$ypath,
// 		$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment
// 		,$rd_id,$yd_id;
// global $CATE_DIR;		//	add ookawara 2015/06/25
// global $ad_id;			//	add ookawara 2015/07/24
global	$PHP_SELF,$ERROR,$CATE_DIR,$cate_table,$goods_table,
		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,$GG_N;

global $mc,$sc,$lc,$ad_id; //@source goods_3<_POST
global $msg; 
global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract; //@source goods_3<_POST
$mode=$_POST['mode'];
$main=$_POST['main'];
$goods=$_POST['goods'];
$g_num=$_POST['g_num'];
$num=$_POST['num'];
$ypath=$_POST['ypath'];


$comment=$_POST['comment'];
$rd_id=$_POST['rd_id'];
$yd_id=$_POST['yd_id'];


	//	add ookawara 2015/06/22
	$s_keyword = "";
	if ($_POST['s_keyword'] != "") {
		$s_keyword = $_POST['s_keyword'];
	}

	//	add ookawara 2015/09/14
	if ($_POST['s_soldout'] == 1) {
		$where .= " AND gt.soldout='1'";
		$checked_s_soldout = "checked";
	}
	if ($_POST['s_yahooshop'] == 1) {
		$where .= " AND gt.yahooshop='1'";
		$checked_s_yahooshop = "checked";
	}
	if ($_POST['s_amazonshop'] == 1) {
		$where .= " AND gt.amazonshop='1'";
		$checked_s_amazonshop = "checked";
	}

	//	add ohkawara 2017/02/08
	if ($_POST['s_wowmashop'] == 1) {
		$where .= " AND gt.wowmashop='1'";
		$checked_s_wowmashop = "checked";
	}

	$flag = 0;
	if (!$ERROR and $mode != "showPost") {
		include ("../../cone.inc");
		$sql =	"SELECT * FROM $cate_table i, $goods_table j" .
				" WHERE i.g_num=j.g_num AND i.num='$num' AND i.state!='3';";
		if (!$result = mysqli_query($conn_id,$sql)) {
			$ERROR[] = "登録商品情報を読み込みできませんでした。";
			$flag = 1;
		} else {
			list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$state,$g_num,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg,$amazonshop,$ad_id, $size_list, $name_head, $bullet_point1, $bullet_point2, $bullet_point3, $bullet_point4, $bullet_point5, $generic_keywords1, $generic_keywords2, $generic_keywords3, $generic_keywords4, $generic_keywords5, $name_foot, $product_subtype, $ad_id2, $department_name, $regist_time, $update_time, $distribution_designation, $model_year, $seasons, $amz_price, $wd_id, $wowmashop,$pg_num,$parent_flag,$gp_option,$gp_option_name) = mysqli_fetch_array($result);	//	add ohkawara 2017/02/08
		}
	//	add ookawara 2015/09/14	start
	} else {
		$name_head = $_POST['name_head'];
		$pg_num = $_POST['pg_num'];
		$parent_flag = $_POST['parent_flag'];
		$gp_option = $_POST['gp_option'];
		$gp_option_name = $_POST['gp_option_name'];
		$cate1 = $_POST['cate1'];
		$cate2 = $_POST['cate2'];
		$cate3 = $_POST['cate3'];
		$cate3 = $_POST['cate3'];
		$yahooshop = $_POST['yahooshop'];
		$soldout = $_POST['soldout'];
		$amazonshop = $_POST['amazonshop'];
		$zaiko = $_POST['zaiko'];
		$wowmashop = $_POST['wowmashop'];
		$bullet_point1 = $_POST['bullet_point1'];
		$bullet_point2 = $_POST['bullet_point2'];
		$bullet_point3 = $_POST['bullet_point3'];
		$bullet_point4 = $_POST['bullet_point4'];
		$bullet_point5 = $_POST['bullet_point5'];
		$generic_keywords1 = $_POST['generic_keywords1'];
		$generic_keywords2 = $_POST['generic_keywords2'];
		$generic_keywords3 = $_POST['generic_keywords3'];
		$generic_keywords4 = $_POST['generic_keywords4'];
		$generic_keywords5 = $_POST['generic_keywords5'];
	//	add ookawara 2015/09/14	end
		$zaiko = $_POST['zaiko'];		//	add ookawara 2015/09/17
		$name_foot = $_POST['name_foot'];				//	add ookawara 2015/09/24
		$product_subtype = $_POST['product_subtype'];	//	add ookawara 2015/09/24
		$ad_id2 = $_POST['ad_id2'];						//	add ookawara 2015/09/24
		$department_name = $_POST['department_name'];						//	add ookawara 2015/09/27

		$distribution_designation = $_POST['distribution_designation'];	//	add ookawara 2015/10/01
		$model_year = $_POST['model_year'];								//	add ookawara 2015/10/01
		$seasons = $_POST['seasons'];									//	add ookawara 2015/10/01
		$amz_price = $_POST['amz_price'];								//	add ookawara 2015/10/08

		$wd_id = $_POST['wd_id'];								//	add ohkawara 2017/02/08
	}
	if ($ERROR) { ERROR($ERROR); }

	if ((!$ERROR && $flag == 0) || $mode == "renew" || $mode == "showPost") {

		//	add ookawara 2015/06/25
		$mc_file = "$CATE_DIR/".$cate1.".dat";
		$sc_file = "$CATE_DIR/".$cate1."_".$cate2.".dat";
		$lc_file = "$CATE_DIR/".$cate1."_".$cate2."_".$cate3.".dat";
		$mc = $cate1;
		$sc = $cate2;
		$lc = $cate3;

		$C_LIST = file($c_file);
		foreach ($C_LIST AS $val) {
			list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
			if ($mc == $mc_num_) { $mc_name = $mc_name_; break; }
		}

		if ($mc && file_exists($mc_file)) {
			$SC_LIST = file($mc_file);
			foreach ($SC_LIST AS $val) {
				list($h_num_,$sc_num_,$sc_name_) = explode("<>",$val);
				if ($sc == $sc_num_) { $sc_name = $sc_name_; break; }
			}
		}

		if ($sc && file_exists($sc_file)) {
			$LC_LIST = file($sc_file);
			foreach ($LC_LIST AS $val) {
				list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
				if ($lc == $lc_num_) { $lc_name = $lc_name_; break;}
			}
		}

		if (!$brand) { $selected = "selected"; } else { $selected = ""; }
		$l_brand = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				if ($del_ == 1) { continue; }
				$B_LINE[$b_num_] = $b_name_;
				if ($brand == $b_num_) { $selected = "selected"; } else { $selected = ""; }
				$l_brand .= "        <OPTION value=\"$b_num_\" $selected>$b_name_</OPTION>\n";
			}
		}

		if (!$ypath) { $selected = "selected"; } else { $selected = ""; }
		$l_ypath = "        <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
		if (file_exists($list_file)) {
			$Y_LIST = file($list_file);
			foreach ($Y_LIST AS $val) {
				list($y_name_,$ypath_,$del_) = explode("<>",$val);
				if ($del_ == 1) { continue; }
				if ($ypath == $y_name_) { $selected = "selected"; } else { $selected = ""; }
				$l_ypath .= "        <OPTION value=\"$y_name_\" $selected>$y_name_</OPTION>\n";
			}
		}

		if ($yahooshop == 1) { $y_checked = "checked"; } else { $y_checked = ""; }
		if ($amazonshop == 1) { $a_checked = "checked"; } else { $a_checked = ""; }		//	add ookawara 2015/06/24
		if ($soldout == 1) { $s_checked = "checked"; } else { $s_checked = ""; }
		if ($sale_price > 0) { $sale_price = $sale_price; } else { $sale_price = ""; }

		if ($amz_price > 0) { $amz_price = $amz_price; } else { $amz_price = ""; }	//	add ookawara 2015/10/08

		if ($wowmashop == 1) { $w_checked = "checked"; } else { $w_checked = ""; }		//	add ohkawara 2017/02/08
		if ($parent_flag == 1) { $pt_checked = "checked"; } else { $pt_checked = ""; }		//	add thao 2023/05/11
		//	楽天＆Yahooディレクトリー情報設定
//yd_rd_set();
		$l_rd = rd_list($rd_id);
		$l_yd = yd_list($yd_id);
		$l_ad = ad_list($ad_id);	//	add ookawara 2015/07/24
		$l_wd = wd_list($wd_id);	//	add ohkawara 2017/02/08

		//	add ookawara 2015/09/24
		$list_type = 2;
		if (preg_match("/^2_/", $ad_id)) {
			$list_type = 99;
		}
		$l_ad2 = ad_list($ad_id2, $list_type);

		$l_product_subtype = amz_product_subtype($product_subtype, $list_type);

		//	add ookawara 2015/09/17
		$zaiko_checked = "";
		if ($zaiko == 1) {
			$zaiko_checked = "checked";
		}

		//	Amazon 対象年齢・性別
		//	add ookawara 2015/09/27
		$department_name_chk = amz_department_name($department_name, $list_type);

		//	Amazon 販売形態(並行輸入品)
		//	add ookawara 2015/10/01
		$chk_distribution_designation = "";
		if ($distribution_designation == 1) {
			$chk_distribution_designation = "checked";
		}

		//	Amazon モデル年(発売年・発表年)
		//	add ookawara 2015/10/01
		$l_model_year = amz_model_year_list($model_year, $list_type);

		//	Amazon シーズン
		//	add ookawara 2015/10/01
		$l_seasons = amz_seasons_list($seasons, $ad_id);

		//商品グループ選択肢
		//thao add 2023/05/08
		$option=""; 
		if (!$gp_option) { $select = "selected"; } else { $select = "";}
		$option.="<OPTION value='' $select>$GG_N[0]</OPTION>\n";
		$count = count($GG_N);
		$max = $count-1;
		for ($i=1; $i<=$max; $i++) {
			if ($gp_option == $i) { $select = "selected"; } else { $select = "";}
			$option.="<OPTION value='$i' $select>$GG_N[$i]</OPTION>\n";
		}
		if($parent_flag!=1){
			include ("../../cone.inc");
			$sql = "SELECT * FROM $goods_table WHERE pg_num='$g_num' or g_num=$g_num and del_flg='0';";
			if ($result = mysqli_query($conn_id,$sql)) {
				while ($list = mysqli_fetch_array($result)) {
					if ($list["g_num"] && ($list["parent_flag"] == 1) && ($list["g_num"] == $g_num)) {$pt_checked = "checked";break;}
				}
			}
		}
		echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST" name="goods_form">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="num" value="$num">
<INPUT type="hidden" name="g_num" value="$g_num">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="mode" value="renew">
<INPUT type="hidden" name="b_code" value="$code">
<INPUT type="hidden" name="s_keyword" value="$s_keyword">
<INPUT type="hidden" name="s_soldout" value="$s_soldout">
<INPUT type="hidden" name="s_yahooshop" value="$s_yahooshop">
<INPUT type="hidden" name="s_amazonshop" value="$s_amazonshop">
<INPUT type="hidden" name="s_wowmashop" value="$s_wowmashop">
商品情報を修正し、変更ボタンを押して下さい。<BR>
※コピーをした商品がある場合（管理商品番号が同じ）で商品の変更をした場合、変更した内容は管理商品番号が同じすべての商品に適用されます。<BR>
<TABLE border="0" cellpadding="1" cellspacing="1" bgcolor="#666666" style="font-size : 12px;" width="800">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">商品管理番号</TH>
      <TD colspan="3" style="font-weight:bold;font-size:120%;">
      <a href="/goods/g{$g_num}/" target="_blank">{$g_num}</a>
      </TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD colspan="3">$mc_name // $sc_name // $lc_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">ブランド名</TH>
      <TD colspan="3"><SELECT name="brand">
$l_brand
      </SELECT></TD>
    </TR>
<!--
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Ypath</TH>
      <TD colspan="3"><SELECT name="ypath">
$l_ypath
      </SELECT></TD>
    </TR>
-->
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">楽天ディレクトリー</TH>
      <TD colspan="3"><SELECT name="rd_id">
$l_rd
      </SELECT></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">YAHOOディレクトリー</TH>
      <TD colspan="3"><SELECT name="yd_id">
$l_yd
      </SELECT></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード</TH>
	  <!--<TD colspan="3"><SELECT name="ad_id" onchange='submit();'> -->
	  <TD colspan="3"><SELECT name="ad_id" onchange="document.forms['goods_form'].elements['mode'].value='showPost';submit();">
$l_ad
      </SELECT><br>
      シューズやバッグは、スポーツ＆アウトドアではなくシューズ＆バッグから始まるブラウズノードを選択して下さい。<br>
      ユニフォームや衣類は、スポーツ＆アウトドアではなく服＆ファッション小物から始まるブラウズノードを選択して下さい。
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Wowmaディレクトリー</TH>
      <TD colspan="3"><SELECT name="wd_id">
$l_wd
      </SELECT></TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品タイプ</TH>
      <TD colspan="3"><SELECT name="product_subtype">
$l_product_subtype
      </SELECT>
      Amazonブラウズノードで、服＆ファッション小物または、シューズ＆バッグのノードを選択時に設定する。
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード2</TH>
      <TD colspan="3"><SELECT name="ad_id2">
$l_ad2
      </SELECT><br>
      Amazon ブラウズノードで選択した以外で該当するカテゴリーがあった場合選択して下さい。<br>
      ユニフォームやシューズで、Amazon ブラウズノードでは、スポーツのカテゴリーで選択し、Amazon ブラウズノード2で服＆ファッションのカテゴリーの選択はしないで下さい。
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon 対象年齢・性別</TH>
      <TD colspan="3">
$department_name_chk
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon 販売形態(並行輸入品)</TH>
      <TD colspan="3">
      <input type="checkbox" name="distribution_designation" value="1" {$chk_distribution_designation} />：並行輸入品
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon モデル年(発売年・発表年)</TH>
      <TD colspan="3"><SELECT name="model_year">
$l_model_year
      </SELECT> 年<br>
      Amazonブラウズノードで、服＆ファッション小物または、シューズ＆バッグのノードを選択時に設定する。
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon シーズン</TH>
      <TD colspan="3"><SELECT name="seasons">
$l_seasons
      </SELECT><br>
      Amazonブラウズノードで、服＆ファッション小物または、シューズ＆バッグのノードを選択時に設定する。
      </TD>
    </TR>

    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Yshop</TH>
      <TD width="220"><INPUT type="checkbox" name="yahooshop" value="1" $y_checked></TD>
      <TH bgcolor="#cccccc" width="80">Soldout</TH>
      <TD width="220"><INPUT type="checkbox" name="soldout" value="1" $s_checked></TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Ashop</TH>
      <TD width="220"><INPUT type="checkbox" name="amazonshop" value="1" $a_checked></TD>
      <TH bgcolor="#cccccc" width="80">在庫一品限り</TH>
      <TD width="220"><INPUT type="checkbox" name="zaiko" value="1" $zaiko_checked></TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Wshop</TH>
      <TD colspan="3"><INPUT type="checkbox" name="wowmashop" value="1" $w_checked></TD>
    </TR>
	<TR bgcolor="#ffffff">
	<TH bgcolor="#cccccc">商品グループ親番号</TH>
	<TD><INPUT type="text" name="pg_num" value="$pg_num">※半角数字<br>
	商品グループ親番号は楽天市場で使用されます。<br>
	商品グループ親番号を設定すると、楽天市場では親番号の商品のカテゴリーに表示されるようになります。</TD>
	<TH bgcolor="#cccccc" width="50">親商品</TH>
	<TD width="150" align="center"><INPUT type="checkbox" name="parent_flag" value="1" $pt_checked></TD>
    </TR>
	<TR bgcolor="#ffffff">
		<TH bgcolor="#cccccc">商品グループ選択肢</TH>
		<TD colspan="3">
		<SELECT name="gp_option">$option</SELECT>
		</TD>
		</TD>
    </TR>
	<TR bgcolor="#ffffff">
	<TH bgcolor="#cccccc">商品グループ選択肢名</TH>
	<TD colspan="3"><INPUT size="40" type="text" name="gp_option_name" value="$gp_option_name"></TD>
	</TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名ヘッダ</TH>
      <TD colspan="3"><INPUT size="60" type="text" name="name_head" value="$name_head"><br>
      商品名ヘッダ + 商品名 + 商品名フッタ合計 全角６４文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名 ※</TH>
      <TD colspan="3"><INPUT size="60" type="text" name="name" value="$name"><br>
      商品名ヘッダ + 商品名 + 商品名フッタ合計 全角６４文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名フッタ</TH>
      <TD colspan="3"><INPUT size="60" type="text" name="name_foot" value="$name_foot"><br>
      商品名ヘッダ + 商品名 + 商品名フッタ合計 全角６４文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">コード ※</TH>
      <TD colspan="3"><INPUT size="60" type="text" name="code" value="$code"><BR>
      半角英数字と半角ハイフンのみ使用可能 半角30文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">金額 ※</TH>
      <TD colspan="3"><INPUT size="20" type="text" maxlength="10" name="price" value="$price">円<BR>半角１０文字以内、税抜き金額を入力して下さい。税計算後は切り捨てします。</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">特価</TH>
      <TD colspan="3"><INPUT size="20" type="text" maxlength="10" name="sale_price" value="$sale_price">円<BR>半角１０文字以内、税抜き金額を入力して下さい。税計算後は切り捨てします。</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon金額</TH>
      <TD colspan="3"><INPUT size="20" type="text" maxlength="10" name="amz_price" value="$amz_price">円<BR>半角１０文字以内、税抜き金額を入力して下さい。税計算後は切り捨てします。特別価格にする場合のみ入力して下さい。</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">サイズ</TH>
      <TD colspan="3">
<div id="size_form">
<div>
<div style="width:150px;float:left;text-align:center;">サイズ</div>
<div style="width:150px;float:left;text-align:center;">JANコード</div>
<div style="width:250px;float:left;text-align:center;">ASIN(Amazonページコード)</div>
<div style="width:150px;float:left;text-align:center;">状態</div>
</div>

WAKABA;

	//	add ookawara 2015/09/11	start
	//	他に追加項目も有りHTMLの中のため詳細省略

	//	シリアライズされている値を配列に戻す
	//	DBから取得した場合
	if ($size_list) {
		$op_size = unserialize($size_list);
	} elseif ($_POST['op_size']) {
		$op_size = $_POST['op_size'];
	}

	//	各値をそれぞれの配列に分ける
	if (count($op_size) < 1) {
		//	必ず空のフォームを一つ作るため空設定を行う
		$op_size[1]['size'] = "";
		$op_size[1]['jan']  = "";
		$op_size[1]['asin'] = "";
		$op_size[1]['type'] = "";
	}

	//	フォームを作成する。
	$i = 1;
	foreach ($op_size AS $key => $VAL) {

		//	add ohkawara 2019/11/19
		//	オリジナル商品で、親番号が有った場合
		if ($op_size[$key]['size'] == "" && $op_size[$key]['jan'] == "par" && $op_size[$key]['asin'] != "") {
			echo "      <div style=\"clear:both;\">\n";
			echo "      <input type=\"hidden\" name=\"op_size[".$i."][jan]\"  value=\"par\" />\n";
			echo "      <input type=\"hidden\" name=\"op_size[".$i."][asin]\" value=\"".$op_size[$key]['asin']."\" />\n";
			echo "      </div>\n";

			continue;
		}


		echo "      <div style=\"clear:both;\">\n";
		echo "      ".$i."：\n";
		echo "      <input type=\"text\" name=\"op_size[".$i."][size]\" value=\"".$op_size[$key]['size']."\" style=\"width:150px;\" />\n";
		echo "      <input type=\"text\" name=\"op_size[".$i."][jan]\"  value=\"".$op_size[$key]['jan']."\"  style=\"width:150px;\" />\n";
		echo "      <input type=\"text\" name=\"op_size[".$i."][asin]\" value=\"".$op_size[$key]['asin']."\" style=\"width:250px;\" />\n";

		$checked1 = "checked";
		$checked2 = "";
		$checked3 = "";
		if ($op_size[$key]['size'] == "" && $op_size[$key]['jan'] == "" && $op_size[$key]['asin'] == "") {
			$checked1 = "";
			$checked2 = "checked ";
		} elseif ($op_size[$key]['type'] == 2) {
			$checked1 = "";
			$checked2 = "checked ";
		} elseif ($op_size[$key]['type'] == 3) {
			$checked1 = "";
			$checked3 = "checked ";
		}

		echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"1\" ".$checked1."/>:販売\n";
		echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"2\" ".$checked2."/>:NEW\n";
		echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"3\" ".$checked3."/>:非表示\n";
		echo "      </div>\n";
		$i++;
	}
	$max_cnt = $i;

	//	add ookawara 2015/09/11	end

	echo <<<WAKABA
</div>
<input type="button" value="入力欄追加" onclick="AddTableRows();" /><br>
<script type="text/javascript">
var counter = {$max_cnt};
function AddTableRows(){
	var div_element = document.createElement("div");
	div_element.innerHTML  = counter+'： ';
	div_element.innerHTML += '<input type="text" name="op_size['+counter+'][size]" value="" style="width:150px;" /> ';
	div_element.innerHTML += '<input type="text" name="op_size['+counter+'][jan]" value="" style="width:150px;" /> ';
	div_element.innerHTML += '<input type="text" name="op_size['+counter+'][asin] value="" style="width:250px;" /> ';
	div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="1" />:通常 ';
	div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="2" checked />:NEW ';
	div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="3" />:非表示';
	var parent_object = document.getElementById("size_form");
	parent_object.appendChild(div_element);
	counter++;
}
</script>
Amazonに出品する商品の場合、必ずJANを入力して下さい。<br>
<span style="color:red;">JANが無いオリジナル商品</span>は「none」と入力して下さい。<br>
オリジナル商品では無い場合は、「none」と入れないで下さい。amazonの出品もしないでください。<br>
#############################################################################################################<br>
旧入力方式<br>
      <TEXTAREA rows="5" cols="30" name="options">$options</TEXTAREA><BR>
      1行に1サイズをいれて下さい。　全角１０２３文字以内<BR>
      新規にサイズを追加する場合は、"/"をサイズの前に入力してください。<BR>
      表示させないサイズの場合は、"*"を入力してください。<br>
      janコードは、サイズの後に、[ ]でくくり入力してください。<br>
      例）XL[82323398169001009]
      </TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品見出し</TH>
      <TD colspan="3"><INPUT size="60" type="text" name="headline" value="$headline"> 全角３０文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明 ※</TH>
      <TD colspan="3"><TEXTAREA rows="5" cols="50" name="caption">$caption</TEXTAREA><BR>
      全角5120文字以内　「,」と「&quot;」は使用しないで下さい。HTMLタグは利用できます。</TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明の箇条書き1-5</TH>
      <TD colspan="3">
        1：<INPUT size="60" type="text" name="bullet_point1" value="$bullet_point1"><br>
        2：<INPUT size="60" type="text" name="bullet_point2" value="$bullet_point2"><br>
        3：<INPUT size="60" type="text" name="bullet_point3" value="$bullet_point3"><br>
        4：<INPUT size="60" type="text" name="bullet_point4" value="$bullet_point4"><br>
        5：<INPUT size="60" type="text" name="bullet_point5" value="$bullet_point5"><br>
        各全角３３文字以内。HTMLタグ不可。<br>
        例：<br>
        メイン素材: ポリエステル<br>
        素材構成: インターロック ポリエステル100%<br>
        サイズレンジ:J/S、J/M、J/L、J/O、J/XO、J2XO<br>
        対象:メンズ<br>
        Mサイズ : 着丈 約73cm / 身幅 約53cm / 肩幅 約45cm / 袖丈 約21cm / 身長165cm - 175cm<br>
        カラー:ブルー
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">検索キーワード1-5</TH>
      <TD colspan="3">
        1：<INPUT size="40" type="text" name="generic_keywords1" value="$generic_keywords1"><br>
        2：<INPUT size="40" type="text" name="generic_keywords2" value="$generic_keywords2"><br>
        3：<INPUT size="40" type="text" name="generic_keywords3" value="$generic_keywords3"><br>
        4：<INPUT size="40" type="text" name="generic_keywords4" value="$generic_keywords4"><br>
        5：<INPUT size="40" type="text" name="generic_keywords5" value="$generic_keywords5"><br>
        各全角10文字以内。HTMLタグ不可。<br>
        商品名フリガナ以外で、検索キーワードとして登録するもの。<br>
        商品を検索しやすくするためのキーワード。<br>
        商品の特性や用途を表す用語を選ぶこと。<br>
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明<BR>(サイト内適用)</TH>
      <TD colspan="3"><TEXTAREA rows="5" cols="50" name="comment">$comment</TEXTAREA></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">一口メモ</TH>
      <TD colspan="3"><TEXTAREA rows="5" cols="50" name="abstract">$abstract</TEXTAREA> 全角5120文字以内</TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
	<TD colspan="4"><INPUT type="submit" value="変更">　<INPUT type="reset" onclick="document.forms['goods_form'].elements['mode'].value='変更';submit();"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	}

	echo <<<WAKABA
<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="submit" value="一覧に戻る">
<input type="hidden" name="bg_g_num" value="{$g_num}">
<INPUT type="hidden" name="action" value="list">
</FORM>

WAKABA;

}



function del() {
//  global	$PHP_SELF,$ERROR,$msg,$cate_table,$goods_table,
//  		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,
//  		$main,$goods,$mc,$sc,$lc,$g_num,$b_code,$num,$mc_r,$sc_r,$brand,$ypath,$copy_num,
//  		$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment
//  		,$rd_id,$yd_id;
//  global $CATE_DIR;		//	add ookawara 2015/06/25
//  global $ad_id;			//	add ookawara 2015/07/24
global	$PHP_SELF,$ERROR,$CATE_DIR,$GG_N,$cate_table,$goods_table,
		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file;

global $msg; 
global $mc,$sc,$lc,$ad_id; // goods_03<_POST

global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract; // @source goods_03<_POST
$main=$_POST['main'];
$goods=$_POST['goods'];
$g_num=$_POST['g_num'];
$num=$_POST['num'];
$ypath=$_POST['ypath'];
$copy_num=$_POST['copy_num'];

$comment=$_POST['comment'];
$rd_id=$_POST['rd_id'];
$yd_id=$_POST['yd_id'];
	//	add ookawara 2015/06/22
	$s_keyword = "";
	if ($_POST['s_keyword'] != "") {
		$s_keyword = $_POST['s_keyword'];
	}

	//	add ookawara 2015/09/14
	$s_soldout = "";
	if ($_POST['s_soldout'] != "") {
		$s_soldout = $_POST['s_soldout'];
	}
	$s_yahooshop = "";
	if ($_POST['s_yahooshop'] != "") {
		$s_yahooshop = $_POST['s_yahooshop'];
	}
	$s_amazonshop = "";
	if ($_POST['s_amazonshop'] != "") {
		$s_amazonshop = $_POST['s_amazonshop'];
	}

	//	add ohkawara 2017/02/08
	$s_wowmashop = "";
	if ($_POST['s_wowmashop'] != "") {
		$s_wowmashop = $_POST['s_wowmashop'];
	}

	$flag = 0;
	if (!$ERROR) {
		include ("../../cone.inc");
		$sql =	"SELECT * FROM $cate_table i, $goods_table j" .
				" WHERE i.g_num=j.g_num AND i.num='$num';";
		if (!$result = mysqli_query($conn_id,$sql)) {
			$ERROR[] = "登録商品情報を読み込みできませんでした。";
			$flag = 1;
		}
		else {
			//list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$g_num,$state,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg) = mysqli_fetch_array($result,0);				//	del ookawara 2015/06/24
			//list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$g_num,$state,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg,$amazonshop,$ad_id) = mysqli_fetch_array($result,0);		//	add ookawara 2015/06/24	//	del ookawara 2015/09/15
			//list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$state,$g_num,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg,$amazonshop,$ad_id, $size_list, $name_head, $bullet_point1, $bullet_point2, $bullet_point3, $bullet_point4, $bullet_point5, $generic_keywords1, $generic_keywords2, $generic_keywords3, $generic_keywords4, $generic_keywords5) = mysqli_fetch_array($result,0);	//	add ookawara 2015/09/15	//	del ookawara 2015/09/24
			//list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$state,$g_num,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg,$amazonshop,$ad_id, $size_list, $name_head, $bullet_point1, $bullet_point2, $bullet_point3, $bullet_point4, $bullet_point5, $generic_keywords1, $generic_keywords2, $generic_keywords3, $generic_keywords4, $generic_keywords5, $name_foot, $product_subtype, $ad_id2) = mysqli_fetch_array($result,0);	//	add ookawara 2015/09/24

			list($num,$cate1,$cate2,$cate3,$g_num,$ypath,$display,$copy_num,$state,$g_num,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$relevant_links,$ship_weight,$brand,$comment,$yahooshop,$soldout,$zaiko,$rd_id,$yd_id,$del_flg,$amazonshop,$ad_id, $size_list, $name_head, $bullet_point1, $bullet_point2, $bullet_point3, $bullet_point4, $bullet_point5, $generic_keywords1, $generic_keywords2, $generic_keywords3, $generic_keywords4, $generic_keywords5, $name_foot, $product_subtype, $ad_id2, $department_name, $regist_time, $update_time, $distribution_designation, $model_year, $seasons, $amz_price, $wd_id, $wowmashop,$pg_num,$parent_flag,$gp_option,$gp_option_name) = mysqli_fetch_array($result);	//	add ohkawara 2017/02/08
		}
	}

	if ($ERROR) { ERROR($ERROR); }

	if (!$ERROR && $flag == 0) {

	//	add ookawara 2015/06/25
	$mc_file = "$CATE_DIR/".$cate1.".dat";
	$sc_file = "$CATE_DIR/".$cate1."_".$cate2.".dat";
	$lc_file = "$CATE_DIR/".$cate1."_".$cate2."_".$cate3.".dat";
	$mc = $cate1;
	$sc = $cate2;
	$lc = $cate3;

	$C_LIST = file($c_file);
	foreach ($C_LIST AS $val) {
		list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
		if ($mc == $mc_num_) { $mc_name = $mc_name_; break; }
	}

	if ($mc && file_exists($mc_file)) {
		$SC_LIST = file($mc_file);
		foreach ($SC_LIST AS $val) {
			list($h_num_,$sc_num_,$sc_name_) = explode("<>",$val);
			if ($sc == $sc_num_) { $sc_name = $sc_name_; break; }
		}
	}

	if ($sc && file_exists($sc_file)) {
		$LC_LIST = file($sc_file);
		foreach ($LC_LIST AS $val) {
			list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
			if ($lc == $lc_num_) { $lc_name = $lc_name_; break; }
		}
	}

	if (file_exists($b_file)) {
		$B_LIST = file($b_file);
		foreach ($B_LIST AS $val) {
			list($b_num_,$b_name_,$del_) = explode("<>",$val);
			if ($brand == $b_num_) { $b_name = $b_name_; break; }
		}
	}

	if (file_exists($list_file)) {
		$Y_LIST = file($list_file);
		foreach ($Y_LIST AS $val) {
			list($y_name_,$ypath_,$del_) = explode("<>",$val);
			if ($ypath == $y_name_) { $y_name = $y_name_; break; }
		}
	}

	if ($yahooshop == 1) { $y_checked = "○"; } else { $y_checked = "×"; }
	if ($amazonshop == 1) { $a_checked = "○"; } else { $a_checked = "×"; }		//	add ookawara 2015/06/24
	if ($soldout == 1) { $s_checked = "○"; } else { $s_checked = "×"; }
	if ($sale_price > 0) { $sale_price = "$sale_price 円"; } else { $sale_price = "---"; }
	if ($wowmashop == 1) { $w_checked = "○"; } else { $w_checked = "×"; }		//	add ohkawara 2017/02/08
	if ($parent_flag == 1) { $pt_checked = "○"; } else { $pt_checked = "×"; }		//	add thao 2023/05/17
	$options = nl2br($options);
	$caption = nl2br($caption);
	$abstract = nl2br($abstract);
	$comment = nl2br($comment);

	$rd_name = rd_val($rd_id);
	$yd_name = yd_val($yd_id);
	$ad_name = ad_val($ad_id);	//	add ookawara 2015/07/24
	if ($ad_id2) {						//	add ookawara 2015/09/24
		$ad_name2 = ad_val($ad_id2);	//	add ookawara 2015/09/24
	}									//	add ookawara 2015/09/24
	//	add ookawara 2015/09/17
	$zaiko_checked = "--";
	if ($zaiko == 1) {
		$zaiko_checked = "○";
	}
	$wd_name = wd_val($wd_id);	//	add ohkawara 2017/02/08


		echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="g_num" value="$g_num">
<INPUT type="hidden" name="num" value="$num">
<INPUT type="hidden" name="mode" value="delete">
<INPUT type="hidden" name="copy_num" value="$copy_num">
<INPUT type="hidden" name="s_keyword" value="$s_keyword">
<INPUT type="hidden" name="s_soldout" value="$s_soldout">
<INPUT type="hidden" name="s_yahooshop" value="$s_yahooshop">
<INPUT type="hidden" name="s_amazonshop" value="$s_amazonshop">
<INPUT type="hidden" name="s_wowmashop" value="$s_wowmashop">
<INPUT type="hidden" name="pg_num" value="$pg_num">
<INPUT type="hidden" name="action" value="list">
以下の商品を完全に削除してもよければ”完全削除”ボタンを<BR>
このカテゴリーから削除する場合は”カテゴリーから削除”ボタンを押して下さい。
<TABLE border="0" cellpadding="1" cellspacing="1" bgcolor="#666666" style="font-size : 12px;" width="800">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">商品管理番号</TH>
      <TD colspan="3">
      <a href="/goods/g{$g_num}/" target="_blank">{$g_num}</a>
      </TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD colspan="3">$mc_name // $sc_name // $lc_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">ブランド名</TH>
      <TD colspan="3">$b_name</TD>
    </TR>
<!--
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Ypath</TH>
      <TD colspan="3">$y_name</TD>
    </TR>
-->
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">楽天ディレクトリー</TH>
      <TD colspan="3">$rd_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">YAHOOディレクトリー</TH>
      <TD colspan="3">$yd_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード</TH>
      <TD colspan="3">$ad_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード2</TH>
      <TD colspan="3">$ad_name2</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Wowmaディレクトリー</TH>
      <TD colspan="3">$wd_name</TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Yshop</TH>
      <TD width="220">$y_checked</TD>
      <TH bgcolor="#cccccc" width="80">Soldout</TH>
      <TD width="220">$s_checked</TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Ashop</TH>
      <TD>$a_checked</TD>
      <TH bgcolor="#cccccc" width="80">在庫一品限り</TH>
      <TD width="220">$zaiko_checked</TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TH bgcolor="#cccccc">Wshop</TH>
      <TD colspan="3">$w_checked</TD>
    </TR>
	<TR bgcolor="#ffffff">
	<TH bgcolor="#cccccc">商品グループ親番号</TH>
	<TD>$pg_num</TD>
	<TH bgcolor="#cccccc" width="50">親商品</TH>
	<TD width="150" align="center">$pt_checked</TD>
  	</TR>
	<TR bgcolor="#ffffff">
	<TH bgcolor="#cccccc">商品グループ選択肢</TH>
	<TD colspan="3">$GG_N[$gp_option]</TD>
	</TR>
	<TR bgcolor="#ffffff">
	<TH bgcolor="#cccccc">商品グループ選択肢名</TH>
	<TD colspan="3">$gp_option_name</TD>
	</TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名ヘッダ</TH>
      <TD colspan="3">$name_head</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名</TH>
      <TD colspan="3">$name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品名フッタ</TH>
      <TD colspan="3">$name_foot</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">コード</TH>
      <TD colspan="3">$code</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">金額</TH>
      <TD colspan="3">$price 円</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">特価</TH>
      <TD colspan="3">$sale_price</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">サイズ</TH>
      <TD colspan="3">
WAKABA;

	$op_size = unserialize($size_list);

	$i = 1;
	foreach ($op_size AS $key => $VAL) {

		//	add	ohkawara	2019/11/19
		//	オリジナル商品で、親情報の場合スキップする
		if ($op_size[$key]['size'] == "" && $op_size[$key]['jan'] == "par" && $op_size[$key]['asin'] != "") {
			continue;
		}


		echo "      <div style=\"clear:both;\">\n";
		echo "      ".$i."：\n";
		echo "      ".$op_size[$key]['size']."\n";
		echo "      [".$op_size[$key]['jan']."]\n";
		echo "      (".$op_size[$key]['asin'].")\n";

		if ($op_size[$key]['size'] == "" && $op_size[$key]['jan'] == "" && $op_size[$key]['asin'] == "") {
			echo "      NEW\n";
		} elseif ($op_size[$key]['type'] == 2) {
			echo "      NEW\n";
		} elseif ($op_size[$key]['type'] == 3) {
			echo "      非表示\n";
		} else {
			echo "      販売\n";
		}
		echo "      </div>\n";
		$i++;
	}

	echo <<<WAKABA
       ##########################################################<br>
       旧サイズ設定<br>
       $options
      </TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品見出し</TH>
      <TD colspan="3">$headline</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明</TH>
      <TD colspan="3">$caption</TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明の箇条書き1-5</TH>
      <TD colspan="3">
      1:$bullet_point1<br>
      2:$bullet_point2<br>
      3:$bullet_point3<br>
      4:$bullet_point4<br>
      5:$bullet_point5<br>
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">検索キーワード1-5</TH>
      <TD colspan="3">
      1:$generic_keywords1<br>
      2:$generic_keywords2<br>
      3:$generic_keywords3<br>
      4:$generic_keywords4<br>
      5:$generic_keywords5<br>
      </TD>
    </TR>

    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">商品説明<BR>(サイト内適用)</TH>
      <TD colspan="3">$comment</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">一口メモ</TH>
      <TD colspan="3">$abstract</TD>
    </TR>
    <TR bgcolor="#ffffff" align="center">
      <TD colspan="4"><INPUT type="submit" name="mode" value="完全削除">　<INPUT type="submit" name="mode" value="カテゴリーから削除"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	}

	echo <<<WAKABA
<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="submit" value="一覧に戻る">
<input type="hidden" name="bg_g_num" value="{$g_num}">
<INPUT type="hidden" name="action" value="list">
</FORM>

WAKABA;

}



function hyouji() {
// global	$ERROR,$cate_table,$renew_file,
// 		$num;
global	$ERROR,$cate_table,$renew_file,$goods_table;
$num=$_POST['num'];
	include ("../../cone.inc");

	// $sql = " SELECT state, g_num FROM $cate_table WHERE num='$num';";
	$sql = "SELECT c.state, c.display, c.g_num, j.pg_num FROM $cate_table c INNER JOIN $goods_table j ON c.g_num=j.g_num AND c.num='$num';";	//add thao 2023/04/14 
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$state = $list['state'];
		$g_num = $list['g_num'];
		$pg_num = $list["pg_num"];
	}
	if ($state == 1) { $state = 1; }
	elseif ($state == 3) { $state = 3; }
	else { $state = 2; }
	//add thao 2023/04/14 start{
	//子商品非表示したい場合親番号チェック
	$sql = "SELECT c.display, c.g_num, j.pg_num FROM $goods_table j INNER JOIN $cate_table c ON  j.g_num='$g_num' AND c.g_num=j.g_num AND del_flg='0' ;";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list_check = mysqli_fetch_array($result);
		if($list_check["pg_num"]){
			$count = 0;
			$display_check = 0;
			$sql = "SELECT c.display, c.g_num, j.pg_num FROM $goods_table j INNER JOIN $cate_table c ON  j.g_num='$pg_num' AND c.g_num=j.g_num AND del_flg='0' ;";
			if ($result = mysqli_query($conn_id,$sql)) {
				while($pg_num_check = mysqli_fetch_array($result)){
					$count++;
					if($pg_num_check["g_num"] && ($pg_num_check["display"] != 1)){
						$display_check++;
					}
				}
			}
			if($display_check == $count){$ERROR[] = "商品グループ親の商品が非表示のため、表示に切り替えができません。親の商品を、表示に切り替えるか、商品グループ親番号を削除してから表示にして下さい。（親の管理商品番号： ".$pg_num.")";}
		}
	}
	//add thao 2023/04/14 end

	if (!$ERROR && $g_num) {
		$sql = "UPDATE $cate_table SET" .
				" display='1'," .
				" state='$state'" .
				" WHERE g_num='$g_num';";
				// " WHERE num='$num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを表示に変更できませんでした。"; }
	} elseif(!$ERROR) {
		$ERROR[] = "表示変更する商品データーが確認できませんでした。";
	}

	//mysqli_close($conn_id);

	//	更新したファイルを記入
	if ($num) {
		$OUT = fopen($renew_file,"a");
		fwrite($OUT,"$num\n");
		fclose($OUT);
		@chmod($up_file,0666);
	}

}



function hihyouji() {
// global	$ERROR,$cate_table,$renew_file,
// 		$num;
global	$ERROR,$CATE_DIR,$cate_table,$renew_file,$goods_table,$c_file,$b_file,$list_file;
$num=$_POST['num'];
	include ("../../cone.inc");
    
	// $sql = " SELECT state, g_num FROM $cate_table WHERE num='$num';";
	$sql = "SELECT c.state, c.g_num, j.pg_num FROM $cate_table c INNER JOIN $goods_table j ON c.g_num=j.g_num AND c.num='$num';";	//add thao 2023/04/14 
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$state = $list['state'];
		$g_num = $list['g_num'];
		$pg_num = $list["pg_num"];
	}

	if ($state == 1) { $state = 1; }
	elseif ($state == 3) { $state = 3; }
	else { $state = 2; }
	//add thao 2023/04/14 start
    if($g_num) {
		$g_num_check = [];
		$display_1_count = 0; //　表示商品数チェック
		$sql = "SELECT c.display, c.g_num, j.pg_num,c.num,c.cate1,c.cate2,c.cate3 FROM $goods_table j INNER JOIN $cate_table c ON  j.pg_num='$g_num' AND c.g_num=j.g_num AND del_flg='0' ;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list_check = mysqli_fetch_array($result)) {
				$cate1 = $list_check["cate1"];
				$cate2 = $list_check["cate2"];
				$cate3 = $list_check["cate3"];
				//カテゴリーの名前取り出し start
				$mc_file = "$CATE_DIR/".$cate1.".dat";
				$sc_file = "$CATE_DIR/".$cate1."_".$cate2.".dat";
				$lc_file = "$CATE_DIR/".$cate1."_".$cate2."_".$cate3.".dat";
				$mc = $cate1;
				$sc = $cate2;
				$lc = $cate3;

				$C_LIST = file($c_file);
				foreach ($C_LIST AS $val) {
					list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
					if ($mc == $mc_num_) { $mc_name = $mc_name_; break; }
				}

				if ($mc && file_exists($mc_file)) {
					$SC_LIST = file($mc_file);
					foreach ($SC_LIST AS $val) {
						list($h_num_,$sc_num_,$sc_name_) = explode("<>",$val);
						if ($sc == $sc_num_) { $sc_name = $sc_name_; break; }
					}
				}

				if ($sc && file_exists($sc_file)) {
					$LC_LIST = file($sc_file);
					foreach ($LC_LIST AS $val) {
						list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
						if ($lc == $lc_num_) { $lc_name = $lc_name_; break; }
					}
				}

				if (file_exists($b_file)) {
					$B_LIST = file($b_file);
					foreach ($B_LIST AS $val) {
						list($b_num_,$b_name_,$del_) = explode("<>",$val);
						if ($brand == $b_num_) { $b_name = $b_name_; break; }
					}
				}

				if (file_exists($list_file)) {
					$Y_LIST = file($list_file);
					foreach ($Y_LIST AS $val) {
						list($y_name_,$ypath_,$del_) = explode("<>",$val);
						if ($ypath == $y_name_) { $y_name = $y_name_; break; }
					}
				}
				//カテゴリーの名前取り出し end
				if(($list_check["pg_num"] != $list_check['g_num']) && ($list_check["display"] == 1) && ($list_check["display"]!= 2)){
					$g_num_check[]=$mc_name." > ".$sc_name." > ".$lc_name."の".$list_check['g_num'];
					$display_1_count++;
				}
			}
			$g_num_check = array_unique($g_num_check);
			if(($display_1_count > 0)){
				foreach($g_num_check as $value){
					$child_num.=$value."<br>";
				}
				$child_num = rtrim($child_num,"<br>");
				$ERROR[] = "商品グループ親の商品のため、非表示にできません。子の管理商品番号：<br>".$child_num;
			}
		}
	}
	//add thao 2023/04/14 end
	if (!$ERROR && $g_num) {
		$sql = "UPDATE $cate_table SET" .
				" display='0'," .
				" state='$state'" .
				" WHERE g_num='$g_num';";
				// " WHERE num='$num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを非表示に変更できませんでした。"; }
	} elseif(!$ERROR) {
		$ERROR[] = "表示変更する商品データーが確認できませんでした。";
	}
	//mysqli_close($conn_id);

	//	更新したファイルを記入
	if ($num) {
		$OUT = fopen($renew_file,"a");
		fwrite($OUT,"$num\n");
		fclose($OUT);
		@chmod($up_file,0666);
	}

}



function renew() {
// global	$ERROR,$msg,$cate_table,$goods_table,$IMAGEF,$IMAGEB,
// 		$brand,$ypath,$b_code,$renew_file,
// 		$num,$g_num,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,
// 		$comment,$rd_id,$yd_id;
// global $ad_id;		//	add ookawara 2015/07/24
// global $ad_id2;		//	add ookawara 2015/09/24
global	$ERROR,$IMAGEF,$IMAGEB,$cate_table,$goods_table,$renew_file;
global $msg; 
global $ad_id;//@source goods_3<_POST
global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract;// @source goods_3<$_POST
global	$CATE_DIR,$c_file,$b_file,$list_file;

$ypath=$_POST['ypath'];
$b_code=$_POST['b_code'];
$g_num=$_POST['g_num'];
$num=$_POST['num'];
$comment=$_POST['comment'];
$rd_id=$_POST['rd_id'];
$yd_id=$_POST['yd_id'];
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];

$ad_id2=$_POST['ad_id2'];

	$pg_num = $_POST['pg_num'];							//	add thao 2023/04/11
	$pg_num = mb_convert_kana($pg_num,"asKV","UTF-8");
	$pg_num = trim($pg_num);								//	add thao 2023/04/11
	
	$gp_option = $_POST['gp_option'];							//	add thao 2023/05/09

	$gp_option_name = $_POST['gp_option_name'];							//	add thao 2023/04/11
	$gp_option_name = mb_convert_kana($gp_option_name,"asKV","UTF-8");
	$gp_option_name = trim($gp_option_name);								//	add thao 2023/04/11

	$parent_flag = $_POST['parent_flag'];							//	add thao 2023/05/09

	$name_head = $_POST['name_head'];							//	add ookawara 2015/09/14
#	$name_head = mb_convert_kana($name_head,"asKV","EUC-JP");	//	add ookawara 2015/09/14
	$name_head = mb_convert_kana($name_head,"asKV","UTF-8");
	$name_head = preg_replace("/,/", "、", $name_head);			//	add ookawara 2015/09/14
	$name_head = trim($name_head);								//	add ookawara 2015/09/14

	$name_foot = $_POST['name_foot'];							//	add ookawara 2015/09/24
#	$name_foot = mb_convert_kana($name_foot,"asKV","EUC-JP");	//	add ookawara 2015/09/24
	$name_foot = mb_convert_kana($name_foot,"asKV","UTF-8");
	$name_foot = preg_replace("/,/", "、", $name_foot);			//	add ookawara 2015/09/24
	$name_foot = trim($name_foot);								//	add ookawara 2015/09/24

#	$name = mb_convert_kana($name,"asKV","EUC-JP");
	$name = mb_convert_kana($name,"asKV","UTF-8");
	$name = preg_replace("/,/", "、", $name);	//	add ookawara 2014/01/03
	$name = ltrim($name);
	$name = rtrim($name);
#	$code = mb_convert_kana($code,"as","EUC-JP");
	$code = mb_convert_kana($code,"as","UTF-8");
	$code = strtoupper($code);
	$code = ltrim($code);
	$code = rtrim($code);
#	$price = mb_convert_kana($price,"n","EUC-JP");
	$price = mb_convert_kana($price,"n","UTF-8");
	$price = ltrim($price);
	$price = rtrim($price);
#	$sale_price = mb_convert_kana($sale_price,"n","EUC-JP");
	$sale_price = mb_convert_kana($sale_price,"n","UTF-8");
	$sale_price = ltrim($sale_price);
	$sale_price = rtrim($sale_price);
#	$options = mb_convert_kana($options,"asKV","EUC-JP");
	$options = mb_convert_kana($options,"asKV","UTF-8");
	$options = ltrim($options);
	$options = rtrim($options);
#	$headline = mb_convert_kana($headline,"asKV","EUC-JP");
	$headline = mb_convert_kana($headline,"asKV","UTF-8");
	$headline = ltrim($headline);
	$headline = rtrim($headline);
#	$caption = mb_convert_kana($caption,"asKV","EUC-JP");
	$caption = mb_convert_kana($caption,"asKV","UTF-8");
	$caption = stripslashes($caption);
	$caption = ltrim($caption);
	$caption = rtrim($caption);
#	$abstract = mb_convert_kana($abstract,"asKV","EUC-JP");
	$abstract = mb_convert_kana($abstract,"asKV","UTF-8");
	$abstract = ltrim($abstract);
	$abstract = rtrim($abstract);
#	$comment = mb_convert_kana($comment,"asKV","EUC-JP");
	$comment = mb_convert_kana($comment,"asKV","UTF-8");
	$comment = ltrim($comment);
	$comment = rtrim($comment);

	$ad_id = trim($ad_id);			//	add ookawara 2015/09/16
	$ad_id2 = trim($ad_id2);		//	add ookawara 2015/09/24

	//	add ookawara 2015/09/14	start
	$op_size = $_POST['op_size'];

	$bullet_point1 = $_POST['bullet_point1'];
#	$bullet_point1 = mb_convert_kana($bullet_point1,"asKV","EUC-JP");
	$bullet_point1 = mb_convert_kana($bullet_point1,"asKV","UTF-8");
	$bullet_point1 = preg_replace("/,/", "、", $bullet_point1);
	$bullet_point1 = trim($bullet_point1);

	$bullet_point2 = $_POST['bullet_point2'];
#	$bullet_point2 = mb_convert_kana($bullet_point2,"asKV","EUC-JP");
	$bullet_point2 = mb_convert_kana($bullet_point2,"asKV","UTF-8");
	$bullet_point2 = preg_replace("/,/", "、", $bullet_point2);
	$bullet_point2 = trim($bullet_point2);

	$bullet_point3 = $_POST['bullet_point3'];
#	$bullet_point3 = mb_convert_kana($bullet_point3,"asKV","EUC-JP");
	$bullet_point3 = mb_convert_kana($bullet_point3,"asKV","UTF-8");
	$bullet_point3 = preg_replace("/,/", "、", $bullet_point3);
	$bullet_point3 = trim($bullet_point3);

	$bullet_point4 = $_POST['bullet_point4'];
#	$bullet_point4 = mb_convert_kana($bullet_point4,"asKV","EUC-JP");
	$bullet_point4 = mb_convert_kana($bullet_point4,"asKV","UTF-8");
	$bullet_point4 = preg_replace("/,/", "、", $bullet_point4);
	$bullet_point4 = trim($bullet_point4);

	$bullet_point5 = $_POST['bullet_point5'];
#	$bullet_point5 = mb_convert_kana($bullet_point5,"asKV","EUC-JP");
	$bullet_point5 = mb_convert_kana($bullet_point5,"asKV","UTF-8");
	$bullet_point5 = preg_replace("/,/", "、", $bullet_point5);
	$bullet_point5 = trim($bullet_point5);

	$generic_keywords1 = $_POST['generic_keywords1'];
#	$generic_keywords1 = mb_convert_kana($generic_keywords1,"asKV","EUC-JP");
	$generic_keywords1 = mb_convert_kana($generic_keywords1,"asKV","UTF-8");
	$generic_keywords1 = preg_replace("/,/", "、", $generic_keywords1);
	$generic_keywords1 = trim($generic_keywords1);

	$generic_keywords2 = $_POST['generic_keywords2'];
#	$generic_keywords2 = mb_convert_kana($generic_keywords2,"asKV","EUC-JP");
	$generic_keywords2 = mb_convert_kana($generic_keywords2,"asKV","UTF-8");
	$generic_keywords2 = preg_replace("/,/", "、", $generic_keywords2);
	$generic_keywords2 = trim($generic_keywords2);

	$generic_keywords3 = $_POST['generic_keywords3'];
#	$generic_keywords3 = mb_convert_kana($generic_keywords3,"asKV","EUC-JP");
	$generic_keywords3 = mb_convert_kana($generic_keywords3,"asKV","UTF-8");
	$generic_keywords3 = preg_replace("/,/", "、", $generic_keywords3);
	$generic_keywords3 = trim($generic_keywords3);

	$generic_keywords4 = $_POST['generic_keywords4'];
#	$generic_keywords4 = mb_convert_kana($generic_keywords4,"asKV","EUC-JP");
	$generic_keywords4 = mb_convert_kana($generic_keywords4,"asKV","UTF-8");
	$generic_keywords4 = preg_replace("/,/", "、", $generic_keywords4);
	$generic_keywords4 = trim($generic_keywords4);

	$generic_keywords5 = $_POST['generic_keywords5'];
#	$generic_keywords5 = mb_convert_kana($generic_keywords5,"asKV","EUC-JP");
	$generic_keywords5 = mb_convert_kana($generic_keywords5,"asKV","UTF-8");
	$generic_keywords5 = preg_replace("/,/", "、", $generic_keywords5);
	$generic_keywords5 = trim($generic_keywords5);
	//	add ookawara 2015/09/14	end

	$zaiko = $_POST['zaiko'];	//	add ookawara 2015/09/17

	$product_subtype = trim($_POST['product_subtype']);	//	add ookawara 2015/09/24

	$department_name = $_POST['department_name'];		//	add ookawara 2015/09/27

	//	add ookawara 2015/10/01
	$distribution_designation = $_POST['distribution_designation'];
	if ($distribution_designation == "") {
		$distribution_designation = 0;
	}
	$model_year = $_POST['model_year'];
	$seasons = $_POST['seasons'];

	//	add ookawara 2015/10/08
	$amz_price = $_POST['amz_price'];
#	$amz_price = mb_convert_kana($amz_price,"n","EUC-JP");
	$amz_price = mb_convert_kana($amz_price,"n","UTF-8");
	$amz_price = trim($amz_price);

	//	add ohkawara 2017/02/08
	$wd_id = trim($_POST['wd_id']);
	$wowmashop = $_POST['wowmashop'];
	if ($wowmashop == "") {
		$wowmashop = 0;
	}

	//if (!$name_head) { $ERROR[] = "商品名ヘッダが入力されておりません。"; }	//	add ookawara 2015/09/14	必須ではないためコメント
	if (!$name) { $ERROR[] = "商品名が入力されておりません。"; }
	//if (!$name_foot) { $ERROR[] = "商品名フッタが入力されておりません。"; }	//	add ookawara 2015/09/24	必須ではないためコメント
	//$name_len = strlen($name);				//	del ookawara 2015/09/14
	//$name_len = strlen($name.$name_head);		//	add ookawara 2015/09/14	//	del ookawara 2015/09/24
	$name_len = strlen($name.$name_head.$name_foot);		//	add ookawara 2015/09/24
	$name_len_sjis = strlen(mb_convert_encoding($name.$name_head.$name_foot,'SJIS','UTF8'));		//	add ookawara 2015/09/24
	if ($name && $name_len_sjis > 128) { $ERROR[] = "商品名が文字数越えてます。"; }	//	chage 150->128 ohkawara 2017/02/16
	if (!$code) { $ERROR[] = "コードが入力されておりません。"; }
##	if ($code && ereg("[^0-9A-Z\-]",$code)) { $ERROR[] = "コードが不正です。"; }
	if ($code && preg_match("/[^0-9A-Z\-]/",$code)) { $ERROR[] = "コードが不正です。"; }
	$code_len = strlen($code);
	if ($code && $code_len > 30) { $ERROR[] = "コードが文字数越えてます。"; }
	if (!$price) { $ERROR[] = "金額が入力されておりません。"; }
##	if ($price && ereg("[^0-9]",$price)) { $ERROR[] = "金額が不正です。"; }
	if ($price && preg_match("/[^0-9]/",$price)) { $ERROR[] = "金額が不正です。"; }
	if ($price != "" && $price <= 10) { $ERROR[] = "10円以下の金額の設定はできません。"; }
##	if ($sale_price && ereg("[^0-9]",$sale_price)) { $ERROR[] = "特価が不正です。"; }
	if ($sale_price && preg_match("/[^0-9]/",$sale_price)) { $ERROR[] = "特価が不正です。"; }
	if ($sale_price != "" && $sale_price <= 10) { $ERROR[] = "10円以下の特価の設定はできません。"; }
	//if ($price && $sale_price && $price < $sale_price) { $ERROR[] = "金額が特価の方が高いです。"; }
	//if (!$caption) { $ERROR[] = "商品説明が入力されておりません。"; }
##	if ($caption && ereg("[,\"]",$caption)) { $ERROR[] = "商品説明では、「,」と「\"」は使用できません。"; }
	if ($caption && preg_match("/[,\"]/",$caption)) { $ERROR[] = "商品説明では、「,」と「\"」は使用できません。"; }
	$headline_len = strlen($headline);
	if ($headline && $headline_len > 60) { $ERROR[] = "商品見出しが文字数越えてます。"; }

	//	add ookawara 2015/10/08
	if ($amz_price > 0) {
		//if (!$amz_price) { $ERROR[] = "Amazon金額が入力されておりません。"; }
##		if ($amz_price && ereg("[^0-9]",$amz_price)) { $ERROR[] = "Amazon金額が不正です。"; }
		if ($amz_price && preg_match("/[^0-9]/",$amz_price)) { $ERROR[] = "Amazon金額が不正です。"; }
		if ($amz_price != "" && $amz_price <= 10) { $ERROR[] = "10円以下のAmazon金額の設定はできません。"; }
	}
    if ($pg_num && preg_match("/[^0-9]/",$pg_num)) { $ERROR[] = "商品グループ親番号が不正です。"; }  //	add thao 2023/04/11

	if($pg_num == $g_num){
		$ERROR[] = "商品グループ親番号には自分を指定できません。";
	}
	include ("../../cone.inc");
    //	add thao 2023/04/11
	//商品グループ親となっている商品で、商品グループ親番号を登録した場合
	$sql = "SELECT pg_num,g_num FROM $goods_table WHERE g_num='$g_num' and del_flg='0';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		if($pg_num != $list["g_num"] && $pg_num && !preg_match("/[^0-9]/",$pg_num)){
			$g_num_check = "";
			$count = 0;
			$sql = "SELECT g_num FROM $goods_table WHERE pg_num='$g_num' and del_flg='0';";
			if ($result = mysqli_query($conn_id,$sql)) {
				while ($list = mysqli_fetch_array($result)) {
					if($list['g_num'] != $g_num){$count++;$g_num_check.=$list['g_num'].",";}
				}
				if($count>=1){
					$g_num_check=rtrim($g_num_check,",");
					$ERROR[] = "商品グループ親の商品のため、商品グループ親番号を登録する事はできません。（子の管理商品番号: ".$g_num_check.")";
				}
			}
		}
	}
	//親商品が非表示されている場合
	$sql = "SELECT * FROM $cate_table WHERE g_num='$g_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
			$display_check="";
			$display_check_2="";
			$g_num_check=[];
			while($list_check = mysqli_fetch_array($result)){
				if($list_check["display"] == 1){
					$display_check ="show";
				}
				if(($list_check["num"]==$num) && ($list_check["display"]==0)){
                    $display_check_2="hidden";
				}
				$cate1 = $list_check["cate1"];
				$cate2 = $list_check["cate2"];
				$cate3 = $list_check["cate3"];
				//カテゴリーの名前取り出し start
				$mc_file = "$CATE_DIR/".$cate1.".dat";
				$sc_file = "$CATE_DIR/".$cate1."_".$cate2.".dat";
				$lc_file = "$CATE_DIR/".$cate1."_".$cate2."_".$cate3.".dat";
				$mc = $cate1;
				$sc = $cate2;
				$lc = $cate3;

				$C_LIST = file($c_file);
				foreach ($C_LIST AS $val) {
					list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
					if ($mc == $mc_num_) { $mc_name = $mc_name_; break; }
				}

				if ($mc && file_exists($mc_file)) {
					$SC_LIST = file($mc_file);
					foreach ($SC_LIST AS $val) {
						list($h_num_,$sc_num_,$sc_name_) = explode("<>",$val);
						if ($sc == $sc_num_) { $sc_name = $sc_name_; break; }
					}
				}

				if ($sc && file_exists($sc_file)) {
					$LC_LIST = file($sc_file);
					foreach ($LC_LIST AS $val) {
						list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
						if ($lc == $lc_num_) { $lc_name = $lc_name_; break; }
					}
				}

				if (file_exists($b_file)) {
					$B_LIST = file($b_file);
					foreach ($B_LIST AS $val) {
						list($b_num_,$b_name_,$del_) = explode("<>",$val);
						if ($brand == $b_num_) { $b_name = $b_name_; break; }
					}
				}

				if (file_exists($list_file)) {
					$Y_LIST = file($list_file);
					foreach ($Y_LIST AS $val) {
						list($y_name_,$ypath_,$del_) = explode("<>",$val);
						if ($ypath == $y_name_) { $y_name = $y_name_; break; }
					}
				}
				if(($list_check["num"]!=$num) && ($list_check["display"]==1) && ($list_check["state"]!=3)){
					$g_num_check[]=$mc_name." > ".$sc_name." > ".$lc_name."の".$list_check['g_num'];
				}
			
			}
			$g_num_check = array_unique($g_num_check);
			//カテゴリーの名前取り出し end
			if ($pg_num && !preg_match("/[^0-9]/",$pg_num) && ($display_check == "show")){
				$sql = "SELECT c.display, c.g_num, j.pg_num FROM $goods_table j INNER JOIN $cate_table c ON  j.g_num='$pg_num' AND c.g_num=j.g_num AND del_flg='0';";
				if ($result = mysqli_query($conn_id,$sql)) {
					$display_check_3="";
					while($list = mysqli_fetch_array($result)){
						if($list["g_num"]){
							if($list["display"] == 1) { $display_check_3="show";break;}
							else{$display_check_3="hidden";};
						}
					}
					if(($display_check_3=="hidden") && ($display_check_2=="hidden")){
						foreach($g_num_check as $value){
							$child_num.=$value."<br>";
						}
						$child_num = rtrim($child_num,"<br>");
						$ERROR[] = "商品グループ親番号の商品が非表示となっていますが、他のカテゴリーのこの商品が表示中のため登録できません。他のカテゴリー：<br>".$child_num;
					}
					if(($display_check_3=="hidden") && ($display_check_2!="hidden")){
						$ERROR[] = "商品グループ親番号の商品が非表示となっています。";
					}
				}
		
			}
		}
	//add thao 2023/05/18
	//商品グループ親番号・親商品をチェック
	if($pg_num && !preg_match("/[^0-9]/",$pg_num) && ($parent_flag == 1)){
		$ERROR[] = "商品グループ親番号と親番号「親商品」両方の値を設定することは出来ません。";
	}
	$sql = "SELECT * FROM $goods_table WHERE pg_num='$g_num' and del_flg='0';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		if ($list["g_num"] && !$parent_flag) {$ERROR[] = "商品グループの親商品として登録されているため、チェックを外すことができません。チェックを外すには、子供の商品の詳細画面から商品グループ親番号を削除してください。";}
			//商品グループ選択肢をチェック
		if($gp_option && !$pg_num && !$parent_flag && !$list["g_num"]) {$ERROR[] = "商品グループ親番号を入力するか、商品グループ選択肢を'-'にして下さい。";}
		if($gp_option_name && !$pg_num && !$parent_flag && !$list["g_num"]) {$ERROR[] = "商品グループ親番号を入力するか、商品グループ選択肢名を空にして下さい。";}
		
	}
	//商品グループ親商品存在をチェック
	if(($pg_num && !$parent_flag) || (!$pg_num && $parent_flag)){
		if ($pg_num && !preg_match("/[^0-9]/",$pg_num)){
			//商品グループ親商品存在をチェック
		   $sql = "SELECT * FROM $goods_table WHERE g_num='$pg_num' and del_flg='0';";
		   if ($result = mysqli_query($conn_id,$sql)) {
			   $list = mysqli_fetch_array($result);
			   if($parent_flag != 1){
				   if (!$list["g_num"]) {$ERROR[] = "商品グループ親番号の商品が登録されておりません。";}
				   if($list["pg_num"] && $list["g_num"] && ($list["g_num"]!= $list["pg_num"])) {$ERROR[] = "商品グループ親番号の商品が子の商品です。";}
				   if(!$gp_option && $list["g_num"]){$ERROR[] = "商品グループ選択肢が選択されておりません。";} //add thao 2023/05/09 
				   if(!$gp_option_name && $list["g_num"]){$ERROR[] = "商品グループ選択肢名が入力されておりません。";}
				}
		    }
	   	}
		   if(!$pg_num && ($parent_flag==1)){
			if(!$gp_option){$ERROR[] = "商品グループ選択肢を選択して下さい。";} //add thao 2023/05/09 
			if(!$gp_option_name){$ERROR[] = "商品グループ選択肢名を入力して下さい。";}
		}
	}

	//add thao 2023/04/13 end

	//	add ookawara 2015/09/14	start
	$size_cnt = 1;
	if ($op_size) {
		$CK_OP = array();		//	add ookawara 2015/09/24
		$_POST['op_size'] = array();
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['size'] == "" && $VAL['jan'] == "" && $VAL['asin'] == "") {
				continue;
			//} elseif (($VAL['jan'] != "" || $VAL['asin'] != "") && $VAL['size'] == "") {							//	del ohkawara 2019/11/19
			} elseif ($VAL['jan'] != "par" && ($VAL['jan'] != "" || $VAL['asin'] != "") && $VAL['size'] == "") {	//	add ohkawara 2019/11/19
				$ERROR[] = $size_cnt."番目のサイズ情報が記載されておりません。";
			}
			$ck_size = $VAL['size'];		//	add ookawara 2015/09/24

			//	add ookawara 2015/09/24
			if ($CK_OP[$ck_size]) {
				$ERROR[] = $size_cnt."番目のサイズ情報が重複しております。";
			}

			//	add ohkawara 2019/11/19
			//	オリジナル商品の親情報の場合
			if ($VAL['jan'] == "par" && $VAL['asin'] != "") {
				$_POST['op_size'][0]['jan']  = $op_size[$key]['jan'];
				$_POST['op_size'][0]['asin'] = $op_size[$key]['asin'];

				continue;
			}


			$_POST['op_size'][$size_cnt]['size'] = $op_size[$key]['size'];
			$_POST['op_size'][$size_cnt]['jan']  = $op_size[$key]['jan'];
			$_POST['op_size'][$size_cnt]['asin'] = $op_size[$key]['asin'];
			$_POST['op_size'][$size_cnt]['type'] = $op_size[$key]['type'];

			$CK_OP[$ck_size] = $ck_size;		//	add ookawara 2015/09/24

			$size_cnt++;
		}
	}

	if ($size_cnt == 1) {
		$ERROR[] = "サイズ情報が登録されておりません。";
	}

	//	以下必須では無い為コメント
	//if (!$bullet_point1) { $ERROR[] = "商品説明の箇条書き1が入力されておりません。"; }
	//if (!$bullet_point2) { $ERROR[] = "商品説明の箇条書き2が入力されておりません。"; }
	//if (!$bullet_point3) { $ERROR[] = "商品説明の箇条書き3が入力されておりません。"; }
	//if (!$bullet_point4) { $ERROR[] = "商品説明の箇条書き4が入力されておりません。"; }
	//if (!$bullet_point5) { $ERROR[] = "商品説明の箇条書き5が入力されておりません。"; }


	//if (!$generic_keywords1) { $ERROR[] = "検索キーワード1が入力されておりません。"; }
	//if (!$generic_keywords2) { $ERROR[] = "検索キーワード2が入力されておりません。"; }
	//if (!$generic_keywords3) { $ERROR[] = "検索キーワード3が入力されておりません。"; }
	//if (!$generic_keywords4) { $ERROR[] = "検索キーワード4が入力されておりません。"; }
	//if (!$generic_keywords5) { $ERROR[] = "検索キーワード5が入力されておりません。"; }
	//	add ookawara 2015/09/14	end

	if ($_POST['amazonshop'] == 1) {	//	add ookawara 2017/01/16
		//	add	ookawara 2015/09/27
		if (!preg_match("/^2_/", $ad_id)) {
			if (!$product_subtype) { $ERROR[] = "商品タイプが選択されておりません。"; }
		}
		if (count($department_name) < 1) {
			$ERROR[] = "Amazon 対象年齢・性別が、選択されておりません。";
		} elseif (preg_match("/^2_/", $ad_id)) {
			if (count($department_name) > 5) { $ERROR[] = "Amazon 対象年齢・性別が、5より多く選択されております。"; }
		}
	}									//	add ookawara 2017/01/16
	$set_department_name = implode(",", $department_name);

	//	add ohkawara 2017/02/08
	if ($wowmashop == 1) {
		if (!$wd_id) { $ERROR[] = "Wowmaディレクトリーが選択されておりません。"; }
	}

	//	以下はエラーチェック無し
	//	$distribution_designation
	//	$model_year
	//	$seasons

	include ("../../cone.inc");
	if (!$ERROR) {
		$sql = "SELECT g_num FROM $goods_table WHERE g_num!='$g_num' AND code='$code';";
		if ($result = mysqli_query($conn_id,$sql)) {
			$count = mysqli_num_rows($result);
			if ($count > 0) {
				list($g_num_) = mysqli_fetch_array($result);
				$ERROR[] = "入力された商品コードは登録済みです。(管理商品番号：$g_num_)";
			}
		}
	}


	if (!$ERROR) {
		$caption = addslashes($caption);	//	add ookawara 2014/10/10

		if (!$price) { $price = "0"; }
		if (!$sale_price) { $sale_price = "0"; }
		if (!$brand) { $brand = "0"; }
		if (!$yahooshop) { $yahooshop = "0"; }
		//	add ookawara 2015/06/24
		if (!$_POST['amazonshop']) {
			$amazonshop = "0";
		} else {
			$amazonshop = $_POST['amazonshop'];
		}
		if (!$soldout) { $soldout = "0"; }

		//	add ookawara 2015/09/14
		if ($_POST['op_size']) {
			$op_size = $_POST['op_size'];
			foreach ($op_size AS $key => $VAL) {
				foreach ($VAL AS $key_name => $val) {
#					$val = mb_convert_kana($val, "asKV", "EUC-JP");
					$val = mb_convert_kana($val, "asKV", "UTF-8");
					$op_size[$key][$key_name] = trim($val);
				}
			}
			$size_list = serialize($op_size);
		}
		//add thao 2023/05/18
		if(!$ERROR && $pg_num){
			$sql = "UPDATE $goods_table set parent_flag='1' where g_num='$pg_num';";
			if (!mysqli_query($conn_id,$sql)) {$sql_check = "親グループ商品番号の親フラグのデーターを変更できませんでした。"; }
		}
		//	add ookawara 2015/0917
		if (!$zaiko) {
			$zaiko = 0;
		}
		
		if(!$parent_flag){$parent_flag = "0";}
		if(!$pg_num){$pg_num = "NULL";}
		if(!$gp_option){$gp_option = "NULL";}
		if(!$gp_option_name){$gp_option_name = "NULL";}else{$gp_option_name="'".$gp_option_name."'";}
		if (!$amz_price) { $amz_price = "0"; }	//	add ookawara 2015/10/08
		if(!$ERROR && $gp_option){
			$sql = "SELECT * FROM $goods_table WHERE pg_num='$pg_num' or pg_num='$g_num' or g_num='$pg_num' and del_flg='0';";
			if ($result = mysqli_query($conn_id,$sql)) {
				while($list = mysqli_fetch_array($result)){
					$g_num_check = $list['g_num'];
					$sql  = "UPDATE $goods_table SET gp_option='$gp_option' WHERE g_num='$g_num_check';";
					if (!mysqli_query($conn_id,$sql)) {$sql_check = "商品データーを変更できませんでした。"; }
				}
				if($sql_check){$ERROR[] = "$sql_check"; }
			}
		}
		$sql = "UPDATE $goods_table SET" .
				" g_name='$name'," .
				" code='$code'," .
				" price='$price'," .
				" sale_price='$sale_price'," .
				" options='$options'," .
				" headline='$headline'," .
				" caption='$caption'," .
				" abstract='$abstract'," .
				" brand='$brand'," .
				" comment='$comment'," .
				" yahooshop='$yahooshop'," .			//	add ohkawara 2020/12/05
				" soldout='$soldout'," .
				" zaiko='$zaiko',".						//	add ookawara 2015/09/17
				" rd_id='$rd_id'," .
				" yd_id='$yd_id'," .
				" amazonshop='$amazonshop'," .			//	add ookawara 2015/06/24
				" ad_id='$ad_id'," .					//	add ookawara 2015/07/24
				" size_list='$size_list',".					//	add ookawara 2015/09/14
				" name_head='$name_head',".					//	add ookawara 2015/09/14
				" bullet_point1='$bullet_point1',".			//	add ookawara 2015/09/14
				" bullet_point2='$bullet_point2',".			//	add ookawara 2015/09/14
				" bullet_point3='$bullet_point3',".			//	add ookawara 2015/09/14
				" bullet_point4='$bullet_point4',".			//	add ookawara 2015/09/14
				" bullet_point5='$bullet_point5',".			//	add ookawara 2015/09/14
				" generic_keywords1='$generic_keywords1',".	//	add ookawara 2015/09/14
				" generic_keywords2='$generic_keywords2',".	//	add ookawara 2015/09/14
				" generic_keywords3='$generic_keywords3',".	//	add ookawara 2015/09/14
				" generic_keywords4='$generic_keywords4',".	//	add ookawara 2015/09/14
				" generic_keywords5='$generic_keywords5',".	//	add ookawara 2015/09/14
				" name_foot='$name_foot',".					//	add ookawara 2015/09/24
				" product_subtype='$product_subtype',".		//	add ookawara 2015/09/24
				" ad_id2='$ad_id2',".						//	add ookawara 2015/09/24
				" department_name='$set_department_name',".	//	add ookawara 2015/09/27
				" update_time=now(),".						//	add ookawara 2015/09/27
				" distribution_designation='$distribution_designation',".	//	add ookawara 2015/10/01
				" model_year='$model_year',".								//	add ookawara 2015/10/01
				" seasons='$seasons',".										//	add ookawara 2015/10/01
				" amz_price='$amz_price',".									//	add ookawara 2015/10/01
				" wd_id='$wd_id',".											//	add ohkawara 2017/01/08
				" wowmashop='$wowmashop',".									//	add thao 2023/04/12
				" pg_num=$pg_num,".											//	add thao 2023/04/12
				" parent_flag=$parent_flag,".											//	add thao 2023/04/12
				" gp_option='$gp_option',".									//	add thao 2023/04/12
				" gp_option_name=$gp_option_name".									//	add ohkawara 2017/01/08
				" WHERE g_num='$g_num';";	
		if (!mysqli_query($conn_id,$sql)) {$ERROR[] = "商品データーを変更できませんでした。(1)"; }
	}
	if (!$ERROR) {
		$sql = " SELECT state FROM $cate_table WHERE num='$num';";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$state = $list['state'];
		}
		if ($state == 1) { $state = 1; }
		elseif ($state == 3) { $state = 3; }
		else { $state = 2; }

		$sql = "UPDATE $cate_table SET" .
				" ypath='$ypath'," .
				" state='$state'" .
				" WHERE g_num='$g_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。(2)"; }
	}

	if (!$ERROR) {
		if ($b_code != $code) {
			$sql = "SELECT copy_num FROM $cate_table WHERE num='$num';";
			if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "商品名を読み込みできませんでした。"; }
			else {
				list($copy_num_) = mysqli_fetch_array($result);
				if ($copy_num_) { $num_ = "-$copy_num_"; }
				else { $num_ = ""; }
			}

			$f_file1 = "$IMAGEF/$b_code$num_.jpg";
			$f_file2 = "$IMAGEF/$code$num_.jpg";
			if (file_exists($f_file1)) { rename($f_file1,$f_file2); }

			$b_file1 = "$IMAGEB/$b_code$num_.jpg";
			$b_file2 = "$IMAGEB/$code$num_.jpg";
			if (file_exists($b_file1)) { rename($b_file1,$b_file2); }

			//	add ookawara 2013/07/03
			$i03_file1 = IMAGE03."/$b_code$num_.jpg";
			$i03_file2 = IMAGE03."/$code$num_.jpg";
			if (file_exists($i03_file1)) { rename($i03_file1,$i03_file2); }

			$i04_file1 = IMAGE04."/$b_code$num_.jpg";
			$i04_file2 = IMAGE04."/$code$num_.jpg";
			if (file_exists($i04_file1)) { rename($i04_file1,$i04_file2); }

			$i05_file1 = IMAGE05."/$b_code$num_.jpg";
			$i05_file2 = IMAGE05."/$code$num_.jpg";
			if (file_exists($i05_file1)) { rename($i05_file1,$i05_file2); }
		}
	}

	//	更新したファイルを記入
	if ($g_num) {
		$OUT = fopen($renew_file,"a");
		fwrite($OUT,"$g_num\n");
		fclose($OUT);
		@chmod($up_file,0666);
	}

	if (!$ERROR) {
		$msg = "商品名：$name を変更しました。<BR>\n<BR>\n";
	}

	// mysqli_close($conn_id);

}



function delete1() {
// global	$ERROR,$msg,$cate_table,$goods_table,$del_goods_table,$num,$g_num,$IMAGEF,$IMAGEB,$CATE_DIR,
// 		$renew_file;
global	$ERROR,$IMAGEF,$IMAGEB,$CATE_DIR,$cate_table,$goods_table,$del_goods_table;
global $msg; 
$num=$_POST['num'];
$g_num=$_POST['g_num'];
$pg_num=$_POST['pg_num'];

	include ("../../cone.inc");


	$sql = "SELECT g_name, code, options FROM $goods_table WHERE g_num='$g_num';";
	if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "商品名を読み込みできませんでした。"; }
	else {
		list($name,$code,$options) = mysqli_fetch_array($result);
	}
	// add thao 2023/04/12 start
	if(!$ERROR){
		if($g_num){
			$cate_name = [];
			$count = 0;
			$sql = "SELECT c.cate1,c.cate2,c.cate3,j.g_num,c.state FROM $goods_table j INNER JOIN $cate_table c ON pg_num='$g_num' and del_flg='0' AND c.g_num=j.g_num;";
			if ($result = mysqli_query($conn_id,$sql)) {
				while ($list = mysqli_fetch_array($result)) {
					if($list['g_num'] && ($list["state"]!=3)){
						$cate1 = $list["cate1"];
						$cate2 = $list["cate2"];
						$cate3 = $list["cate3"];
						$cate_name_check=category_name_check($cate1,$cate2,$cate3);
						$count++;
						$cate_name[]=$cate_name_check." の ".$list['g_num'];
					}
				}
				$cate_name = array_unique($cate_name);
				if(($count>=1)){
					foreach($cate_name as $value){
						$child_name.=$value."<br>";
					}
					$child_num = rtrim($child_num,"<br>");
					$ERROR[] = "商品グループ親の商品のため、削除できません。子の管理商品番号:<br>".$child_name;
				}
			}
		}
	}
	// add thao 2023/04/12 end

	if (!$ERROR && $name && $code) {
		$default_state=3;
		$sql = "INSERT INTO $del_goods_table SELECT *,{$default_state} FROM $goods_table WHERE g_num='$g_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを移行できませんでした。"; }
						}

	if (!$ERROR) {
		$sql = "DELETE FROM $goods_table WHERE g_num='$g_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを削除できませんでした。"; }
					}
				
	if (!$ERROR) {
		$sql = "SELECT num, cate1, cate2, cate3, copy_num, state FROM $cate_table WHERE g_num='$g_num';";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "カテゴリーデーターを読み込みできませんでした。"; }
		else {
			$max = mysqli_num_rows($result);
			for($ctrCate=0; $ctrCate<$max; $ctrCate++) {
				list($num_,$cate1_,$cate2_,$cate3_,$copy_num_,$state_) = mysqli_fetch_array($result);
				$DEL_NUM[] = $num_;
				$lc_file = "$CATE_DIR/$cate1_" . "_$cate2_" . "_$cate3_.dat";
				if (file_exists($lc_file)) {
					unset($N_LIST);
					$LIST = file($lc_file);
					foreach ($LIST AS $val) {
						$val = trim($val);
						list($g_num_) = explode("<>",$val);
						if ($g_num != $g_num_) {
							$N_LIST .= "$val\n";
						}
					}
					if ($N_LIST) {
						$OUT = fopen($lc_file,"w");
						fwrite($OUT,$N_LIST);
						fclose($OUT);
					}
					else { unlink($lc_file); }

					if (!$copy_num_) { $copy_num = ""; } else { $copy_num = "-$copy_num_"; }

					$f_file = "$IMAGEF/$code$copy_num.jpg";
					if (file_exists($f_file)) { unlink($f_file); }
					$b_file = "$IMAGEB/$code$copy_num.jpg";
					if (file_exists($b_file)) { unlink($b_file); }

/*
					//	del ookawara 2015/10/08
					//	add ookawara 2013/07/03
					$i03_file = IMAGE03."/$code$copy_num.jpg";
					if (file_exists($i03_file)) { unlink($i03_file); }
					$i04_file = IMAGE04."/$code$copy_num.jpg";
					if (file_exists($i04_file)) { unlink($i04_file); }
					$i05_file = IMAGE05."/$code$copy_num.jpg";
					if (file_exists($i05_file)) { unlink($i05_file); }
*/

					//	add ookawara 2015/10/08
					for ($i=3; $i<=9; $i++) {
						$image_name = "image0".$i;
						$i_num_file = "../".$image_name."/".$code.$copy_num.".jpg";
						if (file_exists($i_num_file)) {
							unlink($i_num_file);
						}
					}
				}

				if ($state_ == 1 || $state_ == 4) {
					//	削除
					$sql = "DELETE FROM $cate_table WHERE num='$num_';";
				}
				else {
					//	更新
					$sql  = "UPDATE $cate_table SET" .
							" display='2'," .
							" state='3'" .
							//" update_time=now()".						//	add ookawara 2015/09/27	//	del ookawara 2016/01/28
							" WHERE num='$num_';";
				}
				if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品カテゴリーデーターを変更できませんでした。"; }
			}
		}
	}

	if (!$ERROR) { $msg = "商品名：$name を削除しました。<NBR>\n<BR>\n"; }

	//mysqli_close($conn_id);

}



function delete2() {
// global	$ERROR,$msg,$cate_table,$goods_table,$del_goods_table,$num,$g_num,$IMAGEF,$IMAGEB,$copy_num,$lc_file,
// 		$renew_file;
global	$ERROR,$cate_table,$lc_file,$renew_file,$cate_table,$goods_table,$del_goods_table,$IMAGEF,$IMAGEB;
global $msg; 
$g_num=$_POST['g_num'];
$num=$_POST['num'];
$pg_num=$_POST['pg_num'];
$copy_num=$_POST['copy_num'];

	include ("../../cone.inc");
	// add thao 2023/04/12 start
	if($g_num){
		$cate_name = [];
		$count = 0;
		$sql = "SELECT c.cate1,c.cate2,c.cate3,j.g_num,c.state FROM $goods_table j INNER JOIN $cate_table c ON pg_num='$g_num' and del_flg='0' AND c.g_num=j.g_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				if($list['g_num'] && ($list["state"]!=3)){
					$cate1 = $list["cate1"];
					$cate2 = $list["cate2"];
					$cate3 = $list["cate3"];
					$cate_name_check=category_name_check($cate1,$cate2,$cate3);
					$count++;
					$cate_name[]=$cate_name_check." の ".$list['g_num'];
				}
			}
			$cate_name = array_unique($cate_name);
			if(($count>=1)){
				foreach($cate_name as $value){
					$child_name.=$value."<br>";
				}
				$child_num = rtrim($child_num,"<br>");
				$ERROR[] = "商品グループ親の商品のため、削除できません。子の管理商品番号:<br>".$child_name;
			}
		}
	}
	unset($state_);
    if(!$ERROR){
		$sql  = "SELECT state FROM $cate_table".
			" WHERE num='{$num}';";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$state_ = $list['state'];
		}

		if ($state_ == 1 || $state_ == 4) {
			$sql = "DELETE FROM $cate_table WHERE num='$num';";
		} else {
			//	更新
			$sql  = "UPDATE $cate_table SET" .
					" state='3'" .
					//" update_time=now()".						//	add ookawara 2015/09/27	//	del ookawara 2016/01/28
					" WHERE num='$num';";
		}
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。"; }
	}



/*
	$sql = "SELECT num, copy_num, state FROM $cate_table WHERE g_num='$g_num';";
	if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを読み込みできませんでした。"; }
	else {
		$count = mysqli_num_rows($result);
	}

	$sql = "SELECT g_name, code, options FROM $goods_table WHERE g_num='$g_num';";
	if (!$result2 = mysqli_query($conn_id,$sql)) { $ERROR[] = "商品名を読み込みできませんでした。"; }
	else {
		list($name,$code,$options) = mysqli_fetch_array($result2,0);
	}

	if (!$ERROR && $copy_num == 0) {
		for($i=0; $i<$count; $i++) {
			list($num_,$copy_num_,$state_) = mysqli_fetch_array($result,$i);
			if ($num != $num_) {
				$n_num = $num_;
				$copy_num = $copy_num_;
				$state = $state_;
				break;
			}
		}

		if ($state_ == 1 || $state_ == 4) {
			//	削除
			$sql = "DELETE FROM $cate_table WHERE num='$n_num';";
		}
		else {
			//	更新
			$sql  = "UPDATE $cate_table SET" .
					" display='2'," .
					" state='3'," .
					" update_time=now()".						//	add ookawara 2015/09/27
					" WHERE num='$n_num';";
		}
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。"; }
	}

	if (!$ERROR) {
		if ($copy_num > 0) { $dc_num = "-$copy_num"; } else { $dc_num = ""; }
		$f_file = "$IMAGEF/$code$dc_num.jpg";
		if (file_exists($f_file)) { unlink($f_file); }
		$b_file = "$IMAGEB/$code$dc_num.jpg";
		if (file_exists($b_file)) { unlink($b_file); }
	}
*/

	if (!$ERROR && file_exists($lc_file)) {
		$LIST = file($lc_file);
		foreach ($LIST AS $val) {
			$val = trim($val);
			list($g_num_) = explode("<>",$val);
			if ($g_num != $g_num_) {
				$N_LIST .= "$val\n";
			}
		}

		if ($N_LIST) {
			$OUT = fopen($lc_file,"w");
			fwrite($OUT,$N_LIST);
			fclose($OUT);
		}
		else {
			unlink($lc_file);
		}
	}

/*
	if (!$ERROR && $count <= 1) {
		$sql = "INSERT INTO $del_goods_table SELECT * FROM $goods_table WHERE g_num='$g_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを削除できませんでした。1"; }

		$sql = "DELETE FROM $goods_table WHERE g_num='$g_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを削除できませんでした。2"; }
	}
*/

	if (!$ERROR) { $msg = "商品名：$name を削除しました。<NBR>\n<BR>\n"; }

	//mysqli_close($conn_id);

	//	更新したファイルを記入
	if ($num) {
		$OUT = fopen($renew_file,"a");
		fwrite($OUT,"$num\n");
		fclose($OUT);
		@chmod($up_file,0666);
	}

}



function up() {
// global	$lc_file,$g_num;
global	$lc_file;
$g_num=$_POST['g_num'];

	if (file_exists($lc_file)) {
		$LIST = file($lc_file);
		$i = 1;
		foreach($LIST AS $val) {
			list($g_num_) = explode("<>",$val);
			$N_LIST[$i] = $g_num_;
			if ($g_num == $g_num_) {
				$up = $i;
				$down = $i - 1;
			}
			$i++;
		}

		foreach ($N_LIST AS $key => $val) {
			if ($key == $up) {
				$NC[$down] = "$val<>\n";
			}
			elseif ($key == $down) {
				$NC[$up] = "$val<>\n";
			}
			else {
				$NC[$key] = "$val<>\n";
			}
		}
		ksort($NC);

		$n_list = "";
		foreach ($NC AS $val) {
			$n_list .= $val;
		}

		$OUT = fopen($lc_file,"w");
		fwrite($OUT,$n_list);
		fclose($OUT);
	}

}



function down() {
// global	$lc_file,$g_num;
global	$lc_file;
$g_num=$_POST['g_num'];

	if (file_exists($lc_file)) {
		$LIST = file($lc_file);
		$i = 1;
		foreach($LIST AS $val) {
			list($g_num_) = explode("<>",$val);
			$N_LIST[$i] = $g_num_;
			if ($g_num == $g_num_) {
				$up = $i + 1;
				$down = $i;
			}
			$i++;
		}

		foreach ($N_LIST AS $key => $val) {
			if ($key == $up) {
				$NC[$down] = "$val<>\n";
			}
			elseif ($key == $down) {
				$NC[$up] = "$val<>\n";
			}
			else {
				$NC[$key] = "$val<>\n";
			}
		}
		ksort($NC);

		$n_list = "";
		foreach ($NC AS $val) {
			$n_list .= $val;
		}

		$OUT = fopen($lc_file,"w");
		fwrite($OUT,$n_list);
		fclose($OUT);
	}

}



//	写真更新
function picrenew() {
// global $cate_table,$num;
global $cate_table;
$num=$_POST['num'];

	include ("../../cone.inc");

	$sql = " SELECT state, g_num FROM $cate_table WHERE num='$num';";	//	add g_num ookawara 2013/08/05
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$state = $list['state'];
		$g_num = $list['g_num'];	//	add ookawara 2013/08/05
	}

/*
	//	del ookawara 2014/02/05
	if ($state == 0) {
		$sql = "UPDATE $cate_table SET" .
				" state='2'" .
				//" WHERE num='$num';";
				" WHERE g_num='".$g_num."';";	//	add ookawara 2013/08/05
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。(2)"; }
	}
*/

	//	add ookawara 2014/02/05
	$sql  = "INSERT INTO ".T_UP_PIC.
			" VALUES('".$num."', '".$g_num."', now());";
	if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "写真更新を登録できませんでした。"; }


}
function category_name_check($cate1,$cate2,$cate3){
	global	$CATE_DIR,$c_file,$b_file,$list_file;
	include ("../../cone.inc");
	$g_num_check="";
	//カテゴリーの名前取り出し start
	$mc_file = "$CATE_DIR/".$cate1.".dat";
	$sc_file = "$CATE_DIR/".$cate1."_".$cate2.".dat";
	$lc_file = "$CATE_DIR/".$cate1."_".$cate2."_".$cate3.".dat";
	$mc = $cate1;
	$sc = $cate2;
	$lc = $cate3;

	$C_LIST = file($c_file);
	foreach ($C_LIST AS $val) {
		list($h_num_,$mc_num_,$mc_name_) = explode("<>",$val);
		if ($mc == $mc_num_) { $mc_name = $mc_name_; break; }
	}

	if ($mc && file_exists($mc_file)) {
		$SC_LIST = file($mc_file);
		foreach ($SC_LIST AS $val) {
			list($h_num_,$sc_num_,$sc_name_) = explode("<>",$val);
			if ($sc == $sc_num_) { $sc_name = $sc_name_; break; }
		}
	}

	if ($sc && file_exists($sc_file)) {
		$LC_LIST = file($sc_file);
		foreach ($LC_LIST AS $val) {
			list($h_num_,$lc_num_,$lc_name_) = explode("<>",$val);
			if ($lc == $lc_num_) { $lc_name = $lc_name_; break; }
		}
	}

	if (file_exists($b_file)) {
		$B_LIST = file($b_file);
		foreach ($B_LIST AS $val) {
			list($b_num_,$b_name_,$del_) = explode("<>",$val);
			if ($brand == $b_num_) { $b_name = $b_name_; break; }
		}
	}

	if (file_exists($list_file)) {
		$Y_LIST = file($list_file);
		foreach ($Y_LIST AS $val) {
			list($y_name_,$ypath_,$del_) = explode("<>",$val);
			if ($ypath == $y_name_) { $y_name = $y_name_; break; }
		}
	}
	$g_num_check=$mc_name." > ".$sc_name." > ".$lc_name;
	return $g_num_check;
}