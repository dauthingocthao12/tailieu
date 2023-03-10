<?PHP
//	発注状況

//	新消費税計算時間（注文番号）
	define("NEWTAXDAY","080306175919");

function goods_2() {
global $PHP_SELF,$main,$goods,$view,$mode;
include "../../cone.inc";

	if ($view) {
		if ($mode == "")		{ first_00(); }
		if ($mode == "syou")	{ first_01(); }
		if ($mode == "syou2")	{ first_02(); }
		if ($mode == "henkou")	{ first_02(); }
		if ($mode == "hen")		{ first_03(); }
		if ($mode == "add")		{ first_04(); }
		if ($mode == "send_change")		{ first_05(); }
		}

}


function first_00() {
global $PHP_SELF,$main,$goods,$view,$mode;
include "../../cone.inc";
include './array.inc';

	$sql  = "select count(distinct kojin_num) AS count from sells where state!='1';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	if ($count < 1) {
		echo <<<OZZYS
<BR>
現在未発送のご注文は御座いません。
OZZYS;

	} else {
		echo <<<OZZYS
<FORM action='$PHP_SELF' method='POST'>
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="goods_r" value="$goods">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="mode" value="syou">
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
      <TD align='center'>注文日</TD>
    </TR>
OZZYS;

		if (!$page) { $page = 1; }
		$views = $VIEW_NUM[$view];
		$offset = ($page - 1) * $views;
		$limit_max = $offset + $views;
		if ($count < $limit_max) {
			$limit = $count % $views;
		} else {
			$limit = $views;
		}
		if ($offset > 0) {
			$offset1 = " OFFSET $offset";
		}
		$limit_num = " $offset1 LIMIT $limit";

		$sql  = "SELECT distinct kojin_num FROM sells".
				" WHERE state!='1'".
				" ORDER BY kojin_num $limit_num;";
		if ($result = pg_query($conn_id,$sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				$kojin_num = $list['kojin_num'];
				unset($hacyu);
				$sql  = "SELECT distinct a.sells_num, b.name_s, b.name_n, b.prf, b.city, b.email".
						" FROM sells a, kojin b".
						" WHERE a.kojin_num=b.kojin_num AND a.kojin_num='$kojin_num' AND state!='1'".
						" ORDER BY a.sells_num;";
				if ($result2 = pg_query($conn_id,$sql)) {
					WHILE ($list2 = pg_fetch_array($result2)) {
						$sells_num = $list2['sells_num'];
						$name_s = $list2['name_s'];
						$name_n = $list2['name_n'];
						$prf = $list2['prf'];
						$city = $list2['city'];
						$email = $list2['email'];

						$year = substr($sells_num, 0, 2);
						$year = $year + 2000;
						$mon  = substr($sells_num, 2, 2);
						$day  = substr($sells_num, 4, 2);
						$hacyu .= "$year 年 $mon 月 $day 日<BR>\n";
					}
				}
				echo <<<OZZYS
    <TR bgcolor="#ffffff">
      <TD align='center'><INPUT type='submit' name='kojin_num' value='$kojin_num'></TD>
      <TD align='center'>$name_s $name_n</TD>
      <TD align='center'>$PRF_N[$prf] $city</TD>
      <TD align='center'><A href="mailto:$email">$email</A></TD>
      <TD align='center'>$hacyu</TD>
    </TR>
OZZYS;
			}
		}

		echo <<<OZZYS
  </TBODY>
</TABLE>
</FORM>
OZZYS;

	if ($max > $view_e || $view_s >= $views) { next_p_g_1(); }

	}

	pg_close($conn_id);

}


function first_01() {
global $PHP_SELF,$main,$goods,$view,$mode,$kojin_num,$page;
include "../../cone.inc";
include './array.inc';

	$sql = "select name_s, name_n, prf, city, email from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,0);

	echo <<<OZZYS
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

OZZYS;

	$point_data = "select sells_num from points";
	$sql  = "select sells_num from sells";
	$sql .= " where kojin_num='$kojin_num' AND sells_num NOT IN ($point_data)";
	$sql .= " GROUP BY sells_num ORDER BY sells_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count > 0) {

		echo <<<OZZYS
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main"      value="$main">
<INPUT type="hidden" name="main_r"    value="$main">
<INPUT type="hidden" name="goods"     value="$goods">
<INPUT type="hidden" name="goods_r"   value="$goods">
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

OZZYS;

		for($i=0; $i<$count; $i++) {
			list($sells_num) = pg_fetch_array($sql1,$i);

			$sql2  = "select f.name_s, f.name_n, f.prf, f.city, f.add1, f.add2, f.tel1, f.tel2, f.tel3 from add f, sells g";
			$sql2 .= " where g.sells_num='$sells_num' AND  f.add_num=g.add_num;";
			$sql2_ = pg_exec($conn_id,$sql2);

			list($name_s, $name_n, $prf, $city, $add1, $add2, $tel1, $tel2, $tel3) = pg_fetch_array($sql2_,0);

			$year = substr($sells_num, 0, 2);
			$year = $year + 2000;
			$mon  = substr($sells_num, 2, 2);
			$day  = substr($sells_num, 4, 2);

			echo <<<OZZYS
    <TR bgcolor="#ffffff">
      <TD align='center'><INPUT type="submit" name="sells_num" value="$sells_num"></TD>
      <TD align='center'> $year 年 $mon 月 $day 日 </TD>
      <TD> $PRF_N[$prf] $city $add1 $add2 </TD>
      <TD align='center'> $tel1 - $tel2 - $tel3 </TD>
    </TR>

OZZYS;
			}


			echo <<<OZZYS
  </TBODY>
</TABLE>
</FORM>
OZZYS;

	}
	else {
		echo <<<WAKABA
<BR>
今現在ご注文はございません。<BR>
<BR>
WAKABA;
	}

echo <<< OZZYS
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='goods' value='$goods'>
<INPUT type='hidden' name='goods_r' value='$goods'>
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="submit" value="戻る">
</FORM>
OZZYS;

	pg_close($conn_id);

}



function first_02() {
global $PHP_SELF,$main,$goods,$view,$mode,$kojin_num,$page,$sells_num,$b_nums,$list_num;
include "../../cone.inc";
include './array.inc';
include '../sub/array.inc';
include '../sub/souryou.inc';
include '../sub/setup.inc';

	if ($mode == "henkou") {
		if (!$b_nums) { $b_nums = 0; }
		$sql = "UPDATE sells set buy_n='$b_nums' where sells_num='$sells_num' AND list_num='$list_num';";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }
		}

	$sql = "select name_s, name_n, prf, city, email from kojin where kojin_num='$kojin_num';";
	$sql1 = pg_exec($conn_id,$sql);
	list($name_s,$name_n,$prf,$city,$email) = pg_fetch_array($sql1,0);

	echo <<<OZZYS
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
      <TD align='center'>$PRF_L[$prf] $city</TD>
      <TD align='center'><A href="mailto:$email">$email</A></TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<BR>
OZZYS;

	$sql  = "select f.name_s, f.name_n, f.kana_s, f.kana_n, f.zip1, f.zip2, f.prf, f.city, f.add1, f.add2," .
			" f.tel1, f.tel2, f.tel3, f.fax1, f.fax2, f.fax3, f.email, f.msr, f.g_point, f.t_time," .
			" f.pay_type, f.delivery, f.souryou, f.add_num" .
			" from add f, sells g where g.sells_num='$sells_num' AND  f.add_num=g.add_num;";
	$sql1 = pg_exec($conn_id,$sql);

	list($name_s, $name_n, $kana_s, $kana_n, $zip1, $zip2, $prf, $city, $add1, $add2, $tel1, $tel2, $tel3, $fax1, $fax2, $fax3, $email, $msr, $g_point, $t_time, $pay_type, $delivery, $souryou, $add_num)	= pg_fetch_array($sql1,0);

	$year = substr($sells_num, 0, 2);
	$year = $year + 2000;
	$mon  = substr($sells_num, 2, 2);
	$day  = substr($sells_num, 4, 2);

	if (defined("NEWTAXDAY") && NEWTAXDAY <= $sells_num) {
		$tax_msg = "<font color=\"#ff0000\">(税込み表示)</font>";
	}

	echo <<<OZZYS
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='left' colspan="10">注文番号 : $sells_num 　　　注文日 : $year 年 $mon 月 $day 日 {$tax_msg}</TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>商品番号</TD>
      <TD align='center'>商品名</TD>
      <TD align='center'>メーカー名</TD>
      <TD align='center'>色</TD>
      <TD align='center'>サイズ</TD>
      <TD align='center'>単価</TD>
      <TD align='center'>購入数</TD>
      <TD align='center'>小計金額</TD>
      <TD align='center'>納品状態</TD>
      <TD align='center'>切り替え</TD>
    </TR>
OZZYS;

	$sql  = "select g.list_num, d.goods_name, d.color, d.size, g.price, g.buy_n, g.send, g.h_time,";
	$sql .= " b.maker_name, d.class_m, g.tax";
	$sql .= " from maker b, list d, sells g";
	$sql .= " where g.sells_num='$sells_num' AND d.list_num=g.list_num AND b.maker_num=d.maker_num";
	$sql .= " ORDER BY list_num;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	$price_all = 0;
	$point_get = 0;
	$t_sou = 0;
	$list_num_r = "";
	for($i=0; $i<$count; $i++) {
		list($list_num, $goods_name, $color, $size, $price, $buy_n, $send, $h_time, $maker_name,$class_m,$tax) = pg_fetch_array($sql1,$i);
		if ($list_num == $list_num_r) { continue; }

		if ($send == 0) { $point_get = 1; }

		if (defined("NEWTAXDAY") && NEWTAXDAY <= $sells_num) {
			if ($send == 0 || $send == 1) {
				$tanka = $price + floor(($price * $tax) + 0.5);
				$price_a = $tanka * $buy_n;
			} else {
				$price_a = 0;
			}
		} else {
			if ($send == 0 || $send == 1) {
				$tanka = $price;
				$price_a = $tanka * $buy_n;
			} else {
				$price_a = 0;
			}
		}

			echo <<< OZZYS

    <TR bgcolor="#FFFFFF">
      <TD align='center'>$list_num</TD>
      <TD align='center'>$goods_name</TD>
      <TD align='center'>$maker_name</TD>
      <TD align='center'>$color</TD>
      <TD align='center'>$size</TD>
      <TD align='right'>$tanka 円</TD>
      <TD align='center'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="goods" value="$goods">
       <INPUT type="hidden" name="goods_r" value="$goods">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="mode" value="henkou">
       <INPUT type="text" size="4" name="b_nums" value="$buy_n">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="list_num" value="$list_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type="submit" value="変更">
      </TD></FORM>
      <TD align='right'>$price_a 円</TD>
      <TD align='center'>$HASOU[$send]</TD>
      <TD align='center'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="goods" value="$goods">
       <INPUT type="hidden" name="goods_r" value="$goods">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="hen">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="list_num" value="$list_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type='submit' name='type' value='発送'> <INPUT type='submit' name='type' value='キャンセル'> <INPUT type='submit' name='type' value='未発送'>
      </TD></FORM>
    </TR>


OZZYS;

			$price_all += $price_a;

			foreach ($T_SOU_L AS $val) {
				if ($val == $class_m) { $t_sou = 1; }
			}

			$list_num_r = $list_num;

		}

	$p_price_all = $price_all;

	if (defined("NEWTAXDAY") && NEWTAXDAY > $sells_num) {
		$syou = $price_all * $tax;
		$syou = ceil($syou);
		$price_all += $syou;

		echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>消費税</TD>
      <TD align='right'>$syou 円</TD>
      <TD align='center' colspan="2">&nbsp;</TD>
    </TR>
OZZYS;
	}

	if (!$souryou) {
		$sou = $UN_L[$prf];
	}
	else {
		$sou = $souryou;
	}

	if ($t_sou == 1) { $sou = $sou + $tui_sou; }
	$price_all = $price_all+$sou;

			echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>送料</TD>
      <TD align='right'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="goods" value="$goods">
       <INPUT type="hidden" name="goods_r" value="$goods">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="send_change">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type="hidden" name="add_num" value="$add_num">
       <INPUT type="text" size="4" name="sou" value="$sou"> 円
      </TD>
      <TD align='center' colspan="2">
       <INPUT type='submit' value='変更'>
      </TD></FORM>
    </TR>
OZZYS;

	if ($pay_type == 0) {
		$price_all = $price_all+$daibiki;
		echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>代引き手数料</TD>
      <TD align='right'>$daibiki 円</TD>
      <TD align='center' colspan="2">&nbsp;</TD>
    </TR>
OZZYS;
	}

	if ($g_point) {
		$waribiki = ($g_point/$point_c)*$point_b;
		$price_all = $price_all-$waribiki;

		echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>ポイント割引</TD>
      <TD align='right'><FONT color="#ff0000">-$waribiki</FONT> 円</TD>
      <TD align='center' colspan="2">&nbsp;</TD>
    </TR>
OZZYS;

	}

	if ($price_all < 0) { $price_all = 0; }

			echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>合計金額</TD>
      <TD align='right'>$price_all 円</TD>
      <TD align='center' colspan="2">&nbsp;</TD>
    </TR>
OZZYS;

			if ($point_get == 0) {
				if (defined("NEWTAXDAY") && NEWTAXDAY <= $sells_num) {
					$point_a = $point_a + floor(($point_a * $tax) + 0.5);
				}
				if ($g_point || $kojin_num > 600000 || $pay_type == 3) {
					$point = 0;
				} else {
					$point = floor($p_price_all / $point_a);
				}



			echo <<< OZZYS

    <TR bgcolor="#FFFFFF">
      <TD colspan="7" bgcolor="#cccccc" align='right'>ポイント</TD>
      <TD align='right'>
       <FORM action="$PHP_SELF" method="POST">
       <INPUT type="hidden" name="main" value="$main">
       <INPUT type="hidden" name="main_r" value="$main">
       <INPUT type="hidden" name="goods" value="$goods">
       <INPUT type="hidden" name="goods_r" value="$goods">
       <INPUT type="hidden" name="view" value="$view">
       <INPUT type="hidden" name="page" value="$page">
       <INPUT type="hidden" name="mode" value="add">
       <INPUT type="hidden" name="sells_num" value="$sells_num">
       <INPUT type="hidden" name="kojin_num" value="$kojin_num">
       <INPUT type="text" size="4" name="point" value="$point"> pt
      </TD>
      <TD align='center' colspan="2">
       <INPUT type='submit' value='追加'>
      </TD></FORM>
    </TR>
OZZYS;

				}

##	$msr = ereg_replace("\n","<BR>",$msr);
	$msr = preg_replace("/\n/","<BR>",$msr);


	echo <<< OZZYS
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
       $PRF_L[$prf] $city <BR>
       $add1 $add2 
      </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>電話番号</TD>
      <TD bgcolor="#ffffff">$tel1 - $tel2 - $tel3 </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>FAX番号</TD>
      <TD bgcolor="#ffffff">$fax1 - $fax2 - $fax3 </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>メールアドレス</TD>
      <TD bgcolor="#ffffff"><A href="mailto:$email">$email</A></TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>支払い方法</TD>
      <TD bgcolor="#ffffff"><B>$PAY_L[$pay_type]</B></TD>
    </TR>

OZZYS;

	if ($delivery == 1) {
		echo <<< OZZYS
    <TR bgcolor="#cccccc">
      <TD align='center'>配送方法</TD>
      <TD bgcolor="#ffffff"><B>メール便、利用可能なら利用する。</B></TD>
    </TR>

OZZYS;
	}

	echo <<< OZZYS
    <TR bgcolor="#cccccc">
      <TD align='center'>配達希望時間</TD>
      <TD bgcolor="#ffffff">$T_TIME_L[$t_time] </TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center'>メッセージ</TD>
      <TD bgcolor="#ffffff">$msr </TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<BR>
OZZYS;


echo <<< OZZYS
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='goods' value='$goods'>
<INPUT type='hidden' name='goods_r' value='$goods'>
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="mode" value="syou">
<INPUT type="hidden" name="kojin_num" value="$kojin_num">
<INPUT type="submit" value="戻る">

OZZYS;

	pg_close($conn_id);

}



function first_03() {
global $PHP_SELF,$main,$goods,$view,$mode,$kojin_num,$type,$sells_num,$list_num;
include "../../cone.inc";
include './array.inc';

	if ($type == "発送") { $send = "1"; $h_time = "now()"; }
	elseif ($type == "キャンセル") { $send = "2"; $h_time = "1000-01-01"; }
	elseif ($type == "未発送") { $send = "0"; $h_time = "1000-01-01"; }

	$sql = "update sells set send='$send', h_time='$h_time' where sells_num='$sells_num' AND list_num='$list_num';";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }

	pg_close($conn_id);

	first_02();

}



function first_04() {
global $PHP_SELF,$main,$goods,$view,$mode,$kojin_num,$type,$sells_num,$list_num,$point;
include "../../cone.inc";
include './array.inc';

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

	if (!$point) { $point = 0; }
	$sql =  "insert into points values (" .
			" '$point_num'," .
			" '$kojin_num'," .
			" '$sells_num'," .
			" '$point');";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	if ($count <= 0) { $ERROR[] = "登録できませんでした。"; }

	$sql  = "UPDATE sells SET".
			" state='1'".
			" WHERE sells_num='$sells_num';";
	$result = pg_exec($conn_id,$sql);
	$count = pg_numrows($result);
	if ($count <= 0) { $ERROR[] = "更新できませんでした。"; }

	pg_close($conn_id);

	first_01();

}



function first_05() {
global $PHP_SELF,$main,$goods,$view,$mode,$kojin_num,$type,$sells_num,$list_num,$add_num,$sou;
include "../../cone.inc";
include './array.inc';

	if ($add_num > 0) { 

		$sql = "update add set souryou='$sou' where add_num='$add_num';";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if ($count <= 0) { $ERROR[] = "変更できませんでした。"; }

	}

	pg_close($conn_id);

	first_02();

}
?>
