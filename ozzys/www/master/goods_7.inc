<?PHP
//	売れ筋表示
function goods_7() {
global $PHP_SELF,$main,$goods,$maker,$bunrui,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e,$view,$main_r,$goods_r,$mode,$page;
include './array.inc';
include "../../cone.inc";

if ($view) {
		first_00();
	}

}



//	詳細一覧
function first_00() {
    global $PHP_SELF,$main,$goods,$maker,$bunrui,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;
    global $view, $views, $view_e, $view_s, $max, $main_r, $goods_r, $mode, $page;
    include './array.inc';
    include "../../cone.inc";

	$mon_s_ = sprintf("%02d",$mon_s);
	$day_s_ = sprintf("%02d",$day_s);
	$mon_e_ = sprintf("%02d",$mon_e);
	$day_e_ = sprintf("%02d",$day_e);

	$data_s = "$year_s-$mon_s_-$day_s_";
	$data_e = "$year_e-$mon_e_-$day_e_";

	if ($maker || $bunrui) {
		$sql  = "select i.list_num, sum(i.buy_num) as buy_nums from list d, total i where";
		$sql .= " d.list_num=i.list_num";
		if ($maker) {
			$sql .= " AND d.maker_num='$maker'";
			}
		if ($bunrui) {
			$sql .= " AND d.class_m='$bunrui'";
			}
		$sql .= " AND i.s_date BETWEEN timestamp '$data_s' AND timestamp '$data_e'";
		$sql .= " GROUP BY i.list_num ORDER BY buy_nums DESC;";
		}
	else {
		$sql = "select list_num, sum(buy_num) as buy_nums from total where";
		$sql .= " s_date BETWEEN timestamp '$data_s' AND timestamp '$data_e'";
		$sql .= " GROUP BY list_num ORDER BY buy_nums DESC;";
		}

	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

    if (!$page) { $page = 1; }
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	$max = $count-1;
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;
    if ($view_e > $max) { $view_e = $max; }

	if ($count < 1) { echo ("販売された商品はありません。<BR><BR>\n"); }
	if ($count >= 1) {

		echo ("販売商品点数 $count <BR>\n<BR>\n");

echo <<< OZZYS
<BR>
Page : $page / $page_all <BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>順位</TD>
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>メーカー名</TD>
      <TD align='center'>色</TD>
      <TD align='center'>サイズ</TD>
      <TD align='center'>販売数</TD>
    </TR>
OZZYS;

		$number = 0;
		$b_num = 0;
		for ($i=$view_s; $i<=$view_e; $i++) {
			list($list_num,$buy_num_s) = pg_fetch_array($sql1,$i);
			$num = $num + 1;
			$sql2  = "select d.goods_name, b.maker_name, d.color, d.size from maker b, list d where";
			$sql2 .= " b.maker_num=d.maker_num";
			$sql2 .= " AND d.list_num='$list_num';";
			$sql2_ = pg_exec($conn_id,$sql2);
			@list($goods_name,$maker_name,$color,$size) = pg_fetch_array($sql2_,0);
			if ($buy_num_s != $b_num) { $number = $num; }
			$b_num = $buy_num_s;

echo <<< OZZYS
    <TR bgcolor="#ffffff">
      <TD align='center'>$number</TD>
      <TD align='center'>$list_num</TD>
      <TD align='center'>$goods_name</TD>
      <TD align='center'>$maker_name</TD>
      <TD align='center'>$color</TD>
      <TD align='center'>$size</TD>
      <TD align='center'>$buy_num_s</TD>
    </TR>
OZZYS;

			}

	if ($max > $view_e || $view_s >= $views) { next_p_g_2(); }

		}

	pg_close($conn_id);

}



?>
