<?PHP
//	顧客登録削除
function kokyaku_3() {
// global $PHP_SELF,$main,$kokyaku,$prf_,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$ERRORS,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$sex,$b_date,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$msg,$hlist,$s_email_;
global $PHP_SELF;
$view=$_POST['view'];
$mode=$_POST['mode'];
if ($view) {
	if ($mode == "del") { first_01(); }
	else { first_00(); }
	}
}


function first_00() {
// global $PHP_SELF,$main,$kokyaku,$prf_,$kokyaku,$num_k_,$name_s_,$view,$mode,$max,$view_s,$view_e,$views,$page,$msg,$hlist,$s_email_;
global $PHP_SELF,$view_s,$view_e,$views,$max;
global $num_k_,$page;// _POST
include '../sub/array.inc';
include './array.inc';
include "../../cone.inc";
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$prf_=$_POST['prf_'];
$num_k_=$_POST['num_k_'];
$name_s_=$_POST['name_s_'];
$view=$_POST['view'];
$page=$_POST['page'];
$msg=$_POST['msg'];
$hlist=$_POST['hlist'];
$s_email_=$_POST['s_email_'];

	if ($msg) { echo ($msg); }

#	$num_k_	= mb_convert_kana($num_k_,n,"EUC-JP");
	$num_k_	= mb_convert_kana($num_k_,n,"UTF-8");


	$sql  = "select kojin_num, name_s, name_n, prf, city, email from kojin";
	$sql .= " WHERE kojin_num<='100000' AND saku='0'";

	if ($num_k_) {
		$sql .= " AND kojin_num='$num_k_'";
		}
	if (!$num_k_ && $name_s_) {
		$sql .= " AND name_s='$name_s_'";
		}
	if ($s_email_) {
		$sql .= " AND email='$s_email_'";
	}

	if ($prf_) {
		$sql .= " AND prf='$prf_'";
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
削除したいお客様の番号を押して下さい。<BR>
Page : <?php echo $page; ?>/<?php echo $page_all; ?> <BR>
<FORM action='<?php echo $PHP_SELF; ?>' method='POST'>
<INPUT type="hidden" name="mode" value="<?php echo ("del"); ?>">
<INPUT type="hidden" name="main" value="<?php echo $main; ?>">
<INPUT type="hidden" name="main_r" value="<?php echo $main; ?>">
<INPUT type="hidden" name="kokyaku" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="kokyaku_r" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="menber" value="<?php echo $menber; ?>">
<INPUT type="hidden" name="prf_" value="<?php echo $prf_; ?>">
<INPUT type="hidden" name="num_k" value="<?php echo $num_k; ?>">
<INPUT type="hidden" name="name_s" value="<?php echo $name_s; ?>">
<INPUT type="hidden" name="s_email_" value="<?php echo $s_email_; ?>">
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
//  global $PHP_SELF,$main,$kokyaku,$prf_,$num_k_,$name_s_,$s_email_,$view,$mode,$page,$msg,$kojin_num;
global $PHP_SELF;
include './array.inc';
include "../../cone.inc";
$msg=$_POST['msg'];
$kojin_num=$_POST['kojin_num'];

	$sql = "update kojin set " .
			"saku='1'" .
			" where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);

	$count = pg_numrows($sql1);

	if ($count >= 0) {
		$sql = "select name_s, name_n from kojin where kojin_num='$kojin_num';";
		$sql1 = pg_exec($conn_id,$sql);
		list($name_s,$name_n) = pg_fetch_array($sql1,0);

		$msg = "会員番号：$kojin_num $name_s $name_n 様を削除致しました。<BR>\n";

		}
	else {

		$msg = "会員番号：$kojin_num を削除できませんでした。<BR>\n";

		}

first_00();

}


?>
