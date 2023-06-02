<?PHP

function zaiko_1() {
global $mode,$view; //_POST
$mode=$_POST['mode'];
$view=$_POST['view'];
	if ($mode == "hen") { hen(); }
	if ($view && $view!=0) { first_00(); }

	exit();

}



function first_00() {
global $PHP_SELF,$view_s,$view_e,$views,$max,$ERROR;
global $main,$zaiko,$hinban_,$cate1,$cate2,$cate3,$view,$page; //_POST


include './array.inc';
include './list.inc';
include "../../cone.inc";
$hinban_=$_POST['hinban_'];
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];
$page=$_POST['page'];

	if ($hinban_) {
		$sql_l .= " AND hinban LIKE '%$hinban_%'";
		}
	elseif ($cate1 && $cate1 != 0) {
		$sql_l .= " AND category1='$cate1'";
		if ($cate2 && $cate2 != 0) {
			$sql_l .= " AND category2='$cate2'";
			if ($cate3 && $cate3 != 0) {
				$sql_l .= " AND category3='$cate3'";
				}
			}
		}

	$sql  = "SELECT zaiko_num FROM zaiko" .
			" WHERE category3!='' $sql_l;";
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
在庫数を変更する場合、半角数字で入力し、数量変更ボタンを押して下さい。<BR>
Page : $page / $page_all <BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>カテゴリー</TD>
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>サイズ</TD>
      <TD align='center'>金額</TD>
      <TD align='center'>在庫数</TD>
    </TR>
ALPHA;

		$sql =  "SELECT * FROM zaiko" .
				" WHERE category3!='' $sql_l" .
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
      <TD>$KATE[$category1] <BR> {${$KATE2}[$category2]} <BR> {${$KATE3}[$category3]}</TD>
      <TD>$hinban</TD>
      <TD>$title</TD>
      <TD align='center'>$size</TD>
      <TD align='right'>$money 円</TD>
      <TD align='center'>
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
      <INPUT type="hidden" name="view" value="$view">
      <INPUT type="hidden" name="mode" value="hen">
      <INPUT type="hidden" name="page" value="$page">
      <INPUT type="hidden" name="zaiko_num" value="$zaiko_num">
      <INPUT type="text" size="6" name="stock" value="$stock"><BR>
      <INPUT type="submit" value="数量変更">
      </TD></FORM>
    </TR>
ALPHA;

			}

		echo <<<ALPHA
  </TBODY>
</TABLE>
ALPHA;
		if ($max > $view_e || $view_s >= $views) { next_p_z(); }

		}

	// mysqli_close($conn_id);

}



function hen() {
global $zaiko_num, $stock; //_POST
global $PHP_SELF,$ERROR;
include './array.inc';
include './list.inc';
include "../../cone.inc";
$zaiko_num=$_POST['zaiko_num'];
$stock=$_POST['stock'];

#	$stock = mb_convert_kana($stock,a,"EUC-JP");
	$stock = mb_convert_kana($stock,a,"UTF-8");

	if (!$zaiko_num || $stock == "") { $ERROR[] = "在庫番号又は在庫数が入力されておりません。"; }
##	if (!ereg("[0-9]",$stock)) { $ERROR[] = "入力された在庫数が不正です。"; }
	if (!preg_match("/[0-9]/",$stock)) { $ERROR[] = "入力された在庫数が不正です。"; }
	if ($ERROR) {
	first_00();
	exit();
	}

	$sql  = "UPDATE zaiko set".
			" stock='$stock'" .
			" where zaiko_num='$zaiko_num';";

	$sql1 = mysqli_query($conn_id,$sql);

}

?>
