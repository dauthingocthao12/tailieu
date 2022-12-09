<?PHP

//	商品登録
function goods_2() {
// global	$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,
// 		$mode,$cate_table,$goods_table,
//  		$mc,$sc,$lc;
global $cate_table,$sc_file,$lc_file,$b_file,$list_file,$goods_table,$c_file,$mc_file;
global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment,$rd_id,$yd_id,$ad_id,$ad_id2,$wd_id;// _POST
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
$comment=$_POST['comment'];
$rd_id=$_POST['rd_id'];
$yd_id=$_POST['yd_id'];
$ad_id=$_POST['ad_id'];
$ad_id2=$_POST['ad_id2'];
$wd_id=$_POST['wd_id'];

$mode=$_POST['mode'];
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$lc=$_POST['lc'];

	include("./r_y_dir.php");

	$c_file = "../category/category.inc";
	$mc_file = "../category/$mc.dat";
	$sc_file = "../category/$mc" . "_$sc.dat";
	$lc_file = "../category/$mc" . "_$sc" . "_$lc.dat";
	$b_file = "../data/brand.dat";
	$list_file = "./data/ypath.dat";

	$cate_table = "category";
	$goods_table = "goods";

	if ($mode == "news") { news(); }

	first00();

}


function first00() {
	//   global	$PHP_SELF,$ERROR,$msg,
	//  		$c_file,$mc_file,$sc_file,$b_file,$list_file,
	//   		$main,$goods,
	//  		$mc,$sc,$lc,$mc_r,$sc_r,$brand,$ypath,
	//   		$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment
	//   		,$rd_id,$yd_id;
	//   global $ad_id;			//	add ookawara 2015/07/24
	//   global $ad_id2;			// 	add ookawara 2015/09/24
	//   global $wd_id;			//	add ohkawara 2017/02/07
	global	$PHP_SELF,$ERROR,$c_file,$mc_file,$sc_file,$b_file,$list_file;
	global $msg, //@source news()
		   $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment,$rd_id,$yd_id,$ad_id,$ad_id2,$wd_id; //@source goods_2()<-_POST, news()<-goods_2
	$main=$_POST['main'];
	$goods=$_POST['goods'];
	$mc=$_POST['mc'];
	$sc=$_POST['sc'];
	$lc=$_POST['lc'];
	$mc_r=$_POST['mc_r'];
	$sc_r=$_POST['sc_r'];
	$ypath=$_POST['ypath'];

	if ($mc != $mc_r) { $sc = $lc = ""; }
	elseif ($sc != $sc_r) { $lc = ""; }

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
	}
	else {
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
	}
	else {
		$l_lc = "        <OPTION value=\"\" $selected>-----</OPTION>\n";
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

	if (!$ERROR && !isset($yshop)) { $y_checked = "checked"; }
	elseif ($yahooshop == 1) { $y_checked = "checked"; }
	elseif ($msg) { $y_checked = "checked"; }
	else { $y_checked = ""; }
	if ($soldout == 1) { $s_checked = "checked"; } else { $s_checked = ""; }
	if ($_POST['amazonshop'] == 1) { $a_checked = "checked"; } else { $a_checked = ""; }	//	add ookawara 2015/06/25
	if ($_POST['wowmashop'] == 1) { $w_checked = "checked"; } else { $w_checked = ""; }		//	add ohkawara 2017/02/07

	$name = stripslashes($name);
	$code = stripslashes($code);
	$options = stripslashes($options);
	$headline = stripslashes($headline);
	$abstract = stripslashes($abstract);
	$comment = stripslashes($comment);

	$l_rd = rd_list($rd_id);
	$l_yd = yd_list($yd_id);
	$l_ad = ad_list($ad_id);	//	add ookawara 2015/07/24
	$l_wd = wd_list($wd_id);	//	add ohkawara 2017/02/07


	if ($ERROR) {
		ERROR($ERROR);
		unset($ERROR);
	}

	echo <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc_r" value="$sc">
カテゴリー、ブランド名、楽天・Yahooディレクトリーを選択し、決定ボタンを押して下さい。<BR>
<TABLE border="0" style="font-size : 12px;" cellpadding="1" cellspacing="1" bgcolor="#666666" width="800">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD><SELECT name="mc" onchange="submit();">
$l_mc
      </SELECT><SELECT name="sc" onchange="submit();">
$l_sc
      </SELECT><SELECT name="lc">
$l_lc
      </SELECT></TD>
      <TD rowspan="6" align="center"><INPUT type="submit" value="決定"></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">ブランド名</TH>
      <TD><SELECT name="brand">
$l_brand
      </SELECT></TD>
    </TR>
<!--
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Ypath</TH>
      <TD><SELECT name="ypath">
$l_ypath
      </SELECT></TD>
    </TR>
-->
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">楽天ディレクトリー</TH>
      <TD><SELECT name="rd_id">
$l_rd
      </SELECT></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">YAHOOディレクトリー</TH>
      <TD><SELECT name="yd_id">
$l_yd
      </SELECT></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード</TH>
      <TD><SELECT name="ad_id">
$l_ad
      </SELECT><br>
      シューズやバッグは、スポーツ＆アウトドアではなくシューズ＆バッグから始まるブラウズノードを選択して下さい。<br>
      ユニフォームや衣類は、スポーツ＆アウトドアではなく服＆ファッション小物から始まるブラウズノードを選択して下さい。
      </TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Wowmaディレクトリー</TH>
      <TD><SELECT name="wd_id">
$l_wd
      </SELECT></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	if ($mc && $sc && $lc && $brand && $rd_id && $yd_id) {

		$rd_name = rd_val($rd_id);
		$yd_name = yd_val($yd_id);
		$ad_name = ad_val($ad_id);	//	add ookawara 2015/07/24
		$wd_name = wd_val($wd_id);	//	add ohkawara 2017/02/07


		//	add ookawara 2015/09/14	start
		$op_size = $_POST['op_size'];
		$name_head = $_POST['name_head'];
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

		//	add ookawara 2015/09/17
		$zaiko = $_POST['zaiko'];
		$zaiko_checked = "";
		if ($zaiko == 1) {
			$zaiko_checked = "checked";
		}

		//	add ookawara 2015/09/24
		$name_foot = $_POST['name_foot'];
		$product_subtype = $_POST['product_subtype'];
		$ad_id2 = $_POST['ad_id2'];
		$list_type = 2;
		if (preg_match("/^2_/", $ad_id)) {
			$list_type = 99;
		}
		$l_ad2 = ad_list($ad_id2, $list_type);
		$l_product_subtype = amz_product_subtype($product_subtype, $list_type);

		//	add ookawara 2015/09/27
		$department_name = $_POST['department_name'];
		$department_name_chk = amz_department_name($department_name, $list_type);

		//	add ookawara 2015/10/01
		//	Amazon 販売形態(並行輸入品)
		$distribution_designation = $_POST['distribution_designation'];
		$chk_distribution_designation = "";
		if ($distribution_designation == 1) {
			$chk_distribution_designation = "checked";
		}
		//	Amazon モデル年(発売年・発表年)
		$model_year = $_POST['model_year'];
		$l_model_year = amz_model_year_list($model_year, $list_type);
		//	Amazon シーズン
		$seasons = $_POST['seasons'];
		$l_seasons = amz_seasons_list($seasons, $ad_id);

		//	Amazon金額
		//	add ookawara 2015/10/08
		$amz_price = $_POST['amz_price'];
		if (!$amz_price) {
			$amz_price = "";
		}

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc" value="$mc">
<INPUT type="hidden" name="sc" value="$sc">
<INPUT type="hidden" name="lc" value="$lc">
<INPUT type="hidden" name="mc_r" value="$mc">
<INPUT type="hidden" name="sc_r" value="$sc">
<INPUT type="hidden" name="brand" value="$brand">
<INPUT type="hidden" name="rd_id" value="$rd_id">
<INPUT type="hidden" name="yd_id" value="$yd_id">
<INPUT type="hidden" name="ad_id" value="$ad_id">
<INPUT type="hidden" name="wd_id" value="$wd_id">
<INPUT type="hidden" name="mode" value="news">
カテゴリー、ブランド名、楽天・Yahooディレクトリーに間違いなければ入力し登録ボタンを押して下さい。<BR>
<TABLE border="0" cellpadding="1" cellspacing="1" bgcolor="#666666" style="font-size : 12px;" width="800">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc" width="80">カテゴリー</TH>
      <TD colspan="3">$mc_name // $sc_name // $lc_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">ブランド名</TH>
      <TD colspan="3">$B_LINE[$brand]</TD>
    </TR>
<!--
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Ypath</TH>
      <TD colspan="3">$ypath</TD>
    </TR>
-->
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">楽天ディレクトリー</TH>
      <TD colspan="3">$rd_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Yahooディレクトリー</TH>
      <TD colspan="3">$yd_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Amazon ブラウズノード</TH>
      <TD colspan="3">$ad_name</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TH bgcolor="#cccccc">Wowmaディレクトリー</TH>
      <TD colspan="3">$wd_name</TD>
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

	//	add ookawara 2015/09/14	start
	//	他に追加項目も有りHTMLの中のため詳細省略

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

		//echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"1\" ".$checked1."/>:販売\n";
		echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"2\" ".$checked2."/>:NEW\n";
		//echo "      <input type=\"radio\" name=\"op_size[".$i."][type]\" value=\"3\" ".$checked3."/>:非表示\n";
		echo "      </div>\n";
		$i++;
	}
	$max_cnt = $i;

	//	add ookawara 2015/09/14	end

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
	//div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="1" />:通常 ';
	div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="2" checked />:NEW ';
	//div_element.innerHTML += '<input type="radio" name="op_size['+counter+'][type]" value="3" />:非表示';
	var parent_object = document.getElementById("size_form");
	parent_object.appendChild(div_element);
	counter++;
}
</script>
Amazonに出品する商品の場合、必ずJANを入力して下さい。<br>
<span style="color:red;">JANが無いオリジナル商品</span>は「none」と入力して下さい。<br>
オリジナル商品では無い場合は、「none」と入れないで下さい。amazonの出品もしないでください。<br>
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
      <TD colspan="4"><INPUT type="submit" value="登録">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;

	}
	else {
		echo<<<WAKABA
カテゴリー、ブランド名、Ypathを選択してください。<BR>

WAKABA;
	}

}



function news() {
//   global	$PHP_SELF,$ERROR,$msg,
//  		$c_file,$mc_file,$sc_file,$lc_file,$b_file,$list_file,$del_file,
//  		$main,$goods,$cate_table,$goods_table,
//  		$mc,$sc,$lc,$mc_r,$sc_r,$brand,$ypath,
//  		$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment
//  		,$rd_id,$yd_id;
//  global $ad_id;		// 	add ookawara 2015/07/24
//  global $ad_id2;		//	add ookawara 2015/09/24

global	$ERROR,$cate_table,$goods_table,$lc_file;
global $msg;

global $brand,$yahooshop,$soldout,$name,$code,$price,$sale_price,$options,$headline,$caption,$abstract,$comment,$rd_id,$yd_id,$ad_id,$ad_id2,$wd_id; //@source goods_2()<_POST
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$lc=$_POST['lc'];

$ypath=$_POST['ypath'];

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

	$ad_id = trim($ad_id);		//	add ookawara 2015/09/16
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
	$wd_id = trim($wd_id);
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
	if ($name && $name_len > 128) { $ERROR[] = "商品名が文字数越えてます。"; }	//	chage 150->128 ohkawara 2017/02/16
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
//	if ($price && $sale_price && $price < $sale_price) { $ERROR[] = "金額が特価の方が高いです。"; }
//	if (!$caption) { $ERROR[] = "商品説明が入力されておりません。"; }
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

/*
	//	del ookawara 2015/09/14
	//	サイズの頭に/をつける。
##	$options = ereg_replace("\r","",$options);
	$options = preg_replace("/\r/","",$options);
##	$options = ereg_replace("/","",$options);
	$options = preg_replace("/\//","",$options);
	$OPT = explode("\n",$options);
	$n_options = "";
	if ($OPT) {
		foreach ($OPT AS $VAL) {
			$n_options .= "/$VAL\n";
		}
	}
	$n_options = trim($n_options);
*/

	//	add ookawara 2015/09/14	start
	$size_cnt = 1;
	if ($op_size) {
		$CK_OP = array();		//	add ookawara 2015/09/24
		$_POST['op_size'] = array();
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['size'] == "" && $VAL['jan'] == "" && $VAL['asin'] == "") {
				continue;
			} elseif (($VAL['jan'] != "" || $VAL['asin'] != "") && $VAL['size'] == "") {
				$ERROR[] = $size_cnt."番目のサイズ情報が記載されておりません。";
			}
			$ck_size = $VAL['size'];		//	add ookawara 2015/09/24

			//	add ookawara 2015/09/24
			if ($CK_OP[$ck_size]) {
				$ERROR[] = $size_cnt."番目のサイズ情報が重複しております。";
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

	//	以下はエラーチェック無し
	//	$distribution_designation
	//	$model_year
	//	$seasons

	//	add ohkawara 2017/02/08
	if ($wowmashop == 1) {
		if (!$wd_id) { $ERROR[] = "Wowmaディレクトリーが選択されておりません。"; }
	}

	include ("../../cone.inc");
	if (!$ERROR) {
		$sql = "SELECT count(*) FROM $goods_table WHERE code='$code';";
		if ($result = pg_exec($conn_id,$sql)) {
			list($count) = pg_fetch_array($result,0);
			if ($count > 0) {
				$sql = "SELECT g_num FROM $goods_table WHERE code='$code';";
				if ($result = pg_exec($conn_id,$sql)) {
				list($g_num) = pg_fetch_array($result,0);
				$ERROR[] = "入力された商品コードの商品は登録済みです。(管理商品番号：$g_num)";
				}
			}
		}
	}

	if (!$ERROR) {
		$sql = "SELECT MAX(g_num) FROM $goods_table;";
		if ($result = pg_exec($conn_id,$sql)) {
			list($g_num) = pg_fetch_array($result,0);
		}
		else { $ERROR[] = "商品番号を呼び出せませんでした。"; }

		if (!$ERROR) {
			$caption = addslashes($caption);	//	add ookawara 2014/10/10

			$g_num = $g_num + 1;
			if (!$price) { $price = "0"; }
			if (!$sale_price) { $sale_price = "0"; }
			if (!$brand) { $brand = "0"; }
			if (!$yahooshop) { $yahooshop = "0"; }
			if (!$soldout) { $soldout = "0"; }
			if (!$zaiko) { $zaiko = "0"; }
			//	add ookawara 2015/06/24
			if (!$_POST['amazonshop']) {
				$amazonshop = "0";
			} else {
				$amazonshop = $_POST['amazonshop'];
			}

			//	add ookawara 2015/09/14
			if ($_POST['op_size']) {
				$op_size = $_POST['op_size'];
				foreach ($op_size AS $key => $VAL) {
					foreach ($VAL AS $key_name => $val) {
#						$val = mb_convert_kana($val, "asKV", "EUC-JP");
						$val = mb_convert_kana($val, "asKV", "UTF-8");
						$op_size[$key][$key_name] = trim($val);
					}
				}
				$size_list = serialize($op_size);
			}

			if (!$amz_price) { $amz_price = "0"; }	//	add ookawara 2015/10/08

			$sql = "INSERT INTO $goods_table VALUES (" .
					"'$g_num'," .
					"'$name'," .
					"'$code'," .
					"'$price'," .
					"'$sale_price'," .
					"'$n_options'," .
					"'$headline'," .
					"'$caption'," .
					"'$abstract'," .
					"'$relevant_links'," .
					"'$ship_weight'," .
					"'$brand'," .
					"'$comment'," .
					"'$yahooshop'," .
					"'$soldout'," .
					"'$zaiko'," .
					"'$rd_id'," .
					"'$yd_id'," .
					"'0',".
					"'".$amazonshop."',".
					"'".$ad_id."',".					//	add ookawara 2015/07/24
					"'".$size_list."',".					//	add ookawara 2015/09/14
					"'".$name_head."',".					//	add ookawara 2015/09/14
					"'".$bullet_point1."',".			//	add ookawara 2015/09/14
					"'".$bullet_point2."',".			//	add ookawara 2015/09/14
					"'".$bullet_point3."',".			//	add ookawara 2015/09/14
					"'".$bullet_point4."',".			//	add ookawara 2015/09/14
					"'".$bullet_point5."',".			//	add ookawara 2015/09/14
					"'".$generic_keywords1."',".	//	add ookawara 2015/09/14
					"'".$generic_keywords2."',".	//	add ookawara 2015/09/14
					"'".$generic_keywords3."',".	//	add ookawara 2015/09/14
					"'".$generic_keywords4."',".	//	add ookawara 2015/09/14
					"'".$generic_keywords5."',".	//	add ookawara 2015/09/14
					"'".$name_foot."',".		//	add ookawara 2015/09/24
					"'".$product_subtype."',".	//	add ookawara 2015/09/24
					"'".$ad_id2."',".			//	add ookawara 2015/09/24
					"'".$set_department_name."',".	//	add ookawara 2015/09/27
					"now(),".						//	add ookawara 2015/09/27
					"now(),".						//	add ookawara 2015/09/27
					"'".$distribution_designation."',".	//	add ookawara 2015/10/01
					"'".$model_year."',".				//	add ookawara 2015/10/01
					"'".$seasons."',".					//	add ookawara 2015/10/01
					"'".$amz_price."',".				//	add ookawara 2015/10/08
					"'".$wd_id."',".					//	add ohkawara 2017/02/08
					"'".$wowmashop."'".					//	add ohkawara 2017/02/08
					");";
			if (!pg_exec($conn_id,$sql)) { $ERROR[] = "商品データーを登録できませんでした。"; }
		}

		if (!$ERROR) {
			if (!$g_num) { $g_num = "0"; }
			if (!$display) { $display = "0"; }
			if (!$copy_num) { $copy_num = "0"; }
			if ($display == "0") { $state = 4; }
			else { $state = 1; }

			$sql = "INSERT INTO $cate_table (cate1,cate2,cate3,g_num,ypath,display,copy_num,state) VALUES (" .
					"'$mc'," .
					"'$sc'," .
					"'$lc'," .
					"'$g_num'," .
					"'$ypath'," .
					"'$display'," .
					"'$copy_num'," .
					"'1');";
			if (!pg_exec($conn_id,$sql)) {
				$ERROR[] = "商品データーをカテゴリーに登録できませんでした。";
				$sql = "DELETE FROM $goods_table WHERE g_num='$g_num';";
				if (!pg_exec($conn_id,$sql)) { $ERROR[] = "商品データーを削除できませんでした。"; }
			}
		}

		//	ライブドアもデーターベースでチェックその為削除
		if (!$ERROR) {
			$OUT = fopen($lc_file,"a");
			fwrite($OUT,"$g_num<>\n");
			fclose($OUT);
			@chmod($lc_file,0666);
		}

		if (!$ERROR) {

			$sql = "SELECT MAX(num) FROM $cate_table;";
			if ($result = pg_exec($conn_id,$sql)) {
				list($num) = pg_fetch_array($result,0);
			}

			$msg = "商品名：$name を登録しました。<BR>\n<BR>\n";
			$yahooshop = $soldout = $name = $code = $price = $sale_price = $options = $headline = $caption = $abstract = $comment = "";

			//	add ookawara 2015/09/14
			unset($_POST['size_list']);
			unset($_POST['name_head']);
			unset($_POST['bullet_point1']);
			unset($_POST['bullet_point2']);
			unset($_POST['bullet_point3']);
			unset($_POST['bullet_point4']);
			unset($_POST['bullet_point5']);
			unset($_POST['generic_keywords1']);
			unset($_POST['generic_keywords2']);
			unset($_POST['generic_keywords3']);
			unset($_POST['generic_keywords4']);
			unset($_POST['generic_keywords5']);

			//	add ookawara 2015/09/24
			unset($_POST['name_foot']);
			unset($_POST['product_subtype']);
			unset($_POST['ad_id2']);

			//	add ookawara 2015/10/01
			unset($_POST['department_name']);
			unset($_POST['distribution_designation']);
			unset($_POST['model_year']);
			unset($_POST['seasons']);

			//	add ohkawara 2017/02/08
			unset($_POST['wd_id']);

		}
	}

	pg_close($conn_id);

}
?>
