<?PHP
//	発注状況
function order_3() {
global $csv; //_POST
$csv=$_POST['csv'];
	if ($csv == 1) { first_01(); }
	elseif ($csv == 2) { first_02(); }
	elseif ($csv == 3) { first_03(); } //　未確認　2022/11/15
}


function first_01() {

	echo ("作成ファイルリスト<BR>\n<BR>\n");

	$dir_m = "./order";

	$strDir = opendir($dir_m);

	$order_list = "";
	while ($strFle = readdir($strDir)) {
##		if (!ereg("^\.",$strFle)) {
		if (!preg_match("/^\./",$strFle)) {
			$order_list .= "・ <A href=\"./order/$strFle\" target=\"_blank\">$strFle</A><BR>\n";
		}
	}

	if (!$order_list) { echo ("現在はリストは御座いません。"); }
	else {

		echo <<<ALPHA
データーを保存したい場合は、右クリックでファイルを選択し<BR>
”対象をファイルに保存”を選択しダウンロードして下さい。<BR>
<BR>
$order_list
ALPHA;

	}

	closedir($strDir);

}


function first_02() {
global $mode,$csv_file; //_POST
global $ERROR;
$mode=$_POST['mode'];
$csv_file=$_POST['csv_file'];

echo ("ファイル作成<BR>\n<BR>\n");

	if (array_key_exists('csv_file', $_POST)) {
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
global $PHP_SELF,$ERROR;
global $main,$order,$csv;//_POST

include './array.inc';

	if ($ERROR) {
		echo <<<ALPHA
<FONT color="#ff0000">$ERROR</FONT><BR>
<BR>
ALPHA;

	}

	echo <<<ALPHA
作成したいリストを選択し、作成ファイル名を入力し、<BR>
作成ボタンを押して下さい。<BR>
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
		<TD align="center" bgcolor="#cccccc">作成ファイル名（半角英数字）</TD>
		<TD bgcolor="#ffffff"><INPUT size="20" type="text" name="csv_file" value="">.csv</TD>
	</TR>
	<TR>
		<TD colspan="2" bgcolor="#ffffff" align="center"><INPUT type="submit" value="作成">　<INPUT type="reset"></TD>
	</TR>
	</TBODY>
</TABLE>
</FORM>
ALPHA;
}


function data() {
include './array.inc';
// global $year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;

	$year_s=$_POST['year_s'];//ToDo check
	$mon_s=$_POST['mon_s'];//ToDo check
	$day_s=$_POST['day_s'];//ToDo check
	$year_e=$_POST['year_e'];//ToDo check
	$mon_e=$_POST['mon_e'];//ToDo check
	$day_e=$_POST['day_e'];//ToDo check

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
global $send,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e,$csv_file;

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


	$SHIHARAI_N = $SHIHARAI_N1;

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

	$list = "\"注文日\",\"注文番号\",\"会員番号\",\"お名前\",\"ふりがな\",\"郵便番号\",\"住所\",\"電話番号\",\"FAX番号\",\"メールアドレス\",\"在庫なき場合\",\"支払方法\",\"メッセージ\",\"ポイント利用\",\"状態\",\"商品番号\",\"商品名\",\"購入数\",\"金額\"\n";


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


			$buy_n = "1";
			$a_price = "---";
			if ($hinban == "mochikomi") {
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"持ち込み手数料\",\"1\",\"1000\"\n";
			}
			$hinban = "マーキング";
			if ($seban_l) {
				$moji_num = strlen($seban_num);
				$kakaku = $SEBAN_P_N[$seban_l];
				$syoukei = $kakaku * $moji_num;
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"背番号 : $SEBAN_N[$seban_l] : ($seban_num)\",\"$moji_num\",\"$syoukei\"\n";
			}
			if ($sename_l) {
				$moji_num = strlen($sename_name);
				$kakaku = $SENAME_P_N[$sename_l];
				$syoukei = $kakaku * $moji_num;
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"背ネーム : $SENAME_N[$sename_l] : ($sename_name)\",\"$moji_num\",\"$syoukei\"\n";
			}
			if ($muneban_l) {
				$moji_num = strlen($muneban_num);
				$kakaku = $MUNEBAN_P_N[$muneban_l];
				$syoukei = $kakaku * $moji_num;
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"胸番号 : $MUNEBAN_N[$muneban_l] : ($muneban_num)\",\"$moji_num\",\"$syoukei\"\n";
			}
			if ($pant_l) {
				$moji_num = strlen($pant_num);
				$kakaku = $PANT_P_N[$pant_l];
				$syoukei = $kakaku * $moji_num;
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"パンツ番号 : $PANT_N[$pant_l] : ($pant_num)\",\"$moji_num\",\"$syoukei\"\n";
			}
			if ($bach_l) {
				$syoukei = $BACH_P_N[$bach_l];
				$op_list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"バッジ : $BACH_N[$bach_l]\",\"1\",\"$syoukei\"\n";
			}

		}
		if ($sel_n != $sells_num) {

			if ($i != 0) {
				if ($bargain_n != 0) {
					$wari_price = 0;
					$wari_price = $all_price * $bargain_n / 100;
					$wari_price = floor($wari_price);
					$list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"割引( $bargain_n %)\",\"\",\"-$wari_price\"\n";
				}

				$all_price = 0;
				$list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\"\n";
			}

			$sql2 = "SELECT * FROM add WHERE add_num='$add_num'";
			$sql2_ = pg_exec($conn_id,$sql2);

			list($add_num,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$zaiko,$siharai,$t_time,$msr,$g_point,$toi_num) = pg_fetch_array($sql2_,0);

			$y_ = substr($sells_num,0,2);
			$y_ = $y_ + 2000;
			$m_ = substr($sells_num,2,2);
			$d_ = substr($sells_num,4,2);
			$hinichi = "$y_/$m_/$d_";

			$list .= "\"$hinichi\",\"$sells_num\",\"$kojin_num\",\"$name_s $name_n\",\"$kana_s,$kana_n\",\"$zip1-$zip2\",\"$PRF_N[$prf]$city$add1 $add2\",\"$tel1-$tel2-$tel3\",\"$fax1-$fax2-$fax3\",\"$email\",\"$ZAIKO_N[$zaiko]\",\"$SHIHARAI_N[$siharai]\",\"$msr\",\"$g_point\",\"$HASOU_N[$send]\",\"$hinban\",\"$title\",\"$buy_n\",\"$a_price\"\n";

			$all_price = $all_price + $a_price;
			if ($op_list) { $list .= $op_list; }

		}
		else {

			$list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"$hinban\",\"$title\",\"$buy_n\",\"$a_price\"\n";

			$all_price = $all_price + $a_price;
			if ($op_list) { $list .= $op_list; }
		}

		$sel_n = $sells_num;
		$bargain_n = $bargain;

	}

	if ($bargain_n > 0) {
		$wari_price = 0;
		$wari_price = $all_price * $bargain_n / 100;
		$wari_price = floor($wari_price);
		$list .= "\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"割引( $bargain_n %)\",\"\",\"-$wari_price\"\n";
	}

#	$list = mb_convert_encoding($list,"SJIS","EUC");
	$list = mb_convert_encoding($list,"SJIS","UTF-8");

	$file_names = "$csv_file.csv";
	$files = "./order/$file_names";
	$file = fopen("$files","w+");
	fwrite($file,$list);
	fclose($file);
	chmod($files,0666);

	echo <<<ALPHA
ファイル名：$file_names <BR>
を作成しました。
ALPHA;

}



function first_03() {
global $del_file,$mode; // _POST?
global $ERROR;
$mode=$_POST['mode']; //未確認　2022/11/15
$del_file=$_POST['del_file'];//未確認　2022/11/15

echo ("ファイル削除<BR>\n<BR>\n");

	if ($mode) {
		if ($del_file == "") { $ERROR = "削除するファイルが選択されておりません。"; }
	}

	if (!$mode || $ERROR) { first_03_1(); }
	elseif ($mode == "del") { first_03_2(); }

}



function first_03_1() {
global $PHP_SELF,$ERROR;
global $main,$order,$csv; // _POST

	if ($ERROR) {

		echo <<<ALPHA
<FONT color="#ff0000">$ERROR</FONT><BR>
<BR>
ALPHA;

	}

	$dir_m = "./order";

	$strDir = opendir($dir_m);

	$del_list = "";
	while ($strFle = readdir($strDir)) {
##		if (!ereg("^\.",$strFle)) {
		if (!preg_match("/^\./",$strFle)) {
			$del_list .= "<INPUT type=\"checkbox\" name=\"del_file[]\" value=\"$strFle\"> : $strFle<BR>\n";
		}
	}

	if (!$del_list) { echo ("現在はリストは御座いません。"); }
	else {

		echo <<<ALPHA
削除したいファイルにチェックを入れ<BR>
”削除”を押して下さい。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="del">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="order" value="$order">
<INPUT type="hidden" name="csv" value="$csv">
$del_list
<BR>
<INPUT type="submit" value="削除">　<INPUT type="reset">
</FORM>
ALPHA;

	}

	closedir($strDir);

}



function first_03_2() {
global $del_file;

	echo <<<ALPHA
以下のファイルを削除致しました。<BR>
<BR>
ALPHA;

	foreach ($del_file AS $val) {
		$files = "./order/$val";
		unlink($files);
		echo ("・$val <BR>\n");
	}

}



?>
