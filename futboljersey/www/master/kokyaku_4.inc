<?PHP
//	顧客CSV作成
function kokyaku_4() {
// global $PHP_SELF,$main,$kokyaku,$csv;
global $PHP_SELF;
$csv=$_POST['csv'];
if ($csv == 1) { first_01(); }
elseif ($csv == 2) { first_02(); }
elseif ($csv == 3) { first_03(); }

}

function first_01() {

echo ("作成ファイルリスト<BR>\n<BR>\n");

$dir_m = "./member";

	$strDir = opendir($dir_m);
	$member_list = "";
	while ($strFle = readdir($strDir)) {
##		if (!ereg("^\.",$strFle)) {
		if (!preg_match("/^\./",$strFle)) {
			$member_list .= "・ <A href=\"./member/$strFle\" target=\"_blank\">$strFle</A><BR>\n";
			}
		}

	if (!$member_list) { echo ("現在はリストは御座いません。"); }
	else {

		echo <<<ALPHA
データーを保存したい場合は、右クリックでファイルを選択し<BR>
”名前を付けてリンクを保存”を選択しダウンロードして下さい。<BR>
<BR>
$member_list
ALPHA;

		}

	closedir($strDir);

}



function first_02() {
// global $PHP_SELF,$main,$kokyaku,$csv,$mode,$csv_file,$ERROR;
global $PHP_SELF,$ERROR;
$mode=$_POST['mode'];
$csv_file=$_POST['csv_file'];
echo ("ファイル作成<BR>\n<BR>\n");

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
// global $PHP_SELF,$main,$kokyaku,$csv,$ERROR,$csv_file;
global $PHP_SELF,$ERROR;
include './array.inc';
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$csv=$_POST['csv'];
$csv_file=$_POST['csv_file'];
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
<INPUT type="hidden" name="kokyaku" value="$kokyaku">
<INPUT type="hidden" name="csv" value="$csv">
<TABLE border="0" bgcolor="#666666">
  <TBODY>
    <TR>
      <TD align="center" bgcolor="#cccccc">会員</TD>
      <TD bgcolor="#ffffff">
       <SELECT name="member">
ALPHA;

	$max = count($MENBER_N);

	for($i=0; $i<$max; $i++) {
		$selected = "";
		if ($i == 3) { $selected = "selected"; }
		echo ("        <OPTION value=\"$i\" $selected>$MENBER_N[$i]</OPTION>\n");
		}

	echo <<<ALPHA
       </SELECT>
      </TD>
    </TR>
    <TR>
      <TD align="center" bgcolor="#cccccc">都道府県</TD>
      <TD bgcolor="#ffffff">
       <SELECT name="prf">
        <OPTION value="0" selected>全て表示</OPTION>
ALPHA;

	$max = count($PRF_N);

	for($i=1; $i<$max; $i++) {
		echo ("        <OPTION value=\"$i\">$PRF_N[$i]</OPTION>\n");
		}

	echo <<<ALPHA
       </SELECT>
      </TD>
    </TR>
    <TR>
      <TD align="center" bgcolor="#cccccc">作成ファイル名（半角英数字）</TD>
      <TD bgcolor="#ffffff">
       <INPUT size="20" type="text" name="csv_file" value="$csv_file">.csv
      </TD>
    </TR>
    <TR>
      <TD colspan="2" bgcolor="#ffffff" align="center"><INPUT type="submit" value="作成">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

}



function first_02_2() {
// global $PHP_SELF,$main,$kokyaku,$csv,$member,$prf,$csv_file;
global $PHP_SELF;
include "../../cone.inc";
include './array.inc';
$member=$_POST['member'];
$prf=$_POST['prf'];
$csv_file=$_POST['csv_file'];

	$sql = "select * from kojin";
	if ($member != 3 || $prf != 0) {
	$sql .= " where ";
		}
	if ($member != 3) {
		$sql .= " saku='$member'";
		if ($prf != 0) { 
			$sql .= " AND";
			}
		}
	if ($prf != 0) {
		$sql .= " prf='$prf'";
		}
	$sql .= " ORDER BY kojin_num;";
	$sql1 = mysqli_query($conn_id,$sql);
	$max = mysqli_num_rows($sql1);

	$list = "\"会員番号\",\"お名前\",\"ふりがな\",\"郵便番号\",\"住所\",\"電話番号\",\"FAX番号\",\"メールアドレス\",\"パスワード\",\"メールマガジン\",\"ポイント\",\"会員登録状態\"\n";


	for($i=0; $i<$max; $i++) {
		list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$saku) = mysqli_fetch_array($sql1);

		$list .= "\"$kojin_num\",\"$name_s $name_n\",\"$kana_s $kana_n\",\"$zip1-$zip2\",\"$PRF_N[$prf]$city$add1 $add2\",\"$tel1-$tel2-$tel3\",\"$fax1-$fax2-$fax3\",\"$email\",\"$pass\",\"$MERUMA_N[$meruma]\",\"$point\",\"$MENBER_N[$saku]\"\n";

		}

#	$list = mb_convert_encoding($list,"SJIS","EUC");
	$list = mb_convert_encoding($list,"SJIS","UTF-8");

	$file_names = "$csv_file.csv";
	$files = "./member/$file_names";
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
// global $PHP_SELF,$main,$kokyaku,$csv,$mode,$del_file,$ERROR;
global $PHP_SELF,$ERROR,$del_file;
$mode=$_POST['mode'];
$del_file=$_POST["del_file"];

echo ("ファイル削除<BR>\n<BR>\n");

	if ($mode) {
		if ($del_file == "") { $ERROR = "削除するファイルが選択されておりません。"; }
		}

	if (!$mode || $ERROR) { first_03_1(); }
	elseif ($mode == "del") { first_03_2(); }

}



function first_03_1() {
//  global $PHP_SELF,$main,$kokyaku,$csv,$del_file,$ERROR;
global $PHP_SELF,$ERROR;
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$csv=$_POST['csv'];

if ($ERROR) {

	echo <<<ALPHA
<FONT color="#ff0000">$ERROR</FONT><BR>
<BR>
ALPHA;

	}

$dir_m = "./member";

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
<INPUT type="hidden" name="kokyaku" value="$kokyaku">
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
		$files = "./member/$val";
		unlink($files);
		echo ("・$val <BR>\n");
	}

}



?>
