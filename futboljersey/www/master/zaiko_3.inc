<?PHP

function zaiko_3() {
global $mode,$view;
$mode=$_POST['mode'];
$view=$_POST['view'];
	if ($mode == "hen") { hen(); }
	if ($mode == "write") { write(); }
	if ($view && $view!=0) { first_00(); }

	exit();

}



function first_00() {
global $main,$zaiko,$hinban_,$cate1,$cate2,$cate3,$view,$page,$touroku; //_POST
global $PHP_SELF,$ERROR,$max,$view_s,$view_e,$views;
include './array.inc';
include './list.inc';
include "../../cone.inc";
$hinban_=$_POST['hinban_'];
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];
$page=$_POST['page'];
$touroku=$_POST['touroku'];

	$sql_zaiko = "";
	$sql_l = "";
	$where = "";

	if ($touroku == 1) { $sql_zaiko = " category3!=''"; }
	elseif ($touroku == 2) { $sql_zaiko = " category3=''"; }

	if ($hinban_) {
		$sql_l .= " hinban LIKE '%$hinban_%'";
		}
	elseif ($cate1 && $cate1 != 0) {
		$sql_l .= " category1='$cate1'";
		if ($cate2 && $cate2 != 0) {
			$sql_l .= " AND category2='$cate2'";
			if ($cate3 && $cate3 != 0) {
				$sql_l .= " AND category3='$cate3'";
				}
			}
		}

	if ($sql_zaiko && $sql_l) { $sql_l = " AND $sql_l"; }

	if ($sql_zaiko || $sql_l) { $where = "where"; }


	$sql  = "SELECT zaiko_num FROM zaiko" .
			" $where $sql_zaiko $sql_l;";

	$sql1 = mysqli_query($conn_id,$sql);
	$count = mysqli_num_rows($sql1);
	$max = $count;
	if ($count <= 0) { echo ("登録されております商品は御座いません。<BR><BR>\n"); }
	else {

		$views = $VIEW_NUM[$view];
		$page_all = ceil($count / $views);
		if (!$page) { $page = 1; }
		$view_s = $views * ($page-1);
		$view_e = ($views * $page);
		if ($view_e >= $count) { $view_e = $count; }

	if ($ERROR) {
		$error = "";
		foreach ($ERROR AS $val) {
			if ($val != "") {
				$error .= "$val <BR>\n";
				}
			}
		if ($error) { $error .= "<BR>\n"; }
		}

		echo <<< ALPHA
$error
商品数 $count <BR>
<BR>
編集したい商品の、番号ボタンを押して下さい。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="hinban_" value="$hinban_">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
<INPUT type="hidden" name="touroku" value="$touroku">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="mode" value="hen">
<INPUT type="hidden" name="page" value="$page">
Page : $page / $page_all <BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>カテゴリー</TD>
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>サイズ</TD>
      <TD align='center'>金額</TD>
      <TD align='center'>在庫数</TD>
    </TR>
ALPHA;

		$sql =  "SELECT * FROM zaiko" .
				" $where $sql_zaiko $sql_l" .
				" ORDER BY hinban" .
				" LIMIT $view_e OFFSET $view_s;";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		$max_ = $view_e - $view_s;

		for($i=0; $i<$max_; $i++) {
			list($zaiko_num,$hinban,$title,$size,$category1,$category2,$category3,$money,$stock) = mysqli_fetch_array($sql1);

			$KATE2 = "KATE_$category1";
			$KATE3 = "KATE_$category1". "_$category2";

			echo <<<ALPHA
    <TR bgcolor="#FFFFFF">
      <TD align='center'>
      <INPUT type="submit" name="hen_num" value="$zaiko_num">
      </TD>
      <TD>$KATE[$category1] <BR> {${$KATE2}[$category2]} <BR> {${$KATE3}[$category3]}</TD>
      <TD>$hinban</TD>
      <TD>$title</TD>
      <TD align='center'>$size</TD>
      <TD align='right'>$money 円</TD>
      <TD align='center'>$stock</TD>
    </TR>
ALPHA;

			}

		echo <<<ALPHA
  </TBODY>
</TABLE>
</FORM>
ALPHA;

		if ($max > $view_e || $view_s >= $views) { next_p_z(); }

		}

	// mysqli_close($conn_id);

}



function hen() {
global $ca1,$ca2,$ca3,$ca1_r,$ca2_r,$hinban,$titles,$money,$stock,$size; //_POST @source write()
global $PHP_SELF,$ERROR;
global $main,$zaiko,$cate1,$cate2,$cate3,$view,$page,$touroku,$hen_num,$hinban,$hinban_; // _POST
include './array.inc';
include './list.inc';
include "../../cone.inc";
$hinban_=$_POST['hinban_'];
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];
$page=$_POST['page'];
$touroku=$_POST['touroku'];
$hen_num=$_POST['hen_num'];

	if (!$ERROR) {
		$sql = "SELECT * FROM zaiko WHERE zaiko_num='$hen_num';";
		$sql1 = mysqli_query($conn_id,$sql);
		$count = mysqli_num_rows($sql1);
		list($zaiko_num_m,$hinban_m,$title_m,$size_m,$category1_m,$category2_m,$category3_m,$money_m,$stock_m) = mysqli_fetch_array($sql1);
		}
	else {
		$error = "<FONT color=\"#ff0000\">\n";
		foreach ($ERROR AS $val) {
			$error .= "$val <BR>\n";
			}
		$error .= "<BR></FONT>\n";
		}

	if (!$ca1)		{ $ca1 = $category1_m; }
	if (!$ca2)		{ $ca2 = $category2_m; }
	if (!$ca3)		{ $ca3 = $category3_m; }
	if (!$hinban)	{ $hinban = $hinban_m; }
	if (!$titles)	{ $titles = $title_m; }
	if (!$size)		{ $size = $size_m; }
	if (!$money)	{ $money = $money_m; }
	if (!$stock)	{ $stock = $stock_m; }

	echo <<<ALPHA
$error
内容を、修正し変更ボタンを押して下さい。<BR>
但し、カテゴリーを選択していない場合は登録変更できません。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="hen">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="touroku" value="$touroku">
<INPUT type="hidden" name="hinban_" value="$hinban_">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="hen_num" value="$hen_num">
<INPUT type="hidden" name="page" value="$page">
カテゴリー変更、選択<BR>
<TABLE border="0">
  <TBODY>
    <TR>
      <TD>メインカテゴリー<BR><SELECT name='ca1'>
ALPHA;

	if ($ca1_r && $ca1 != $ca1_r) { $ca2 = ""; $ca3 = ""; }
	if ($ca2_r && $ca2 != $ca2_r) { $ca3 = ""; }

	if (!$ca1 || ($ca1 == 0)) { $select = "selected"; } else { $select = "";}
	echo ("		<OPTION value='' $select>$KATE[0]</OPTION>\n");
	$max = count($KATE);
	for ($i=1; $i<$max; $i++) {
		if ($ca1 == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$KATE[$i]</OPTION>\n");
			}
	echo ("      </SELECT>\n");
	echo ("      <INPUT type=\"hidden\" name=\"ca1_r\" value=\"$ca1\">\n");
	echo ("      </TD>\n");

	if ($ca1 && $ca1 != 0) {

		$KATE_N2 = "KATE_$ca1";

		echo ("      <TD>カテゴリー2<BR><SELECT name='ca2'>\n");
		if (!$ca2 || ($ca2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${"$KATE_N2"}[0]}</OPTION>\n");
		$max = count(${$KATE_N2});
		for ($i=1; $i<$max; $i++) {
			if ($ca2 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${"$KATE_N2"}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT>\n");
		echo ("      <INPUT type=\"hidden\" name=\"ca2_r\" value=\"$ca2\">\n");
		echo ("      </TD>\n");

		}

	if ($ca2 && $ca2 != 0) {

		$KATE_N3 = "KATE_$ca1" . "_$ca2";

		echo ("      <TD>カテゴリー3<BR><SELECT name='ca3'>\n");
		if (!$ca2 || ($ca2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${$KATE_N3}[0]}</OPTION>\n");
		$max = count(${$KATE_N3});
		for ($i=1; $i<$max; $i++) {
			if ($ca3 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${$KATE_N3}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT></TD>\n");

		}

	echo <<<ALPHA
      <TD valign="bottom">
      <INPUT type="submit" value="変更">
      </TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	$KA_1 = "$KATE[$ca1]";
	$KA_2 = "{${$KATE_N2}[$ca2]}";
	$KA_3 = "{${$KATE_N3}[$ca3]}";

	if (!$KA_3) { $KA_3 = "--"; }
	if (!$KA_2) { $KA_2 = "--"; }
	if (!$KA_1) { $KA_1 = "--"; }

	if (!$stock) { $stock = "0"; }

	if (!$ca3) { $input = "カテゴリーを設定して下さい。"; }
	else { $input = "<INPUT type=\"submit\" value=\"変更\">　<INPUT type=\"reset\">"; }

	echo <<<ALPHA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="write">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="touroku" value="$touroku">
<INPUT type="hidden" name="hinban_" value="$hinban_">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
<INPUT type="hidden" name="ca1" value="$ca1">
<INPUT type="hidden" name="ca2" value="$ca2">
<INPUT type="hidden" name="ca3" value="$ca3">
<INPUT type="hidden" name="ca1_r" value="$ca1">
<INPUT type="hidden" name="ca2_r" value="$ca2">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="hen_num" value="$hen_num">
<INPUT type="hidden" name="page" value="$page">
<TABLE border="0" bgcolor="#666666" cellpadding="4">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc" align="center">カテゴリー</TD>
      <TD bgcolor="#ffffff">$KA_1 &gt; $KA_2 &gt; $KA_3</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">商品番号</TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="hinban" value="$hinban"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">商品名</TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="titles" value="$titles"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">サイズ</TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="size" value="$size"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">金額</TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="money" value="$money"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">在庫数</TD>
      <TD bgcolor="#ffffff"><INPUT size="40" type="text" name="stock" value="$stock"></TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD colspan="2" align="center">$input</TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	echo <<<ALPHA

<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="touroku" value="$touroku">
<INPUT type="hidden" name="hinban_" value="$hinban_">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="submit" value="戻る">
</FORM>
ALPHA;

	exit();

}



function write() {
global $ERROR;
global $hen_num,$hinban,$titles,$size,$ca1,$ca2,$ca3,$money,$stock; //_POST
include "../../cone.inc";
$hen_num=$_POST['hen_num']; 
$ca1=$_POST['ca1']; 
$ca2=$_POST['ca2'];
$ca3=$_POST['ca3']; 
$hinban=$_POST['hinban']; 
$titles=$_POST['titles']; 
$money=$_POST['money']; 
$stock=$_POST['stock']; 
$size=$_POST['size']; 

	if ($hinban == "") { $ERROR[] = "商品番号が入力されておりません。"; }
	if ($titles == "") { $ERROR[] = "商品名が入力されておりません。"; }
//	if ($money == "") { $ERROR[] = "金額が入力されておりません。"; }
//	if ($stock == "") { $ERROR[] = "在庫数が入力されておりません。"; }

#	$money = mb_convert_kana($money,a,"EUC-JP");
	$money = mb_convert_kana($money,a,"UTF-8");
#	$stock = mb_convert_kana($stock,a,"EUC-JP");
	$stock = mb_convert_kana($stock,a,"UTF-8");

##	if (!ereg("[0-9]",$money)) { $ERROR[] = "入力された金額が不正な商品があります。"; }
	if (!preg_match("/[0-9]/",$money)) { $ERROR[] = "入力された金額が不正な商品があります。"; }
##	if (!ereg("[0-9]",$stock)) { $ERROR[] = "入力された在庫数が不正な商品があります。"; }
	if (!preg_match("/[0-9]/",$stock)) { $ERROR[] = "入力された在庫数が不正な商品があります。"; }

if ($ERROR) { hen(); }

	$sql  = "UPDATE zaiko set".
			" hinban='$hinban'," .
			" title='$titles'," .
			" size='$size'," .
			" category1='$ca1'," .
			" category2='$ca2'," .
			" category3='$ca3'," .
			" money='$money'," .
			" stock='$stock'" .
			" where zaiko_num='$hen_num';";

	$sql1 = mysqli_query($conn_id,$sql);

	$ERROR[] = "変更いたしました。";

	hen();

}

?>
