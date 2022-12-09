<?PHP
//	顧客情報修正
function kokyaku_2() {
global $view,$mode,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$sex,$b_date,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$hlist,$s_email_;
global $PHP_SELF,$ERRORS;
$view=$_POST['view'];
$mode=$_POST['mode'];
$kojin_num=$_POST['kojin_num'];
$name_s=$_POST['name_s'];
$name_n=$_POST['name_n'];
$kana_s=$_POST['kana_s'];
$kana_n=$_POST['kana_n'];
$b_year=$_POST['b_year'];
$zip1=$_POST['zip1'];
$zip2=$_POST['zip2'];
$prf=$_POST['prf'];
$city=$_POST['city'];
$add1=$_POST['add1'];
$tel1=$_POST['tel1'];
$tel2=$_POST['tel2'];
$tel3=$_POST['tel3'];
$fax1=$_POST['fax1'];
$fax2=$_POST['fax2'];
$fax3=$_POST['fax3'];
$email=$_POST['email'];
$pass=$_POST['pass'];
$meruma=$_POST['meruma'];

if ($view) {
	if ($mode == "") { first_00(); }
	elseif ($mode == "syou") { first_01(); }
	elseif ($mode == "write") {

#		$b_year	= mb_convert_kana($b_year,n,"EUC-JP");
#		$zip1	= mb_convert_kana($zip1,n,"EUC-JP");
#		$zip2	= mb_convert_kana($zip2,n,"EUC-JP");
#		$tel1	= mb_convert_kana($tel1,n,"EUC-JP");
#		$tel2	= mb_convert_kana($tel2,n,"EUC-JP");
#		$tel3	= mb_convert_kana($tel3,n,"EUC-JP");
#		$fax1	= mb_convert_kana($fax1,n,"EUC-JP");
#		$fax2	= mb_convert_kana($fax2,n,"EUC-JP");
#		$fax3	= mb_convert_kana($fax3,n,"EUC-JP");

		$b_year	= mb_convert_kana($b_year,n,"UTF-8");
		$zip1	= mb_convert_kana($zip1,n,"UTF-8");
		$zip2	= mb_convert_kana($zip2,n,"UTF-8");
		$tel1	= mb_convert_kana($tel1,n,"UTF-8");
		$tel2	= mb_convert_kana($tel2,n,"UTF-8");
		$tel3	= mb_convert_kana($tel3,n,"UTF-8");
		$fax1	= mb_convert_kana($fax1,n,"UTF-8");
		$fax2	= mb_convert_kana($fax2,n,"UTF-8");
		$fax3	= mb_convert_kana($fax3,n,"UTF-8");

		$ERROR = "";
		if (!$name_s) { $ERROR[] = "漢字氏名（姓）が入力されておりません。"; }
		if (!$name_n) { $ERROR[] = "漢字氏名（名）が入力されておりません。"; }
		if (!$kana_s) { $ERROR[] = "ひらがな氏名（姓）が入力されておりません。"; }
		if (!$kana_n) { $ERROR[] = "ひらがな氏名（名）が入力されておりません。"; }
		if (!$zip1) { $ERROR[] = "郵便番号３桁が入力されておりません。"; }
		$zip1_n = strlen($zip1);
##		if ($zip1 && (!eregi("[0-9]",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
		if ($zip1 && (!preg_match("/[0-9]/i",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
		if (!$zip2) { $ERROR[] = "郵便番号４桁が入力されておりません。"; }
		$zip2_n = strlen($zip2);
##		if ($zip2 && (!eregi("[0-9]",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
		if ($zip2 && (!preg_match("/[0-9]/i",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
		if (!$prf) { $ERROR[] = "都道府県名が選択されておりません。"; }
		if (!$city) { $ERROR[] = "市区町村名が入力されておりません。"; }
		if (!$add1) { $ERROR[] = "所番地が入力されておりません。"; }
		if (!$tel1 || !$tel2 || !$tel3) { $ERROR[] = "電話番号が入力されておりません。"; }
//		if (!$fax1 || !$fax2 || !$fax3) { $ERROR[] = "FAX番号が入力されておりません。"; }
		if (!$email) { $ERROR[] = "E-mailアドレスが入力されておりません。"; }
##		if ($email && !ereg("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
		if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
		$mail_host = $regs[1];
		if ($email &&!getmxrr($mail_host,$mxhostarr)) { $ERROR[] = "E-mailアドレスのホスト名が見つかりませんでした。"; }
		if (!$meruma) { $ERROR[] = "メールマガジン購読中、非購読中が選択されておりません。"; }
		if ( $kojin_num < 100001) {
			if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }
			$pass_l = strlen($pass);
			if ($pass && $pass_l < 6 || $pass_l > 8) { $ERROR[] = "パスワードが不正です。"; }
			}

		if ($ERROR) {

			$ERRORS = <<<ALPHA
<BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc"><FONT color="#ff0000">エラー</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">
ALPHA;

			foreach ($ERROR AS $val) {
				$ERRORS .= "       $val<BR>\n";
				}

			$ERRORS .= <<<ALPHA
     </TD>
    </TR>
  </TBODY>
</TABLE>
ALPHA;
			first_01();
			}
		else { first_02(); }
		}
	}

}


function first_00() {
// global $PHP_SELF,$main,$kokyaku,$prf_,$menber,$kokyaku,$num_k_,$name_s_,$view,$mode,$max,$view_s,$view_e,$views,$page,$hlist,$s_email_;
global $PHP_SELF,$max,$view_s,$view_e,$views;
global $num_k_,$page;
include '../sub/array.inc';
include './array.inc';
include "../../cone.inc";

$main=$_POST['main'];
$view=$_POST['view'];
$mode=$_POST['mode'];
$name_s_=$_POST['name_s_'];
$kokyaku=$_POST['kokyaku'];
$prf_=$_POST['prf_'];
$menber=$_POST['menber'];
$num_k_=$_POST['num_k_'];
$prf=$_POST['prf'];
$city=$_POST['city'];
$page=$_POST['page'];
$hlist=$_POST['hlist'];
$s_email_=$_POST['s_email_'];

#	$num_k_	= mb_convert_kana($num_k_,n,"EUC-JP");
	$num_k_	= mb_convert_kana($num_k_,n,"UTF-8");

	$num_k_ = chop($num_k_);
	$sql = "select kojin_num, name_s, name_n, prf, city, email from kojin";
	if ($num_k_ || $name_s_ || $menber != 3 || $prf_ || $s_email_) {
		$sql .= " where";
		}
	if ($num_k_) {
		$sql .= " kojin_num='$num_k_'";
		}
	if (!$num_k_ && $name_s_) {
		$sql .= " name_s='$name_s_'";
		}
	if (($num_k_ || $name_s_) && $menber <= 2) {
		$sql .= " AND";
		}
	if ($menber == 2) {
		$sql .= " kojin_num>='100001'";
		}
	elseif ($menber == 0 || $menber == 1) {
		$sql .= " kojin_num<='100000'";
		}
	if ($menber == 0) {
		$sql .= " AND saku='0'";
		}
	elseif ($menber == 1) {
		$sql .= " AND saku='1'";
		}
	if (($num_k_ || $name_s_ || $menber != 3) && $prf_) {
		$sql .= " AND";
		}
	if ($prf_) {
		$sql .= " prf='$prf_'";
		}
	if (($num_k_ || $name_s_ || $menber != 3 || $prf_) && $s_email_) {
		$sql .= " AND";
		}
	if ($s_email_) {
		$sql .= " email='$s_email_'";
	}
	if ($hlist == 1) { $desc = "DESC"; } else { $desc = ""; }
	$sql .= " order by kojin_num $desc;";

	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	$max = $count-1;
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;
	if ($view_e > $max) { $view_e = $max; }
	if ($count < 1) { echo ("登録されておりますお客様はいません。<BR><BR>\n"); }
	if ($count >= 1) {

	echo ("登録数 $count 件<BR>\n<BR>\n");

?>
変更したいお客様の番号を押して下さい。<BR>
Page : <?php echo $page; ?>/<?php echo $page_all; ?> <BR>
<FORM action='<?php echo $PHP_SELF; ?>' method='POST'>
<INPUT type="hidden" name="mode" value="<?php echo "syou"; ?>">
<INPUT type="hidden" name="main" value="<?php echo $main; ?>">
<INPUT type="hidden" name="main_r" value="<?php echo $main; ?>">
<INPUT type="hidden" name="kokyaku" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="kokyaku_r" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="menber" value="<?php echo $menber; ?>">
<INPUT type="hidden" name="s_email_" value="<?php echo $s_email_; ?>">
<INPUT type="hidden" name="prf_" value="<?php echo $prf_; ?>">
<INPUT type="hidden" name="num_k_" value="<?php echo $num_k_; ?>">
<INPUT type="hidden" name="name_s" value="<?php echo $name_s; ?>">
<INPUT type="hidden" name="view" value="<?php echo $view; ?>">
<INPUT type="hidden" name="page" value="<?php echo $page; ?>">
<INPUT type="hidden" name="hlist" value="<?php echo $hlist; ?>">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>
<?PHP

	for($i=$view_s; $i<=$view_e; $i++) {
		list($kojin_num,$name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,$i);
echo <<<ALPHA
    <TR bgcolor="#ffffff">
      <TD align='center'>
       <INPUT type='submit' name='kojin_num' value='$kojin_num'>
      </TD>
      <TD>
       $name_s $name_n
      </TD>
      <TD>
       $PRF_N[$prf] $city
      </TD>
      <TD>
       <A href="mailto:$email">$email</A>
      </TD>
    </TR>

ALPHA;
		}

?>
  </TBODY>
</TABLE>
</FORM>
<?PHP

	if ($max > $view_e || $view_s >= $views) { next_p_k(); }

	}

	pg_close($conn_id);

}


function first_01() {
// global $PHP_SELF,$main,$kokyaku,$prf_,$menber,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$kojin_num,$ERRORS,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$sex,$b_date,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$hlist,$s_email_;
global $PHP_SELF,$ERRORS;
include '../sub/array.inc';
include './array.inc';
include "../../cone.inc";
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$prf_=$_POST['prf_'];
$menber=$_POST['menber'];
$num_k_=$_POST['num_k_'];
$name_s_=$_POST['name_s_'];
$view=$_POST['view'];
$mode=$_POST['mode'];
$page=$_POST['page'];
$kojin_num=$_POST['kojin_num'];
$name_s=$_POST['name_s'];
$name_n=$_POST['name_n'];
$kana_s=$_POST['kana_s'];
$kana_n=$_POST['kana_n'];
$zip1=$_POST['zip1'];
$zip2=$_POST['zip2'];
$prf=$_POST['prf'];
$city=$_POST['city'];
$add1=$_POST['add1'];
$add2=$_POST['add2'];
$tel1=$_POST['tel1'];
$tel2=$_POST['tel2'];
$tel3=$_POST['tel3'];
$fax1=$_POST['fax1'];
$fax2=$_POST['fax2'];
$fax3=$_POST['fax3'];
$email=$_POST['email'];
$pass=$_POST['pass'];
$meruma=$_POST['meruma'];
$point=$_POST['point'];
$hlist=$_POST['hlist'];

$s_email_=$_POST['s_email_'];
	if (!$ERRORS) {
		$sql = "select * from kojin where kojin_num='$kojin_num';";
		$sql1 = pg_exec($conn_id,$sql);

		list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,
			$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,
			$pass,$meruma,$point) = pg_fetch_array($sql1,0);

		}

		if ($meruma == 1) { $meruma1 = "checked"; $meruma2 = ""; } else { $meruma1 = ""; $meruma2 = "checked"; }

echo <<<ALPHA
$ERRORS
変更をして変更ボタンを押して下さい。<BR>
<FORM action='$PHP_SELF' method='POST'>
<INPUT type="hidden" name="mode" value="write">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="kokyaku" value="$kokyaku">
<INPUT type="hidden" name="kokyaku_r" value="$kokyaku">
<INPUT type="hidden" name="menber" value="$menber">
<INPUT type="hidden" name="prf_" value="$prf_">
<INPUT type="hidden" name="num_k_" value="$num_k_">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="s_email_" value="$s_email_">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="kojin_num" value="$kojin_num">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR>
      <TD bgcolor="#cccccc"><B>漢字氏名</B>(全角)<FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　姓：<INPUT name="name_s" value="$name_s">　名：<INPUT name="name_n" value="$name_n"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>ふりがな</B>(全角)<FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　姓：<INPUT name="kana_s" value="$kana_s">　名：<INPUT name="kana_n" value="$kana_n"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>住所</B><FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><B>郵便番号</B>(半角)<BR>
      　〒<INPUT size="6" name="zip1" value="$zip1">-<INPUT size="8" name="zip2" value="$zip2"></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><B>都道府県</B><BR>
      　<SELECT size="1" name="prf">
ALPHA;

		for($i=1; $i<=47; $i++) {
			if ($i == $prf) { $selected = "selected"; } else { $selected = ""; }
			echo ("       <option value=\"$i\" $selected>$PRF_N[$i]</option>\n");
			}

echo <<<ALPHA
      </SELECT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><B>市区町村</B>(全角)<BR>
      　<INPUT size="30" name="city" value="$city"></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><B>所番地</B>(全角)<BR>
      　<INPUT size="40" name="add1" value="$add1"></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><B>マンション名など</B>（全角）<BR>
      　<INPUT size="40" name="add2" value="$add2"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>電話番号</B>（半角）<FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT size="8" name="tel1" value="$tel1">-<INPUT size="8" name="tel2" value="$tel2">-<INPUT size="8" name="tel3" value="$tel3"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>FAX番号</B>（半角）</TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT size="8" name="fax1" value="$fax1">-<INPUT size="8" name="fax2" value="$fax2">-<INPUT size="8" name="fax3" value="$fax3"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>E-mailアドレス</B>（半角）<FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT size="40" name="email" value="$email"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>パスワード</B>（半角英数字６文字以上８文字以内）<FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT type="text" size="16" name="pass" value="$pass"></TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>メールマガジン購読</B><FONT color="#ff0000">(必記)</FONT></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT type="radio" value="1" name="meruma" $meruma1>：購読中 <INPUT type="radio" value="2" name="meruma" $meruma2>：非購読中</TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc"><B>ポイント</B>（半角）</TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff">　<INPUT type="text" size="5" name="point" value="$point"></TD>
    </TR>
    <TR>
      <TD bgcolor="#ffffff"><INPUT type="submit" value="変更">　<INPUT type="reset" value="元に戻す"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='kokyaku' value='$kokyaku'>
<INPUT type='hidden' name='kokyaku_r' value='$kokyaku'>
<INPUT type='hidden' name='menber' value='$menber'>
<INPUT type='hidden' name='prf_' value='$prf_'>
<INPUT type="hidden" name="num_k_" value="$num_k_">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="s_email_" value="$s_email_">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="submit" value="戻る">
ALPHA;

	pg_close($conn_id);

}


function first_02() {
// global $PHP_SELF,$main,$kokyaku,$prf_,$menber,$num_k_,$name_s_,$view,$mode,$page,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$s_email_;
global $PHP_SELF,$ERRORS;
$kojin_num=$_POST['kojin_num'];
$name_s=$_POST['name_s'];
$name_n=$_POST['name_n'];
$kana_s=$_POST['kana_s'];
$kana_n=$_POST['kana_n'];
$zip1=$_POST['zip1'];
$zip2=$_POST['zip2'];
$prf=$_POST['prf'];
$city=$_POST['city'];
$add1=$_POST['add1'];
$add2=$_POST['add2'];
$tel1=$_POST['tel1'];
$tel2=$_POST['tel2'];
$tel3=$_POST['tel3'];
$fax1=$_POST['fax1'];
$fax2=$_POST['fax2'];
$fax3=$_POST['fax3'];
$email=$_POST['email'];
$pass=$_POST['pass'];
$meruma=$_POST['meruma'];
$point=$_POST['point'];
include './array.inc';
include "../../cone.inc";

	if (!$point) { $point = "0"; }
	$sql = "update kojin set " .
			"name_s='$name_s'," .
			"name_n='$name_n'," .
			"kana_s='$kana_s'," .
			"kana_n='$kana_n'," .
			"zip1='$zip1'," .
			"zip2='$zip2'," .
			"prf='$prf'," .
			"city='$city'," .
			"add1='$add1'," .
			"add2='$add2'," .
			"tel1='$tel1'," .
			"tel2='$tel2'," .
			"tel3='$tel3'," .
			"fax1='$fax1'," .
			"fax2='$fax2'," .
			"fax3='$fax3'," .
			"email='$email'," .
			"pass='$pass'," .
			"meruma='$meruma'," .
			"point='$point'" .
			" where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);

first_00();

}
?>
