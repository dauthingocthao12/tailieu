<?PHP

function zaiko_4() {
global $mode,$view;
$mode=$_POST['mode'];
$view=$_POST['view'];
	if ($mode == "del") { del(); }
	if ($view && $view!=0) { first_00(); }

	exit();

}



function first_00() {
global $main,$zaiko,$view,$hinban_,$cate1,$cate2,$cate3,$page; //_POST
global $PHP_SELF,$ERROR,$max,$view_s,$view_e,$views;
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
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
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
削除したい商品の、番号ボタンを押して下さい。<BR>
確認無しで消えてしまいますので気をつけてボタンをおして下さい。<BR>
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
<INPUT type="hidden" name="mode" value="del">
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
				" WHERE category3!='' $sql_l" .
				" ORDER BY hinban" .
				" LIMIT $view_e OFFSET $view_s;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		$max_ = $view_e - $view_s;

		for($i=0; $i<$max_; $i++) {
			list($zaiko_num,$hinban,$title,$size,$category1,$category2,$category3,$money,$stock) = pg_fetch_array($sql1,$i);

			$KATE2 = "KATE_$category1";
			$KATE3 = "KATE_$category1". "_$category2";

			echo <<<ALPHA
    <TR bgcolor="#FFFFFF">
      <TD align='center'>
      <INPUT type="submit" name="del_num" value="$zaiko_num">
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

	pg_close($conn_id);

}



function del() {
global $$del_num; //_POST?
include './array.inc';
include './list.inc';
include "../../cone.inc";
$del_num=$_POST['del_num'];
	$sql = "DELETE FROM zaiko WHERE zaiko_num='$del_num';";
	$sql1 = pg_exec($conn_id,$sql);
}

?>
