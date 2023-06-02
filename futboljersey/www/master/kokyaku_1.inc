<?PHP
//	顧客情報管理	************************************************************************
//	顧客買い物履歴
function kokyaku_1() {
// global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num,$s_email_;
global $PHP_SELF;
$view=$_POST['view'];
$mode=$_POST['mode'];
if ($view) {
	if ($mode == "") { first_00(); }
	elseif ($mode == "syou") { first_01(); }
	}

}



function first_00() {
// global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$max,$view_s,$view_e,$views,$page,$hlist,$menber2,$s_email_;
global $PHP_SELF,$views,$view_s,$view_e,$max;
global $num_k_,$page;//_POST
include './array.inc';
include "../../cone.inc";
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$prf_=$_POST['prf_'];
$num_k_=$_POST['num_k_'];
$name_s_=$_POST['name_s_'];
$view=$_POST['view'];
$page=$_POST['page'];
$hlist=$_POST['hlist'];
$menber2=$_POST['menber2'];
$s_email_=$_POST['s_email_'];
#	$num_k_	= mb_convert_kana($num_k_,n,"EUC-JP");
	$num_k_	= mb_convert_kana($num_k_,n,"UTF-8");

	$num_k_ = chop($num_k_);
	if ($num_k_) {
		$sql = "select kojin_num from sells where kojin_num='$num_k_' GROUP BY kojin_num";
	}
	elseif ($prf_ || $name_s_ || $s_email_) {
		$sql = "select c.kojin_num from kojin a, sells c";
		if ($prf_ || $name_s_ || $s_email_ || $menber2 > 0) {
			$sql .= " where a.kojin_num=c.kojin_num";
			if ($prf_) {
				$sql .= " AND a.prf='$prf_'";
			}
			if ($name_s_) {
				$sql .= " AND a.name_s='$name_s_'";
			}
			if ($menber2 == 1) {
				$sql .= " AND c.kojin_num<='100000'";
			}
			if ($menber2 == 2) {
				$sql .= " AND c.kojin_num>'100000'";
			}
			if ($s_email_) {
				$sql .= " AND a.email='$s_email_'";
			}
		}
		$sql .= " GROUP BY c.kojin_num";
		if ($hlist == 1) { $desc = "DESC"; } else { $desc = ""; }
		$sql .= " ORDER BY c.kojin_num $desc";
	}
	else {
		$sql = "select kojin_num from sells";
		if ($menber2) {
			$sql .= " WHERE";
		}
		if ($menber2 == 1) {
			$sql .= " kojin_num<='100000'";
		} elseif ($menber2 == 2) {
			$sql .= " kojin_num>'100000'";
		}

		$sql .= " GROUP BY kojin_num";
		if ($hlist == 1) { $desc = "DESC"; } else { $desc = ""; }
		$sql .= " ORDER BY kojin_num $desc";
	}

	$sql1 = mysqli_query($conn_id,$sql);
	$count = mysqli_num_rows($sql1);
	$max = $count-1;
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;
	if ($view_e > $max) { $view_e = $max; }
	if ($count < 1) { echo ("購入されたお客様はいません。<BR><BR>\n"); }
	if ($count >= 1) {

	echo ("購入お客様数 $count 人<BR>\n<BR>\n");

?>
詳細を見たいお客様の番号を押して下さい。<BR>
Page : <?php echo $page; ?>/<?php echo $page_all; ?> <BR>
<FORM action='<?php echo $PHP_SELF; ?>' method='POST'>
<INPUT type="hidden" name="mode" value="<?php echo("syou"); ?>">
<INPUT type="hidden" name="main" value="<?php echo $main; ?>">
<INPUT type="hidden" name="main_r" value="<?php echo $main; ?>">
<INPUT type="hidden" name="kokyaku" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="kokyaku_r" value="<?php echo $kokyaku; ?>">
<INPUT type="hidden" name="prf_" value="<?php echo $prf_; ?>">
<INPUT type="hidden" name="num_k_" value="<?php echo $num_k_; ?>">
<INPUT type="hidden" name="name_s" value="<?php echo $name_s; ?>">
<INPUT type="hidden" name="s_email_" value="<?php echo $s_email_; ?>">
<INPUT type="hidden" name="view" value="<?php echo $view; ?>">
<INPUT type="hidden" name="page" value="<?php echo $page; ?>">
<INPUT type="hidden" name="hlist" value="<?php echo $hlist; ?>">
<INPUT type="hidden" name="menber2" value="<?php echo $menber2; ?>">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>
<?PHP
	$limit=$view_e-$view_s+1;
	$sql.=" LIMIT ".$limit." OFFSET ".$view_s.";";
	$sql1 = mysqli_query($conn_id,$sql);

	for($i=0; $i<$limit; $i++) {
		list($kojin_num) = mysqli_fetch_array($sql1);
		$sql2 = "select name_s, name_n, prf, city, email from kojin WHERE kojin_num='$kojin_num' ORDER BY name_s;";
		$sql2_ = mysqli_query($conn_id,$sql2);
		list($name_s, $name_n, $prf, $city, $email) = mysqli_fetch_array($sql2_);

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

	// mysqli_close($conn_id);

}



function first_01() {
// global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num,$hinban,$title,$price_all,$sells_num,$menber2,$hlist,$s_email_;
global $PHP_SELF,$conn_id;
include './array.inc';
include "../../cone.inc";
$main=$_POST['main'];
$kokyaku=$_POST['kokyaku'];
$prf_=$_POST['prf_'];
$num_k_=$_POST['num_k_'];
$name_s_=$_POST['name_s_'];
$view=$_POST['view'];
$page=$_POST['page'];
$kojin_num=$_POST['kojin_num'];
$hinban=$_POST['hinban'];
$title=$_POST['title'];
$price_all=$_POST['price_all'];
$sells_num=$_POST['sells_num'];
$menber2=$_POST['menber2'];
$hlist=$_POST['hlist'];
$s_email_=$_POST['s_email_'];

	$sql = "select name_s, name_n, prf, city, email from kojin WHERE kojin_num='$kojin_num' ORDER BY name_s;";
	$sql1 = mysqli_query($conn_id,$sql);
	list($name_s, $name_n, $prf, $city, $email) = mysqli_fetch_array($sql1);

	$sql = "select sells_num from sells where kojin_num='$kojin_num' GROUP BY sells_num ORDER BY sells_num DESC;";
	$sql1 = mysqli_query($conn_id,$sql);
	$count = mysqli_num_rows($sql1);
	$max = $count-1;

echo <<<ALPHA
<BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
      <TD align='center'>注文回数</TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD align='center'>$kojin_num</TD>
      <TD align='center'>$name_s $name_n</TD>
      <TD align='center'>$PRF_N[$prf] $city</TD>
      <TD align='center'><A href="mailto:$email">$email</A></TD>
      <TD align='center'>$count 回</TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
ALPHA;

	$opflag = 0;
	for($i=0; $i<=$max; $i++) {
		list($sells_num) = mysqli_fetch_array($sql1);

		$sql2  = "select hinban, title, price, buy_n, send, h_time from sells";
		$sql2 .= " where sells_num='$sells_num'";
		$sql2 .= " ORDER BY sells_num;";
		$sql2_ = mysqli_query($conn_id,$sql2);
		$count2 = mysqli_num_rows($sql2_);
		$max2 = $count2-1;

		$price_all = 0;
		for($ii=0; $ii<=$max2; $ii++) {
			list($hinban, $title, $price, $buy_n, $send, $h_time) = mysqli_fetch_array($sql2_);

			if ($ii == 0) {

				$year = substr($sells_num, 0, 2);
				$year = $year + 2000;
				$mon  = substr($sells_num, 2, 2);
				$day  = substr($sells_num, 4, 2);

echo <<< ALPHA
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='left' colspan="5">注文番号 : $sells_num 　　　注文日 : $year 年 $mon 月 $day 日</TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center' width="250">商品番号</TD>
      <TD align='center' width="250">商品名</TD>
      <TD align='center' width="60">購入数</TD>
      <TD align='center' width="100">小計金額</TD>
      <TD align='center' width="100">納品状態</TD>
    </TR>
ALPHA;

				}

	if ($hinban != "option") {

		$price_a = $price * $buy_n;

		echo <<< ALPHA
    <TR bgcolor="#FFFFFF">
      <TD align='center' width="250">$hinban</TD>
      <TD align='center' width="250">$title</TD>
      <TD align='center' width="60">$buy_n</TD>
      <TD align='right' width="100">$price_a 円</TD>
      <TD align='center' width="100">$HASOU[$send]</TD>
    </TR>
ALPHA;

	$price_all = $price_all+$price_a;
				}
	else { $opflag = 1; }

			}

	if ($opflag == 1) { options(); }

echo <<< ALPHA
    <TR bgcolor="#FFFFFF">
      <TD align='center' colspan="3">合計金額</TD>
      <TD align='right'>$price_all 円</TD>
      <TD align='center'>&nbsp;</TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<BR>
ALPHA;

		}


echo <<< ALPHA
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='kokyaku' value='$kokyaku'>
<INPUT type='hidden' name='kokyaku_r' value='$kokyaku'>
<INPUT type='hidden' name='prf_' value='$prf_'>
<INPUT type="hidden" name="num_k_" value="$num_k_">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="s_email_" value="$s_email_">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="menber2" value="$menber2">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="submit" value="戻る">
ALPHA;

	// mysqli_close($conn_id);

}



function options() {
// global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num,$hinban,$title,$price_all,$sells_num;
global $PHP_SELF;
include './array.inc';
include "../../cone.inc";
include '../sub/array.inc';
$main=$_POST['main'];
$view=$_POST['view'];
$kojin_num=$_POST['kojin_num'];
$price_all=$_POST['price_all'];
$sells_num=$_POST['sells_num'];

	$sql_o = "SELECT * FROM `option` WHERE sells_num='$sells_num' AND kojin_num='$kojin_num';";
	$sql_o1 = mysqli_query($conn_id,$sql_o);
	$counts = mysqli_num_rows($sql_o1);

	for($op=0; $op<$counts; $op++) {

			echo <<< ALPHA
    <TR bgcolor="#e1e1e1">
      <TD colspan="5">オプション</TD>
    </TR>
ALPHA;


	list($option_num_, $sells_num_, $kojin_num_, $hinban_, $title_, $seban_l_, $seban_num_, $seban_price_, $sename_l_, $sename_name_, $sename_price_, $muneban_l_, $muneban_num_, $muneban_price_, $pant_l_, $pant_num_, $pant_price_, $bach_l_, $bach_name_, $bach_price_, $send_, $h_time_) = mysqli_fetch_array($sql_o1);

		echo <<< ALPHA
    <TR bgcolor="#e1e1e1">
      <TD>$hinban_</TD>
      <TD>$title_</TD>
      <TD align='center'>1</TD>
      <TD align='center'>--</TD>
      <TD align='center'>$HASOU[$send_]</TD>
    </TR>
ALPHA;

		if ($hinban_ == "mochikomi") {
			$kakaku_ = $mochi_pri;
			$syoukei = $kakaku_;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">持ち込み手数料</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

//	背番号
		if ($seban_l_) {
			$moji_num = strlen(mb_convert_encoding($seban_num_,'SJIS','UTF8'));
			$kakaku_ = $SEBAN_P_N[$seban_l_];
			$syoukei = $kakaku_ * $moji_num;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">背番号 $SEBAN_N[$seban_l_] 番号：$seban_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

//	背ネーム
		if ($sename_l_) {
			$sename_name_ = str_replace('\\', '', $sename_name_);
			$sename_name_m = str_replace(' ', '', $sename_name_);
			$moji_num = strlen(mb_convert_encoding($sename_name_m,'SJIS','UTF8'));
			$kakaku_ = $SENAME_P_N[$sename_l_];
			$syoukei = $kakaku_ * $moji_num;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">背ネーム $SENAME_N[$sename_l_] ネーム：$sename_name_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

//	胸番号
		if ($muneban_l_) {
			$moji_num = strlen(mb_convert_encoding($muneban_num_,'SJIS','UTF8'));
			$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
			$syoukei = $kakaku_ * $moji_num;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">胸番号 $MUNEBAN_N[$muneban_l_] 番号：$muneban_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

//	パンツ番号
		if ($pant_l_) {
			$moji_num = strlen(mb_convert_encoding($pant_num_,'SJIS','UTF8'));
			$kakaku_ = $PANT_P_N[$pant_l_];
			$syoukei = $kakaku_ * $moji_num;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">パンツ番号 $PANT_N[$pant_l_] 番号：$pant_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

//	バッジ
		if ($bach_l_) {
			$kakaku_ = $BACH_P_N[$bach_l_];
			$syoukei = $kakaku_;
			$price_all = $price_all + $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">バッジ $BACH_N[$bach_l_]</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
ALPHA;
			}

		}

}

?>
