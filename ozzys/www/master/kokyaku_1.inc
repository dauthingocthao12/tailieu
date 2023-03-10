<?PHP
//	顧客情報管理	************************************************************************
//	顧客買い物履歴
function kokyaku_1() {
global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num;

	if ($view) {
		if ($mode == "") { first_00(); }
		elseif ($mode == "syou") { first_01(); }
	}

}



function first_00() {
global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$max,$view_s,$views,$view_e;
include './array.inc';
include "../../cone.inc";

	## $num_k_ = mb_convert_kana($num_k_,"n","EUC-JP");
	$num_k_ = mb_convert_kana($num_k_,"n","UTF-8");

	$count = 0;
	$where = "";
	if ($num_k_) {
		$where .= " AND e.kojin_num='$num_k_'";
	} elseif ($prf_ || $name_s_) {
		if ($prf_) {
			$where .= " AND e.prf='$prf_'";
		}
		if ($name_s_) {
			$where .= " AND e.name_s='$name_s_'";
		}
	}
	$sql  = "SELECT count(distinct e.kojin_num) AS count FROM kojin e, sells g".
			" WHERE e.kojin_num=g.kojin_num";
	$sql .= " AND e.saku='0'";
	$sql .= $where.";";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	if ($count < 1) {
		echo ("ご購入されたお客様はいません。<BR><BR>\n");
	} else {
		$views = $VIEW_NUM[$view];
		$page_all = ceil($count / $views);
		if (!$page) {
			$page = 1;
		} elseif ($page > $page_all) {
			$page = $page_all;
		}
		$offset = ($page - 1) * $views;
		$limit = $views;
		$display_start = $offset + 1;
		$display_end = $display_start + $views - 1;
		if ($display_end > $count) { $display_end = $count; }
		//	検索数
		$limit_num = " OFFSET {$offset} LIMIT {$limit}";

		//	旧システムに対応のため
		$max = $count - 1;
		$view_s = $views * ($page-1);
		$view_e = ($views * $page) - 1;
		if ($view_e > $max) { $view_e = $max; }

		echo <<<EOT
<FORM action='{$PHP_SELF}' method='POST'>
<INPUT type="hidden" name="mode" value="syou">
<INPUT type="hidden" name="main" value="{$main}">
<INPUT type="hidden" name="main_r" value="{$main}">
<INPUT type="hidden" name="kokyaku" value="{$kokyaku}">
<INPUT type="hidden" name="kokyaku_r" value="{$kokyaku}">
<INPUT type="hidden" name="prf_" value="{$prf_}">
<INPUT type="hidden" name="num_k_" value="{$num_k_}">
<INPUT type="hidden" name="name_s_" value="{$name_s_}">
<INPUT type="hidden" name="view" value="{$view}">
<INPUT type="hidden" name="page" value="{$page}">
詳細を見たいお客様の番号を押して下さい。<BR>
ご購入お客様{$count}人中 {$display_start} - {$display_end}人目 ({$page}/{$page_all} Page)<br>
<TABLE border="0" bgcolor="#999999">
    <TR bgcolor="#cccccc">
      <TD align='center'>番号</TD>
      <TD align='center'>名前</TD>
      <TD align='center'>住所</TD>
      <TD align='center'>メールアドレス</TD>
    </TR>

EOT;

		$sql  = "SELECT distinct e.kojin_num, e.name_s, e.name_n, e.prf, e.city, e.email FROM kojin e, sells g".
				" WHERE e.kojin_num=g.kojin_num";
		$sql .= " AND e.saku='0'";
		$sql .= $where." ORDER BY e.kojin_num".$limit_num.";";
		if ($result = pg_query($conn_id,$sql)) {
			WHILE ($list = pg_fetch_array($result)) {
				foreach($list AS $key => $val) {
					$$key = $val;
				}
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
		}


		echo <<<EOT
</TABLE>
</FORM>

EOT;

		if ($max > $view_e || $view_s >= $views) { next_p_k(); }

	}

	pg_close($conn_id);

}



function first_01() {
    global $PHP_SELF,$main,$kokyaku,$menber,$prf_,$num_k_,$name_s_,$view,$mode,$page,$kojin_num;
    include './array.inc';
    include "../../cone.inc";
    require_once "../sub/webcollect.class.inc";

	$sql = "select name_s, name_n, prf, city, email from kojin WHERE kojin_num='$kojin_num' ORDER BY name_s;";

	$sql1 = pg_exec($conn_id,$sql);
	list($name_s, $name_n, $prf, $city, $email) = pg_fetch_array($sql1,0);

	$sql = "select sells_num from sells where kojin_num='$kojin_num' GROUP BY sells_num ORDER BY sells_num DESC;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	$max = $count-1;

    echo <<<OZZYS
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
OZZYS;

	for($i=0; $i<=$max; $i++) {
		list($sells_num) = pg_fetch_array($sql1,$i);

		$sql2  = "SELECT DISTINCT g.list_num, d.goods_name, d.color, d.size, g.price, g.buy_n, g.send, g.h_time, b.maker_name, w.order_no, w.settle_result";
        $sql2 .= " FROM maker b, list d, sells g";
        $sql2 .= " LEFT JOIN webcollect w ON w.sells_num = g.sells_num AND w.add_num = g.add_num";
		$sql2 .= " WHERE g.sells_num='$sells_num' AND d.list_num=g.list_num AND b.maker_num=d.maker_num";
		$sql2 .= " ORDER BY list_num;";
		$sql2_ = pg_exec($conn_id,$sql2);
		$count2 = pg_numrows($sql2_);
		$max2 = $count2-1;

		$price_all = 0;
		for($ii=0; $ii<=$max2; $ii++) {
			list($list_num, $goods_name, $color, $size, $price, $buy_n, $send, $h_time, $maker_name, $order_no, $settle_result) = pg_fetch_array($sql2_,$ii);

			if ($ii == 0) {

				$year = substr($sells_num, 0, 2);
				$year = $year + 2000;
				$mon  = substr($sells_num, 2, 2);
				$day  = substr($sells_num, 4, 2);

                if($order_no) {
                    $payment = Webcollect::masterPaymentIndicator($settle_result);
                }
                else {
                    $payment = "その他";
                }

echo <<< OZZYS
<TABLE border="0" bgcolor="#999999">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD align='left' colspan="8">注文番号 : $sells_num 　　　注文日 : $year 年 $mon 月 $day 日 　　　支払い方法：<span style="background-color: white;">$payment</span></TD>
    </TR>
    <TR bgcolor="#cccccc">
      <TD align='center' width="60">商品番号</TD>
      <TD align='center' width="120">商品名</TD>
      <TD align='center' width="120">メーカー名</TD>
      <TD align='center' width="120">色</TD>
      <TD align='center' width="120">サイズ</TD>
      <TD align='center' width="60">購入数</TD>
      <TD align='center' width="60">小計金額</TD>
      <TD align='center' width="60">納品状態</TD>
    </TR>
OZZYS;

				}

	$price_a = $price * $buy_n;

echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD align='center' width="60">$list_num</TD>
      <TD align='center' width="120">$goods_name</TD>
      <TD align='center' width="120">$maker_name</TD>
      <TD align='center' width="120">$color</TD>
      <TD align='center' width="120">$size</TD>
      <TD align='center' width="60">$buy_n</TD>
      <TD align='right' width="60">$price_a 円</TD>
      <TD align='center' width="60">$HASOU[$send]</TD>
    </TR>
OZZYS;

	$price_all = $price_all+$price_a;

			}



echo <<< OZZYS
    <TR bgcolor="#FFFFFF">
      <TD align='center' colspan="6">合計金額</TD>
      <TD align='right'>$price_all 円</TD>
      <TD align='center'>&nbsp;</TD>
    </TR>
  </TBODY>
</TABLE>
<BR>
<BR>
OZZYS;

		}


echo <<< OZZYS
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main' value='$main'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='kokyaku' value='$kokyaku'>
<INPUT type='hidden' name='kokyaku_r' value='$kokyaku'>
<INPUT type='hidden' name='prf_' value='$prf_'>
<INPUT type="hidden" name="num_k_" value="$num_k_">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="submit" value="戻る">
OZZYS;

	pg_close($conn_id);

}

?>
