<?PHP
//	売れ筋表示
function order_4() {
global $view; //_PSOT
include './array.inc';
include "../../cone.inc";
$view=$_POST['view']?:1;
if ($view) {
	first_00();
	}

}


//	詳細一覧
function first_00() {
include './array.inc';
include "../../cone.inc";
global $max,$view_s,$view_e,$views,$number,$b_num;
global $year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e,$view,$page; //_POST

 	$year_s=$_POST['year_s']?:2003;
	$mon_s=$_POST['mon_s']?:1;
	$day_s=$_POST['day_s']?:1;
	$year_e=$_POST['year_e']?:date("Y");
	$mon_e=$_POST['mon_e']?:date("m");
	$day_e=$_POST['day_e']?:date("d");
	$page=$_POST['page'];
	// $number=$_POST['number']; //　_POST	ではない　未確認　2022/11/15
	// $b_num=$_POST['b_num']; // _POST	ではない　未確認　2022/11/15

	$mon_s_ = sprintf("%02d",$mon_s);
	$day_s_ = sprintf("%02d",$day_s);
	$mon_e_ = sprintf("%02d",$mon_e);
	$day_e_ = sprintf("%02d",$day_e);

	$data_s = "$year_s-$mon_s_-$day_s_";
	$data_e = "$year_e-$mon_e_-$day_e_";

	$sql = "select hinban,  sum(buy_n) as buy_nums from total where";
	$sql .= " s_date BETWEEN timestamp '$data_s' AND timestamp '$data_e' AND hinban!='option'";
	$sql .= " GROUP BY hinban ORDER BY buy_nums DESC";

	$sql1 = mysqli_query($conn_id,$sql);
	$count = mysqli_num_rows($sql1);
	$max = $count-1;
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;

	if ($view_e > $max) { $view_e = $max; }
	if ($count < 1) { echo ("販売された商品はありません。<BR><BR>\n"); }
	if ($count >= 1) {

		echo ("販売商品点数 $count <BR>\n<BR>\n");

echo <<< ALPHA
Page : $page / $page_all <BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>順位</TD>
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>販売数</TD>
    </TR>
ALPHA;

		if (!$number) { $number = 0; }
		if (!$b_num) { $b_num = 0; }
		if ($page != 1) { $num = $views * ($page-1); }
		$limit=$view_e-$view_s+1;
		$sql.=" LIMIT ".$limit." OFFSET ".$view_s.";";
		$sql1 = mysqli_query($conn_id,$sql);
		for ($i=0; $i<$limit; $i++) {
			list($hinban,$buy_num_s) = mysqli_fetch_array($sql1);
			$num = $num + 1;
			$sql2 = "SELECT title FROM sells WHERE hinban='$hinban' LIMIT 1;";
			$sql2_ = mysqli_query($conn_id,$sql2);
			list($title) = mysqli_fetch_array($sql2_);
			if ($buy_num_s != $b_num) { $number = $num; }
			$b_num = $buy_num_s;

echo <<< ALPHA
    <TR bgcolor="#ffffff">
      <TD align='center'>$number</TD>
      <TD>$hinban</TD>
      <TD>$title</TD>
      <TD align='center'>$buy_num_s</TD>
    </TR>
ALPHA;

			}

echo <<<ALPHA
  </TBODY>
</TABLE>
<BR>
ALPHA;
	if ($max > $view_e || $view_s >= $views) {next_p_o_2(); }

		}

	// mysqli_close($conn_id);

}



?>
