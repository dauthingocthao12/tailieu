<?PHP
//	商品クリック履歴
function order_6() {

	first_01();

}


function first_01() {
global $main,$order,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e,$action; //_POST
global $PHP_SELF,$ERROR;
$year_s=$_POST['year_s']; 
$mon_s=$_POST['mon_s']; 
$day_s=$_POST['day_s']; 
$year_e=$_POST['year_e']; 
$mon_e=$_POST['mon_e']; 
$day_e=$_POST['day_e']; 
$action=$_POST['action'];

include "../../cone.inc";
include './array.inc';
include '../sub/array.inc';


echo ("商品クリック履歴<BR>\n");

	echo <<<ALPHA
表示したい期間を選択し表示ボタンを押して下さい。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="make">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="order" value="$order">
<INPUT type="hidden" name="action" value="check">
<TABLE border="0" bgcolor="#666666">
  <TBODY>
    <TR>
      <TD align="center" bgcolor="#cccccc">期間</TD>
      <TD bgcolor="#ffffff">
ALPHA;

	data();

	echo <<<ALPHA
      </TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#ffffff" align="center"><INPUT type="submit" value="表示">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	if ($year_s && $mon_s && $day_s && $year_e && $mon_e && $day_e) {
		if (!checkdate($mon_s,$day_s,$year_s)) { $ERROR = "開始日が不正です。"; }
		if (!checkdate($mon_e,$day_e,$year_e)) { $ERROR = "終了日が不正です。"; }
	}

	if (!$ERROR && $action == "check") {
//		$year_s  = substr($year_s,2,2);
		$year_s_ = sprintf("%04d",$year_s);
		$mon_s_  = sprintf("%02d",$mon_s);
		$day_s_  = sprintf("%02d",$day_s);
//		$year_e  = substr($year_e,2,2);
		$year_e_ = sprintf("%04d",$year_e);
		$mon_e_  = sprintf("%02d",$mon_e);
		$day_e   = $day_e + 1;
		$day_e_  = sprintf("%02d",$day_e);
		$e_date  = mktime(0,0,0,$mon_e_,$day_e_,$year_e_);

		$data_s = $year_s_ . "-" . $mon_s_ . "-" . $day_s_;
		$data_e = date("Y-m-d",$e_date);

		$sql = "select * from cart_access";
		$sql .= " WHERE time>='$data_s' AND time<'$data_e'";
		$sql .= " ORDER BY time;";
		$sql1 = mysqli_query($conn_id,$sql);
		$max = mysqli_num_rows($sql1);
		if ($max > 0) {
			echo <<<WAKABA
クリック数：{$max}件<br>
<table border="0" cellspacing="1" bgcolor="#666666">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>商品コード</td>
      <td>商品名</td>
      <td>価格</td>
      <td>IP</td>
      <td>ホスト</td>
      <td>時間</td>
    </tr>
WAKABA;

			while ($list = mysqli_fetch_array($sql1)) {
				$code = $list['code'];
				$name = $list['name'];
				$kakaku = $list['kakaku'];
				$ip = $list['ip'];
				$host = $list['host'];
				$time = $list['time'];

				echo <<<WAKABA
    <tr bgcolor="#ffffff">
      <td>$code</td>
      <td>$name</td>
      <td>$kakaku</td>
      <td>$ip</td>
      <td>$host</td>
      <td>$time</td>
    </tr>
WAKABA;
			}

			echo<<<WAKABA
  </tbody>
</table>

WAKABA;

		}
		else {
			echo <<<WAKABA
<BR>
履歴はありません。<BR>

WAKABA;
		}

	}
	else {
		echo <<<WAKABA
<BR>
$ERROR<BR>

WAKABA;
	}

}


function data() {
include './array.inc';
global $year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;

$year_t = date("Y");
$mon_t = date("n");
$day_t = date("d");

//	if (!$year_s) { $year_s = 2006; }
//	if (!$mon_s) { $mon_s = 1; }
//	if (!$day_s) { $day_s = 1; }
	if (!$year_s) { $year_s = $year_t; }
	if (!$mon_s) { $mon_s = $mon_t; }
	if (!$day_s) { $day_s = $day_t; }
	if (!$year_e) { $year_e = $year_t; }
	if (!$mon_e) { $mon_e = $mon_t; }
	if (!$day_e) { $day_e = $day_t; }

	echo ("      <SELECT name='year_s'>\n");
	for ($i=2003; $i<=$year_t; $i++) {
		if ($year_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>年<SELECT name='mon_s'>\n");
	for ($i=1; $i<=12; $i++) {
		if ($mon_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>月<SELECT name='day_s'>\n");
	for ($i=1; $i<=31; $i++) {
		if ($day_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>日～<SELECT name='year_e'>\n");
	for ($i=2003; $i<=$year_t; $i++) {
		if ($year_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>年<SELECT name='mon_e'>\n");
	for ($i=1; $i<=12; $i++) {
		if ($mon_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>月<SELECT name='day_e'>\n");
	for ($i=1; $i<=31; $i++) {
		if ($day_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
		}
	echo ("      </SELECT>日\n");

}
?>
