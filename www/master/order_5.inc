<?PHP
//	発注状況
function order_5() {
	first_01();
}


function first_01() {
global $mode,$csv_file;//_POST
global $ERROR;
	$mode=$_POST['mode'];
	$csv_file=$_POST['csv_file']; // 未確認　2022/11/15 未使用可能高

echo ("売り上げ料金表示<BR>\n<BR>\n");

	if ($csv_file) {
#		$csv_file	= mb_convert_kana($csv_file,"rn","EUC-JP");
		$csv_file	= mb_convert_kana($csv_file,"rn","UTF-8");
##		$csv_file = ereg_replace("　","",$csv_file);
		$csv_file = preg_replace("/　/","",$csv_file);
##		$csv_file = ereg_replace(" ","",$csv_file);
		$csv_file = preg_replace("/ /","",$csv_file);
		if ($csv_file == "") { $ERROR = "保存ファイル名が入力されておりません。"; }
##		if ($csv_file && (ereg("[^a-zA-Z0-9_]",$csv_file))) { $ERROR = "保存ファイル名が不正です。"; }
		if ($csv_file && (preg_match("/[^a-zA-Z0-9_]/",$csv_file))) { $ERROR = "保存ファイル名が不正です。"; }
	}

	if (!$mode || $ERROR) { first_02_1(); }
	elseif ($mode == "make") { first_02_2(); }

}


function first_02_1() {
global $main,$order,$csv; //_POST
global $PHP_SELF,$ERROR;
include './array.inc';

	if ($ERROR) {

		echo <<<ALPHA
<FONT color="#ff0000">$ERROR</FONT><BR>
<BR>
ALPHA;

	}

	echo <<<ALPHA
表示したいリストを選択し表示ボタンを押して下さい。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="make">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="order" value="$order">
<INPUT type="hidden" name="csv" value="$csv">
<TABLE border="0" bgcolor="#666666">
  <TBODY>
    <TR>
      <TD align="center" bgcolor="#cccccc">納品状態</TD>
      <TD bgcolor="#ffffff"><SELECT name="send">
ALPHA;

	$max = count($HASOU_N);

	for($i=0; $i<$max; $i++) {
		$selected = "";
		if (!$send && $i == 5) { $selected = "selected"; }
		elseif ($send == $i) { $selected = "selected"; }
		echo ("        <OPTION value=\"$i\" $selected>$HASOU_N[$i]</OPTION>\n");
	}

	echo <<<ALPHA
      </SELECT></TD>
    </TR>
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

}


function data() {
include './array.inc';
//global $year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;　//必要ではない 2022/11/15
$year_s=$_POST['year_s']; //ToDo check
$mon_s=$_POST['mon_s']; //ToDo check
$day_s=$_POST['day_s']; //ToDo check
$year_e=$_POST['year_e']; //ToDo check
$mon_e=$_POST['mon_e']; //ToDo check
$day_e=$_POST['day_e']; //ToDo check


	$year_t = date("Y");
	$mon_t = date("n");
	$day_t = date("d");
//	if (!$year_s) { $year_s = 2003; }
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



function first_02_2() {
// global $send,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;　//必要ではない 2022/11/15
include "../../cone.inc";
include './array.inc';
include '../sub/array.inc';
$send=$_POST['send']; 
$year_s=$_POST['year_s']; 
$mon_s=$_POST['mon_s']; 
$day_s=$_POST['day_s']; 
$year_e=$_POST['year_e']; 
$mon_e=$_POST['mon_e']; 
$day_e=$_POST['day_e']; 

	$check = 0;
	$year_t = date("Y");
	$mon_t = date("n");
	$day_t = date("d");
	if ($year_s == 2003 && $mon_s == 1 && $day_s == 1 && $year_e == $year_t && $mon_e == $mon_t && $day_e == $day_t) { $check = 1; }
	elseif ($year_s == 2003 && $mon_s == 1 && $day_s == 1) { $check = 2; }

	$year_s  = substr($year_s,2,2);
	$year_s_ = sprintf("%02d",$year_s);
	$mon_s_  = sprintf("%02d",$mon_s);
	$day_s_  = sprintf("%02d",$day_s);
	$year_e  = substr($year_e,2,2);
	$year_e_ = sprintf("%02d",$year_e);
	$mon_e_  = sprintf("%02d",$mon_e);
	$day_e   = $day_e + 1;
	$day_e_  = sprintf("%02d",$day_e);

	$data_s = "$year_s_$mon_s_$day_s_" . "000000";
	$data_e = "$year_e_$mon_e_$day_e_" . "000000";

	$sql = "select * from sells";
	if ($send != 5 || $check == 0) {
	$sql .= " where ";
	}
	if ($send <= 3) {
		$sql .= " send='$send'";
	}
	elseif ($send == 4) {
		$sql .= " send!='2'";
	}
	if ($send <= 4 && $check != 1) {
		$sql .= " AND";
	}
	if ($check == 0) {
		$sql .= " sells_num>='$data_s' AND sells_num<'$data_e'";
	}
	if ($check == 2) {
		$sql .= " sells_num<'$data_e'";
	}
	$sql .= " ORDER BY sells_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$max = pg_numrows($sql1);


	$sel_n = "";
	$bargain_n = 0;
	$all_price = 0;
	for($i=0; $i<$max; $i++) {
		list($sells_num,$kojin_num,$add_num,$hinban,$title,$price,$buy_n,$send,$h_time,$bargain) = pg_fetch_array($sql1,$i);

		$a_price = $price * $buy_n;

		$op_list = "";
		if ($hinban == "option") {

			$sql_o = "SELECT * FROM option WHERE sells_num='$sells_num' AND option_num='$title';";
			$sql_o_ = pg_exec($conn_id,$sql_o);

			list($option_num,$sells_num,$kojin_num,$hinban,$title,$seban_l,$seban_num,$seban_price,$sename_l,$sename_name,$sename_price,$muneban_l,$muneban_num,$muneban_price,$pant_l,$pant_num,$pant_price,$bach_l,$bach_name,$bach_price,$send,$h_time) = pg_fetch_array($sql_o_,0);

			if ($hinban == "mochikomi") {
				$a_price = 1000;
			}

			if ($seban_l) {
				$moji_num = strlen($seban_num);
				$kakaku = $SEBAN_P_N[$seban_l];
				$a_price += $kakaku * $moji_num;
			}
			if ($sename_l) {
				$moji_num = strlen($sename_name);
				$kakaku = $SENAME_P_N[$sename_l];
				$a_price += $kakaku * $moji_num;
			}
			if ($muneban_l) {
				$moji_num = strlen($muneban_num);
				$kakaku = $MUNEBAN_P_N[$muneban_l];
				$a_price += $kakaku * $moji_num;
			}
			if ($pant_l) {
				$moji_num = strlen($pant_num);
				$kakaku = $PANT_P_N[$pant_l];
				$a_price += $kakaku * $moji_num;
			}
			if ($bach_l) {
				$a_price += $BACH_P_N[$bach_l];
			}

		}
		if ($sel_n != $sells_num) {

			if ($i != 0) {
				if ($bargain_n > 0) {
					$wari_price = 0;
					$wari_price = $all_price * $bargain_n / 100;
					$wari_price = floor($wari_price);
					$all_price = $all_price - $wari_price;
				}
				$all += $all_price;
				$all_price = 0;
			}

			$all_price = $all_price + $a_price;

		}
		else {
			$all_price = $all_price + $a_price;
		}

		$sel_n = $sells_num;
		$bargain_n = $bargain;

	}

	if ($bargain_n > 0) {
		$wari_price = 0;
		$wari_price = $all_price * $bargain_n / 100;
		$wari_price = floor($wari_price);
		$all_price = $all_price - $wari_price;
	}
	$all += $all_price;

	$a_price = number_format($all);

	echo <<<ALPHA
金額：$a_price 円<BR>
ALPHA;

}

?>
