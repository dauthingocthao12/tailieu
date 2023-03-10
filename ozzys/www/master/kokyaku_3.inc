<?PHP
//	顧客登録削除
function kokyaku_3() {
global $PHP_SELF,$main,$kokyaku,$prf_,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$ERRORS,$kojin_num,$name_s,$name_n,$kana_s,$kana_n,$sex,$b_date,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$point,$msg;

if ($view) {
	if ($mode == "del") { first_01(); }
	else { first_00(); }
	}
}


function first_00() {
global $PHP_SELF,$main,$kokyaku,$prf_,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$msg,$max,$view_s,$views,$view_e;
include './array.inc';
include "../../cone.inc";

	if ($msg) { echo ($msg); }

	$num_k_	= mb_convert_kana($num_k_,n,"EUC-JP");

	$sql = "select kojin_num, name_s, name_n, prf, city, email from kojin where saku!=1";
	if ($num_k_ || $name_s || $menber != 3) {
		$sql .= " AND";
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

	$sql .= " kojin_num BETWEEN 100001 AND 600000";

	if ($prf_) {
		$sql .= " AND prf='$prf_'";
		}
	$sql .= " order by kojin_num;";

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
Page : <? echo("$page"); ?>/<? echo("$page_all"); ?> <BR>
<FORM action='<? echo("$PHP_SELF"); ?>' method='POST'>
<INPUT type="hidden" name="mode" value="<? echo("del"); ?>">
<INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
<INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
<INPUT type="hidden" name="kokyaku" value="<? echo("$kokyaku"); ?>">
<INPUT type="hidden" name="kokyaku_r" value="<? echo("$kokyaku"); ?>">
<INPUT type="hidden" name="menber" value="<? echo("$menber"); ?>">
<INPUT type="hidden" name="prf_" value="<? echo("$prf_"); ?>">
<INPUT type="hidden" name="num_k" value="<? echo("$num_k"); ?>">
<INPUT type="hidden" name="name_s" value="<? echo("$name_s"); ?>">
<INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
<INPUT type="hidden" name="page" value="<? echo("$page"); ?>">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>
<?

	for($i=$view_s; $i<=$view_e; $i++) {
		list($kojin_num,$name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,$i);
echo <<<OZZYS
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
OZZYS;
		}

?>
  </TBODY>
</TABLE>
</FORM>
<?

	if ($max > $view_e || $view_s >= $views) { next_p_k(); }

	}

	pg_close($conn_id);

}


function first_01() {
global $PHP_SELF,$main,$kokyaku,$prf_,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$msg,$kojin_num;
include './array.inc';
include "../../cone.inc";

	$sql = "update kojin set " .
			"saku='1'" .
			" where kojin_num='$kojin_num';";

	if (pg_exec($conn_id,$sql)) {
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
