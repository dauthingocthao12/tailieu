<?PHP
//	発注状況
function order_1() {
global $view,$mode;//_POST
include "../../cone.inc";
$view=$_POST['view'];
	$mode=$_POST['mode'];
	if ($view) {
		if ($mode == "")		{ first_00(); }
		if ($mode == "syou")	{ first_01(); }
		if ($mode == "syou2")	{ first_02(); }
		if ($mode == "hen")		{ first_03(); } 
		if ($mode == "hen2")	{ first_04(); } //未確認 2022/11/15
		if ($mode == "add")		{ first_05(); } //未確認 2022/11/15
		}

}


function first_00() {
global $PHP_SELF,$max,$view_s,$view_e,$views;
global $main,$order,$view,$page;//_POST
include "../../cone.inc";
include './array.inc';
	$page=$_POST['page'];
	$SHIHARAI_N = $SHIHARAI_N1;
     echo $count;
	$point_data = "select sells_num from nopoints";
	$sql  = "select kojin_num from sells where sells_num IN ($point_data) GROUP BY kojin_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	$max = $count - 1;
	if ($view > 5) { $view = 1; }
	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$view_s = $views * ($page-1);
	$view_e = ($views * $page) - 1;
	if ($view_e > $max) { $view_e = $max; }
if ($count < 1) {
	echo <<<ALPHA
<BR>
現在未発送のご注文は御座いません。
ALPHA;

	}
else {
	echo <<<ALPHA
<FORM action='$PHP_SELF' method='POST'>
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="order" value="$order">
<INPUT type="hidden" name="order_r" value="$order">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="mode" value="syou">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc" align='center'>
      <TD>番号</TD>
      <TD>請求書</TD>
      <TD>名前</TD>
      <TD>住所</TD>
      <TD>メールアドレス</TD>
      <TD>注文日</TD>
    </TR>
ALPHA;


	for($i=$view_s; $i<=$view_e; $i++) {
		list($kojin_num) = pg_fetch_array($sql1,$i);

		$point_data = "select sells_num from nopoints";
		$sql2  = "select sells_num from sells";
		$sql2 .= " where kojin_num='$kojin_num' AND sells_num IN ($point_data)";
		$sql2 .= " GROUP BY sells_num ORDER BY sells_num;";
		$sql2_ = pg_exec($conn_id,$sql2);
		$count2 = pg_numrows($sql2_);
		$max2 = $count2 - 1;

		$hacyu = "";
		for($ii=0; $ii<=$max2; $ii++) {
			list($sells_num) = pg_fetch_array($sql2_,$ii);
			$year = substr($sells_num, 0, 2);
			$year = $year + 2000;
			$mon  = substr($sells_num, 2, 2);
			$day  = substr($sells_num, 4, 2);
			$hacyu .= "$year 年 $mon 月 $day 日<BR>\n";
		}

		$sql3  = "select name_s, name_n, prf, city, email from kojin where kojin_num='$kojin_num';";
		$sql3_ = pg_exec($conn_id,$sql3);
		list($name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql3_,0);

		$bill_count = 0;
		$sqlb = "SELECT count(*) AS bill_count FROM bill_base WHERE (order_num='$sells_num' OR order_num='ORG-$sells_num') AND state='0';";
		if ($result = pg_query($conn_id,$sqlb)) {
			$list = pg_fetch_array($result);
			$bill_count = $list['bill_count'];
		}

		if ($bill_count > 0) { $bill_msg = "○"; } else { $bill_msg = "--"; }

		echo <<<ALPHA
    <TR bgcolor="#ffffff" align='center'>
      <TD><INPUT type='submit' name='kojin_num' value='$kojin_num'></TD>
      <TD>$bill_msg</TD>
      <TD>$name_s $name_n</TD>
      <TD>$PRF_N[$prf] $city</TD>
      <TD><A href="mailto:$email">$email</A></TD>
      <TD>$hacyu</TD>
    </TR>
ALPHA;
		}

	echo <<<ALPHA
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	if ($max > $view_e || $view_s >= $views) { next_p_o_1(); }

	}

	pg_close($conn_id);

}


function first_01() {
global $PHP_SELF;
global $main,$order,$view,$kojin_num,$page;//_POST
include "../../cone.inc";
include './array.inc';
	$kojin_num=$_POST['kojin_num'];
	$page=$_POST['page'];

	$sql = "select name_s, name_n, prf, city, email from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,0);

	echo <<<ALPHA
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD align='center'>$kojin_num</TD>
      <TD align='center'>$name_s $name_n</TD>
      <TD align='center'>$PRF_N[$prf] $city</TD>
      <TD align='center'><A href="mailto:$email">$email</A></TD>
    </TR>
  </TBODY>
</TABLE>

ALPHA;

	$point_data = "select sells_num from nopoints";
	$sql  = "select sells_num from sells";
	$sql .= " where kojin_num='$kojin_num' AND sells_num IN ($point_data)";
	$sql .= " GROUP BY sells_num ORDER BY sells_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count < 1) {

		echo <<<ALPHA
<BR>
現在未発送の商品は御座いません。<BR>
<BR>
ALPHA;

		}
	else {

		echo <<<ALPHA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main"      value="$main">
<INPUT type="hidden" name="main_r"    value="$main">
<INPUT type="hidden" name="order"     value="$order">
<INPUT type="hidden" name="order_r"   value="$order">
<INPUT type="hidden" name="view"      value="$view">
<INPUT type="hidden" name="page"      value="$page">
<INPUT type="hidden" name="mode"      value="syou2">
<INPUT type="hidden" name="kojin_num" value="$kojin_num">
<TABLE border="0" bgcolor="#999999">
 <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>注文番号</TD>
      <TD align='center'>注文日</TD>
      <TD align='center'>発送先住所</TD>
      <TD align='center'>発送先電話番号</TD>
    </TR>

ALPHA;

	for($i=0; $i<$count; $i++) {
		list($sells_num) = pg_fetch_array($sql1,$i);

		$sql2  = "select f.name_s, f.name_n, f.prf, f.city, f.add1, f.add2, f.tel1, f.tel2, f.tel3, f.kei1, f.kei2, f.kei3 from add f, sells g";
		$sql2 .= " where g.sells_num='$sells_num' AND  f.add_num=g.add_num;";
		$sql2_ = pg_exec($conn_id,$sql2);

		list($name_s, $name_n, $prf, $city, $add1, $add2, $tel1, $tel2, $tel3, $kei1, $kei2, $kei3) = pg_fetch_array($sql2_,0);

		$year = substr($sells_num, 0, 2);
		$year = $year + 2000;
		$mon  = substr($sells_num, 2, 2);
		$day  = substr($sells_num, 4, 2);

		echo <<<ALPHA
    <TR bgcolor="#ffffff">
      <TD align='center'><INPUT type="submit" name="sells_num" value="$sells_num"></TD>
      <TD align='center'> $year 年 $mon 月 $day 日 </TD>
      <TD> $PRF_N[$prf] $city $add1 $add2 </TD>
      <TD> 固定：$tel1 - $tel2 - $tel3 携帯：$kei1 - $kei2 - $kei3</TD>
    </TR>

ALPHA;
		}


		echo <<<ALPHA
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	}

echo <<< ALPHA
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='order' value='$order'>
<INPUT type='hidden' name='order_r' value='$order'>
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="submit" value="戻る">
</FORM>
ALPHA;

	pg_close($conn_id);

}




function first_02() {
global $PHP_SELF,$CON_TESU,$TESU_P; //hinbanはfirst_03で初期ち
global $main,$order,$view,$kojin_num,$page,$sells_num,$hinban; //_POST
include "../../cone.inc";
include './array.inc';
include '../sub/array.inc';
	$kojin_num=$_POST['kojin_num'];
	$page=$_POST['page'];
	$sells_num=$_POST['sells_num'];
	$hinban=$_POST['hinban'];
	$sql = "select name_s, name_n, prf, city, email from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,0);

	echo <<<ALPHA
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD align='center'>$kojin_num</TD>
      <TD align='center'>$name_s $name_n</TD>
      <TD align='center'>$PRF_N[$prf] $city</TD>
      <TD align='center'><A href="mailto:$email">$email</A></TD>
    </TR> 
  </TBODY>
</TABLE>
<BR>
<BR>
ALPHA;

	$sql  = "select b.name_s, b.name_n, b.kana_s, b.kana_n, b.zip1, b.zip2, b.prf, b.city, b.add1, b.add2, b.tel1, b.tel2, b.tel3, b.kei1, b.kei2, b.kei3, b.fax1, b.fax2, b.fax3, b.email, b.email2, b.zaiko, b.siharai, b.t_time, b.msr, b.g_point, b.shipping";
	$sql .= " from add b, sells c where c.sells_num='$sells_num' AND  b.add_num=c.add_num;";
	$sql1 = pg_exec($conn_id,$sql);

	list($name_s, $name_n, $kana_s, $kana_n, $zip1, $zip2, $prf, $city, $add1, $add2, $tel1, $tel2, $tel3, $kei1, $kei2, $kei3, $fax1, $fax2, $fax3, $email, $email2, $zaiko, $shiharai, $t_time, $msr, $g_point, $shipping)	= pg_fetch_array($sql1,0);

	$year = substr($sells_num, 0, 2);
	$year = $year + 2000;
	$mon  = substr($sells_num, 2, 2);
	$day  = substr($sells_num, 4, 2);

	$print_check = 0;
	echo <<<ALPHA
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='left' colspan="7">注文番号 : $sells_num 　　　注文日 : $year 年 $mon 月 $day 日</TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>購入数</TD>
      <TD align='center'>小計金額</TD>
      <TD align='center'>納品状態</TD>
      <TD align='center'>切り替え</TD>
      <TD align='center'>在庫数</TD>
    </TR>
ALPHA;

	$sql  = "select hinban, title, price, buy_n, send, h_time, bargain, p_ritu, tax from sells";	//	add tax ookawara 2014/03/31	
	$sql .= " where sells_num='$sells_num' AND hinban!='option'";
	$sql .= " ORDER BY hinban;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	$price_all = 0;
	$point_get = 0;
	$mail_check = 0;
	for($i=0; $i<$count; $i++) {
		list($hinban, $title, $price, $buy_n, $send, $h_time, $bargain , $p_ritu , $tax) = pg_fetch_array($sql1,$i);	//	add $tax ookawara 2014/03/31

		$TAX_ = $tax;	//	add ookawara 2014/03/31

		$price = floor($price * ($TAX_ + 1) + 0.5);
		if ($send == 1) { $mail_check = 1; }
		if ($title != "option") {
			if ($send == 0 || $send == 3) { $point_get = 1; }
			if ($send == 2) { $price_a = 0; }
			else { $price_a = $price * $buy_n; }

			if ($send != 2) { $print_check = 1; }

			$sql2 = "SELECT stock FROM zaiko WHERE hinban='$hinban';";
			$sql2_ = pg_exec($conn_id,$sql2);
			$count_z = pg_numrows($sql2_);
			if ($count_z <= 0) { $stock = "--"; }
			else { list($stock) = pg_fetch_array($sql2_,0); }

##			list($k_year,$k_mon,$k_day) = split("-",$h_time);
			list($k_year,$k_mon,$k_day) = explode("-",$h_time);
			if ($k_year == "1000") {
				$k_year = date("Y");
				$k_mon = date("m");
				$k_day = date("d");
			}

			echo <<< ALPHA

    <TR bgcolor="#FFFFFF">
      <TD align='center'>$hinban</TD>
      <TD align='center'>$title</TD>
      <TD align='center'>$buy_n</TD>
      <TD align='right'>$price_a 円</TD>
      <TD align='center'>$HASOU[$send]</TD>
      <TD align='center'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="send_r" value="$send">
       <INPUT type="hidden" name="buy_n" value="$buy_n">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="order" value="$order">
       <INPUT type="hidden" name="order_r" value="$order">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="hen">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="hinban" value="$hinban">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type='submit' name='type' value='発送'> 
       <INPUT type='submit' name='type' value='キャンセル'> 
       <INPUT type='submit' name='type' value='取寄中'> 
       <INPUT type='submit' name='type' value='未発送'><BR>
       発送・入荷予定日<INPUT type="text" size="8" name="k_year" value="$k_year" maxlength="4">年 
       <INPUT type="text" size="4" name="k_mon" value="$k_mon" maxlength="2">月 
       <INPUT type="text" size="4" name="k_day" value="$k_day" maxlength="2">日
      </TD></FORM>
      <TD align='center'>$stock</TD>
    </TR>


ALPHA;

			$price_all = $price_all+$price_a;

		}

	}

	$sql = "SELECT * FROM option WHERE sells_num='$sells_num' ORDER BY hinban;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	for($i=0; $i<$count; $i++) {
		if ($i == 0) {
			echo <<< ALPHA

    <TR bgcolor="#e1e1e1">
      <TD colspan="7">オプション</TD>
    </TR>
ALPHA;

		}

		list($option_num_, $sells_num_, $kojin_num_, $hinban_, $title_, $seban_l_, $seban_num_, $seban_price_, $sename_l_, $sename_name_, $sename_price_, $muneban_l_, $muneban_num_, $muneban_price_, $pant_l_, $pant_num_, $pant_price_, $bach_l_, $bach_name_, $bach_price_, $send_, $h_time_) = pg_fetch_array($sql1,$i);
		if ($send_ == 1) { $mail_check = 1; }
		if ($send_ == 0 || $send_ == 3) { $point_get = 1; }

##		list($k_year,$k_mon,$k_day) = split("-",$h_time_);
		list($k_year,$k_mon,$k_day) = explode("-",$h_time_);
		if ($k_year == "1000") {
			$k_year = date("Y");
			$k_mon = date("m");
			$k_day = date("d");
		}

		echo <<< ALPHA

    <TR bgcolor="#e1e1e1">
      <TD>$hinban_</TD>
      <TD>$title_</TD>
      <TD colspan="2"></TD>
      <TD align='center'>$HASOU[$send_]</TD>
      <TD align='center'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="order" value="$order">
       <INPUT type="hidden" name="order_r" value="$order">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="hen2">
       <INPUT type="hidden" name="sells_num" value="$sells_num_">
       <INPUT type="hidden" name="hinban" value="$hinban_">
       <INPUT type="hidden" name="option_num" value="$option_num_">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type='submit' name='type' value='発送'> 
       <INPUT type='submit' name='type' value='キャンセル'> 
       <INPUT type='submit' name='type' value='取寄中'> 
       <INPUT type='submit' name='type' value='未発送'><BR>
       発送・入荷予定日<INPUT type="text" size="8" name="k_year" value="$k_year" maxlength="4">年 
       <INPUT type="text" size="4" name="k_mon" value="$k_mon" maxlength="2">月 
       <INPUT type="text" size="4" name="k_day" value="$k_day" maxlength="2">日
      </TD></FORM>
      <TD align='center'>--</TD>
    </TR>
ALPHA;

	$op1 = 0;
	$op2 = 0;
	$op3 = 0;
	$op4 = 0;
	$op5 = 0;
	$op6 = 0;

//	持ち込み手数料
		if ($hinban_ == "mochikomi") {
			$kakaku_ = $mochi_pri;
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_;
			$op1 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">持ち込み手数料</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

//	背番号
		if ($seban_l_) {
			$moji_num = strlen($seban_num_);
			$kakaku_ = $SEBAN_P_N[$seban_l_];
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $moji_num;
			$op2 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">背番号 $SEBAN_N[$seban_l_] 番号：$seban_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

//	背ネーム
		if ($sename_l_) {
			$sename_name_ = str_replace('\\', '', $sename_name_);
			$sename_name_m = str_replace(' ', '', $sename_name_);
			$moji_num = strlen($sename_name_m);
			$kakaku_ = $SENAME_P_N[$sename_l_];
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $moji_num;
			$op3 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">背ネーム $SENAME_N[$sename_l_] ネーム：$sename_name_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

//	胸番号
		if ($muneban_l_) {
			$moji_num = strlen($muneban_num_);
			$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $moji_num;
			$op4 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">胸番号 $MUNEBAN_N[$muneban_l_] 番号：$muneban_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

//	パンツ番号
		if ($pant_l_) {
			$moji_num = strlen($pant_num_);
			$kakaku_ = $PANT_P_N[$pant_l_];
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_ * $moji_num;
			$op5 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">パンツ番号 $PANT_N[$pant_l_] 番号：$pant_num_</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

//	バッジ
		if ($bach_l_) {
			$kakaku_ = $BACH_P_N[$bach_l_];
			$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
			$syoukei = $kakaku_;
			$op6 = $syoukei;
			$kakaku_ = number_format($kakaku_);
			$syoukei = number_format($syoukei);

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="2">バッジ $BACH_N[$bach_l_]</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;
		}

		if ($send_ != 2) {
			$price_all = $price_all + $op1 + $op2 + $op3 + $op4 + $op5 + $op6;
			$print_check = 1;
		}

	}

//	会員特別割引
	if ($bargain > 0) {

		$nebiki = $price_all * $bargain / 100;
		$price_all = $price_all - $nebiki;
		// 合計金額に小数点以下0.5が入るとnumber_formatで四捨五入されて実計算と誤差が出る
		// 表示する値引き金額を切り捨てて整数とする
		$nebiki = floor($nebiki);	// add ookawara 2014/02/25
		$nebiki = number_format($nebiki);

		echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>会員特別割引（ $bargain % ）</B></TD>
            <TD align="right"><FONT color="#ff0000">-$nebiki</FONT> 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

	}

	if ($g_point) {
		$p_all = $price_all;
		$g_point_p = $g_point;
		$p_all = $p_all - $g_point_p;
		if ($p_all <= 0) {
//			$g_point_p = $g_point_p + $p_all;
//			$g_point = ceil($g_point_p);
			$zan_g_point -= $p_all;
			$price_all = 0;
		}
		else {
			$price_all = $p_all;
		}
	}

	if (!$p_ritu) { $p_ritu = $P_RITU; }
	$k_point = $price_all * ($p_ritu / 100);
	$k_point = floor($k_point + 0.5);

//	$unchin = $UN_N[$prf];
//	$unchin = floor($unchin * ($TAX_ + 1) + 0.5);
//	if ($price_all < 1) {
//		$unchin = 0;
//		$tesu = 0;
//	}

	$unchin = $shipping;

	$aff_price = $price_all;

	if ($shiharai == 1 && $price_all > 0) {
		$TESU_P = floor($TESU_P * ($TAX_ + 1) + 0.5);
		$price_all = $price_all + $TESU_P + $unchin;
		$max = count($DAIBIKI_N);
		$daibiki = "";
		for($i=0; $i<$max; $i++) {
			$p_all = $price_all;
			//	支払金額が０の場合
			if ($p_all < 1) {
				$daibiki = "";
				$tax = "";
				$price_all = "";
				break;
			}

			if (!$daibiki && $DAIBIKI_N[$i] >= $p_all) {
				$daibiki = $DAIBIKI_P_N[$i];
				$daibiki = floor($daibiki * ($TAX_ + 1) + 0.5);
				$p_all = $p_all + $daibiki;
//				$tax = $p_all * $TAX_ + 0.5;
//				$tax = floor($tax);
//				$p_all = $p_all + $tax;
				if ($DAIBIKI_N[$i] >= $p_all) {
					$price_all = $p_all;
				}
				else {
					$daibiki = "";
					$tax = "";
				}
			}
		}
	} elseif ($shiharai == 4 && $price_all > 0) {
		$con_tesu = floor($CON_TESU * ($TAX_ + 1) + 0.5);
		$price_all = $price_all + $unchin + $con_tesu;
	
	//	後払い手数料計算	add ookawara 2010/12/10
	} elseif ($shiharai == 5 && $price_all > 0) {
		$ato_tesu = floor(($price_all+ $unchin) * (atobarai / 100) + 0.5);
		if ($ato_tesu < ato_low_price) { $ato_tesu = ato_low_price; }	// add ookawara 2011/01/20
		$price_all = $price_all + $unchin + $ato_tesu;
	} else {
		$price_all = $price_all + $unchin;
//		$tax = $price_all * $TAX_ + 0.5;
//		$tax = floor($tax);
//		$price_all = $price_all + $tax;
	}

	if ($zan_g_point) {
		$price_all -= $zan_g_point;
		if ($price_all < 0) {
			$g_point_p = $g_point_p + $price_all;
			$price_all = 0;
		}
	}

	$g_point_p = number_format($g_point_p);
//	$tax = number_format($tax);
	$daibiki = number_format($daibiki);
	$tesu = number_format($tesu);
	$unchin = number_format($unchin);
	$price_all = number_format($price_all);

		if ($g_point) {

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>割引ポイント利用</B></TD>
            <TD align="right"><FONT color="#ff0000">-$g_point_p</FONT> 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

		}

		echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>送料</B></TD>
            <TD align="right">$unchin 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

		if ($shiharai == 1) {

			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>代引手数料</B></TD>
            <TD align="right">$daibiki 円</TD>
            <TD colspan="3"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>支払手数料</B></TD>
            <TD align="right">$TESU_P 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

		} elseif ($shiharai == 4) {
			echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>請求書発行手数料</B></TD>
            <TD align="right">$con_tesu 円</TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

		//	後払い手数料追加 2010/12/10 add ookawara
		} elseif ($shiharai == 5) {
			$atobarai = atobarai;
			$ato_low_price = ato_low_price;
			echo <<<WAKABA
          <TR bgcolor="#ffffff">
            <td colspan="3" align="right" bgcolor="#CCCCCC"><B>後払い決算手数料</B>(全代金{$atobarai}% 但し{$ato_low_price}円未満の場合は、{$ato_low_price}円。)</td>
            <td align="right">$ato_tesu 円</td>
            <td colspan="3">&nbsp;</td>
          </TR>
WAKABA;

		}

//		echo <<<ALPHA
//          <TR bgcolor="#ffffff">
//            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>消費税</B></TD>
//            <TD align="right">$tax 円</TD>
//            <TD colspan="3"></TD>
//          </TR>
//ALPHA;

		echo <<<ALPHA
          <TR bgcolor="#ffffff">
            <TD colspan="7"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="3" align="right" bgcolor="#CCCCCC"><B>合計金額</B></TD>
            <TD align="right"><B>$price_all 円</B></TD>
            <TD colspan="3"></TD>
          </TR>
ALPHA;

			if ($point_get == 0) {

//				if ($g_point || $kojin_num >= 100001) {
				if ($kojin_num >= 100001) {
					$point = 0;
				}
				else {
					$point = floor($k_point);
				}

			echo <<< ALPHA

    <TR bgcolor="#FFFFFF">
      <TD colspan="3" bgcolor="#cccccc" align='right'>ポイント( $p_ritu %)</TD>
      <TD align='right'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="order" value="$order">
       <INPUT type="hidden" name="order_r" value="$order">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="add">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type="hidden" name="aff_price" value="$aff_price">
       <INPUT type="text" size="6" name="point" value="$point"> pt
      </TD>
      <TD align='center' colspan="3">
       <INPUT type='submit' value='追加'>
      </TD></FORM>
    </TR>
ALPHA;

			}

		if ($mail_check == 1) {

			echo <<<ALPHA

    <TR bgcolor="#ffffff">
     <TD colspan="7"></TD>
    </TR>
    <TR bgcolor="#FFFFFF">
      <TD colspan="3" bgcolor="#cccccc" align='right'>発送メール作成</TD>
      <TD colspan="4">
       <FORM action="./make_mail.php" method="POST" target="_blank">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       発送会社 <INPUT type="radio" checked name="unsou" value="0">：佐川　<INPUT type="radio" name="unsou" value="1">：ヤマト　<INPUT type="radio" name="unsou" value="2">：日本郵便　<INPUT type="radio" name="unsou" value="3">：西濃運輸<br>
       伝票番号：<INPUT size="28" type="text" maxlength="14" name="bangou"><BR>
       <INPUT type='submit' value='メール作成'>
      </TD></FORM>
    </TR>

ALPHA;

		}

##	$msr = ereg_replace("\n","<BR>",$msr);	
	$msr = preg_replace("/\n/","<BR>",$msr);

	if ($t_time == "") { $t_time = "0"; }

	if ($print_check == 1) {
		echo <<<WAKABA
    <TR bgcolor="#FFFFFF">
      <TD colspan="3" bgcolor="#cccccc" align='right'>請求書作成</TD>
      <TD colspan="4">
       <FORM action="./bill.php" method="POST" target="bill">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type='submit' value='請求書作成'>
      </TD></FORM>
    </TR>

WAKABA;
	}

echo <<< ALPHA
  </TBODY>
</TABLE>
<BR>
<BR>
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='left' colspan="2">発送先連絡先住所</TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>名前</TD>
      <TD bgcolor="#ffffff">$name_s $name_n </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>ふりがな</TD>
      <TD bgcolor="#ffffff">$kana_s $kana_n </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>住所</TD>
      <TD bgcolor="#ffffff">
       〒$zip1 - $zip2 <BR>
       $PRF_N[$prf] $city <BR>
       $add1 $add2 
      </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>電話番号</TD>
      <TD bgcolor="#ffffff">固定電話：$tel1 - $tel2 - $tel3<BR>携帯電話：$kei1 - $kei2 - $kei3</TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>FAX番号</TD>
      <TD bgcolor="#ffffff">$fax1 - $fax2 - $fax3 </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>メールアドレス</TD>
      <TD bgcolor="#ffffff">Email1：<A href="mailto:$email">$email</A><BR>Email2：<A href="mailto:$email2">$email2</A></TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>配達希望時間</TD>
      <TD bgcolor="#ffffff">$HAITATU_N[$t_time] </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>在庫無き場合</TD>
      <TD bgcolor="#ffffff">$ZAIKO_N[$zaiko] </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>支払方法</TD>
      <TD bgcolor="#ffffff">$SHIHARAI_N[$shiharai] </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>メッセージ</TD>
      <TD bgcolor="#ffffff">$msr </TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<BR>
ALPHA;


echo <<< ALPHA
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='order' value='$order'>
<INPUT type='hidden' name='order_r' value='$order'>
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="mode" value="syou">
<INPUT type="hidden" name="kojin_num" value="$kojin_num">
<INPUT type="submit" value="戻る">

ALPHA;

	pg_close($conn_id);

}


function first_03() {
global $type,$sells_num,$hinban,$send_r,$buy_n,$k_year,$k_mon,$k_day;//_POST
include "../../cone.inc";
include './array.inc';
	$type=$_POST['type'];
	$sells_num=$_POST['sells_num'];
	$hinban=$_POST['hinban'];
	$send_r=$_POST['send_r'];
	$k_year=$_POST['k_year'];
	$k_mon=$_POST['k_mon'];
	$k_day=$_POST['k_day'];
	

	if ($type == "発送") { $send = "1"; $h_time = "$k_year" . "-$k_mon" . "-$k_day"; }
	elseif ($type == "キャンセル") { $send = "2"; $h_time = "1000-01-01"; }
	elseif ($type == "取寄中") { $send = "3"; $h_time = "$k_year" . "-$k_mon" . "-$k_day"; }
	elseif ($type == "未発送") { $send = "0"; $h_time = "1000-01-01"; }

	if ($send == 1 && $send_r != 1) {

		$sql = "SELECT stock FROM zaiko WHERE hinban='$hinban';";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if ($count > 0) {
			list($stock) = pg_fetch_array($sql1,0);
			if ($stock == "") { $stock = 0; }
			$stock = $stock - $buy_n;

			$sql = "update zaiko set stock='$stock' where hinban='$hinban';";
			$sql1 = pg_exec($conn_id,$sql);

			}

		}

	if ($send != 1 && $send_r == 1) {

		$sql = "SELECT stock FROM zaiko WHERE hinban='$hinban';";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if ($count > 0) {
			list($stock) = pg_fetch_array($sql1,0);
			$stock = $stock + $buy_n;

			$sql = "update zaiko set stock='$stock' where hinban='$hinban';";
			$sql1 = pg_exec($conn_id,$sql);

			}

		}

	$sql = "update sells set send='$send', h_time='$h_time' where sells_num='$sells_num' AND hinban='$hinban';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }

	pg_close($conn_id);

	first_02();

}


function first_04() {
global $type,$sells_num,$hinban,$option_num,$k_year,$k_mon,$k_day;
include "../../cone.inc";
include './array.inc';
    $type=$_POST['type'];//未確認 2022/11/15
	$sells_num=$_POST['sells_num'];//未確認 2022/11/15
	$hinban=$_POST['hinban'];//未確認 2022/11/15
	$option_num=$_POST['option_num']; // ToDO check
	$k_year=$_POST['k_year'];//未確認 2022/11/15
	$k_mon=$_POST['k_mon'];//未確認 2022/11/15
	$k_day=$_POST['k_day'];//未確認 2022/11/15

	if ($type == "発送") { $send = "1"; $h_time = "$k_year" . "-$k_mon" . "-$k_day"; }
	elseif ($type == "キャンセル") { $send = "2"; $h_time = "1000-01-01"; }
	elseif ($type == "取寄中") { $send = "3"; $h_time = "$k_year" . "-$k_mon" . "-$k_day"; }
	elseif ($type == "未発送") { $send = "0"; $h_time = "1000-01-01"; }

	$sql = "update option set send='$send', h_time='$h_time' where sells_num='$sells_num' AND option_num='$option_num';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	$sql = "update sells set send='$send', h_time='$h_time' where sells_num='$sells_num' AND hinban='option' AND title='$option_num';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }

	pg_close($conn_id);

	first_02();

}



function first_05() {
global $kojin_num,$sells_num,$point; //_POST
global $appoint_table,$aff_ritsu;
include "../../cone.inc";
include './array.inc';
include '../sub/setup.inc';
include '../sub/array.inc';
	$kojin_num=$_POST['kojin_num'];//未確認 2022/11/15
	$sells_num=$_POST['sells_num'];//未確認 2022/11/15
	$point=$_POST['point'];//未確認 2022/11/15

	$sql = "select point from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($point_) = @pg_fetch_array($sql1,0);

	$a_point = $point_ + $point;

	$sql = "update kojin set point='$a_point' where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }

	$sql = "select MAX(point_num) from points;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count >= 1) {
		list($point_num) = pg_fetch_array($sql1,0);
		$point_num = $point_num + 1;
		}
	else {
		$point_num = 1;
		}

	if (!$point) { $point = "0"; }
	$sql =  "insert into points values (" .
			" '$point_num'," .
			" '$kojin_num'," .
			" '$sells_num'," .
			" '$point');";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	if ($count <= 0) { $ERROR[] = "登録できませんでした。"; }

	$sql =  "DELETE FROM nopoints WHERE kojin_num='$kojin_num' AND sells_num='$sells_num';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	if ($count <= 0) { $ERROR[] = "注文チェック登録を削除できませんでした。"; }

	//	アフェリエイト処理
	$aff_price = $_POST['aff_price'];
	//	ポイント率読み込み
	$sql  = "SELECT p_ritu FROM $appoint_table" .
			" WHERE sells_num='$sells_num';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		(int)$p_ritu = $list['p_ritu'];
	}
	if ($p_ritu < 1) { $p_ritu = $aff_ritsu; }

	$point = floor($aff_price * $p_ritu / 100);

	$sql  = "UPDATE $appoint_table SET" .
			" point='$point'," .
			" state='1'," .
			" send_day=now()" .
			" WHERE sells_num='$sells_num';";
	$result = pg_exec($conn_id,$sql);

	pg_close($conn_id);

	first_01();

}




?>
