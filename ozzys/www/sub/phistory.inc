<?PHP
//	ご注文履歴
function phistory() {

	$mode = $_POST['mode'];

	if ($mode == "syousai") { $html = syousai(); }
	else { $html = first(); }

	return $html;

}



//	初期画面
function first() {
global $PHP_SELF,$db;

	$idpass = $_SESSION['idpass'];
	list($email,$pass,$memo) = explode("<>",$idpass);

	$html = <<<WAKABA
	  <h2 class="sub-title-prod">ご注文履歴</h2>

WAKABA;

	$sql =  "SELECT g.sells_num, w.order_no, w.settle_result, w.settle_date FROM kojin e, sells g "
            . "LEFT JOIN webcollect w ON w.sells_num=g.sells_num AND w.add_num=g.add_num "
			. "WHERE e.saku!='1' AND e.email='$email' AND e.kojin_num=g.kojin_num "
            . "AND e.kojin_num BETWEEN 100001 AND 600000 "
			. "GROUP BY g.sells_num, w.order_no ORDER BY g.sells_num DESC;";
//    print $sql;
	if ($result = pg_query($db,$sql)) {
		$count = pg_numrows($result);
	}

	if ($count < 1) {
		$html .= <<<WAKABA
      <TABLE class="table-resp spaced">
        <TBODY>
          <TR bgcolor="#ffffff" align="center">
			<TD>
				<BR>
				ご注文履歴はありません。
				<BR>
			</TD>
          </TR>
        </TBODY>
      </TABLE>

WAKABA;
	}
	else {
		$html .= <<<WAKABA
      <TABLE class="table-resp spaced">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>ご注文内容の詳細を見たい場合は詳細ボタンを押して下さい。</B></TD>
          </TR>
        </TBODY>
      </TABLE>

      <p class="spaced">
        ※クレジットカード決済をご利用の場合で、未決済の場合は「未決済」アイコンが表示されます。<br>
        　詳細ページからお支払いページへ行くことができます。
      </p>

      <TABLE class="table-history clearfix">
        <TBODY>
          <TR class="table-history-headers">
            <TH width="27%">ご注文番号</TH>
            <TH width="27%">ご注文日</TH>
            <TH width="27%">商品状況</TH>
            <TH width="19%">詳細</TH>
          </TR>

WAKABA;

		while ($list = pg_fetch_array($result)) {
			$sells_num = $list['sells_num'];

			$year = substr($sells_num, 0, 2);
			$year = $year + 2000;
			$mon  = substr($sells_num, 2, 2);
			$day  = substr($sells_num, 4, 2);
			$hacyu = $year . "年 " . $mon . "月 " . $day . "日";

			$h = 0; $m = 0;
			$sql2 = "SELECT send FROM sells WHERE sells_num='$sells_num';";
			if ($result2 = pg_query($db,$sql2)) {
				while ($list2 = pg_fetch_array($result2)) {
					$send = $list2['send'];
					if ($send == 0) { $m = 1; }
					elseif ($send == 1) { $h = 1; }
				}
			}

            $canceled = false;
			$jyou = "";
			if ($h == 1 && $m == 0) { $jyou = "発送済み"; }
			elseif ($h == 1 && $m == 1) { $jyou = "一部発送済み"; }
			elseif ($h == 0 && $m == 1) { $jyou = "発送準備中"; }
			else { $jyou = "キャンセル"; $canceled = true;}

            // add simon 2019-02-22 webcollect order? >>>
            $order_no = $list['order_no'];
            if($list['order_no'] && !$canceled) {
                $hacyu .= "<br><small>カード払い";
                if($list['settle_result']==WEBCOLLECT_STATUS_PAID_ERR) {
                    $hacyu .= " <span class='webcollect-tag-error'>未決済</span>";
                }
                elseif($list['settle_result']==WEBCOLLECT_STATUS_UNPAID && !$canceled) {
                    $hacyu .= " <span class='webcollect-tag-warning'>未決済</span>";
                }
                $hacyu .= "</small>";
            }
            // <<<

			$html .= <<<WAKABA
          <TR bgcolor="#ffffff" align="center">
            <TD>$sells_num</TD>
            <TD>$hacyu</TD>
            <TD>$jyou</TD>
            <TD>
             <FORM action="$PHP_SELF" method="POST">
             <INPUT type="hidden" name="mode" value="syousai">
             <INPUT type="hidden" name="sells_num" value="$sells_num">
             <INPUT type="hidden" name="order_no" value="$order_no">
             <INPUT type="image" src="/images/syousai.gif" value="詳細">
            </TD></FORM>
          </TR>

WAKABA;

		}

		$html .= <<<WAKABA
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	}

	return $html;

}



//	ユーザー確認
function syousai() {
global $PHP_SELF,$db;

	$sells_num = $_POST['sells_num'];
	## $sells_num = mb_convert_kana($sells_num,"as","EUC-JP");
  $sells_num = mb_convert_kana($sells_num,"as","UTF-8");
	$sells_num = trim($sells_num);
	if (!$sells_num) { $ERROR[] = "商品番号が確認できません。"; }

    // webcollect
    if($_POST['order_no']) {
        $order = Webcollect::readOrder($_POST['order_no']);
        $order_add = Webcollect::readAdd($order['add_num']);
        //var_dump($order_add);
        $order_data = Webcollect::readSells($order['sells_num'], $order_add['kojin_num']);
        //var_dump($order_data);
        $cancelled = Webcollect::isCancelled($order['sells_num'], $order_add['kojin_num']);

        if($order['settle_result']==WEBCOLLECT_STATUS_PAID_OK && !$cancelled) {
            $date_formated = Webcollect::formatSettleDate($order['settle_date']);
            $order_msg = "<tr><td>";
            $order_msg .= "クレジットカード払い決済完了日時：".$date_formated;
            $order_msg .= "</td></tr>";
        }
        elseif($order['settle_result']==WEBCOLLECT_STATUS_PAID_ERR && !$cancelled) {
            $order_msg = "<tr><td>";
            $order_msg .= "クレジットカード払い：未決済<br>";
            $order_msg .= "お支払いを進める場合は、下記の「クレジット決済」ボタンを押し、<br>
            クレジットカードでのお支払い手続きを進めて下さい。<br>
            <br><a href='/endhtml/order_card.php?on={$order['order_no']}' class='btn btn-webcollect'>クレジット決済</a>";
            $order_msg .= "</td></tr>";
        }
        elseif($order['settle_result']==WEBCOLLECT_STATUS_UNPAID  && !$cancelled) {
            $order_msg = "<tr><td>";
            $order_msg .= "クレジットカード払い：未決済<br>";
            $order_msg .= "お支払いを進める場合は、下記の「クレジット決済」ボタンを押し、<br>
            クレジットカードでのお支払い手続きを進めて下さい。<br>
            <br><a href='/endhtml/order_card.php?on={$order['order_no']}' class='btn btn-webcollect'>クレジット決済</a>";
            $order_msg .= "</td></tr>";
        }
    }

	if (!$ERROR) {
		$year = substr($sells_num, 0, 2);
		$year = $year + 2000;
		$mon  = substr($sells_num, 2, 2);
		$day  = substr($sells_num, 4, 2);
		$hacyu = $year . "年 " . $mon . "月 " . $day . "日";

		$html = <<<WAKABA
      <h2 class="sub-title-prod">ご注文履歴</h2>

      <TABLE class="table-resp spaced">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>ご注文番号： $sells_num　ご注文日： $hacyu</TD>
          </TR>
          $order_msg
        </TBODY>
      </TABLE>

      <TABLE class="table-history spaced">
        <TBODY>
          <TR class="table-history-headers">
            <TH>商品番号</TH>
            <TH>商品名</TH>
            <TH>カラー</TH>
            <TH>サイズ</TH>
            <TH>注文数</TH>
            <TH>発送状態</TH>
          </TR>

WAKABA;

		$sql =  "SELECT d.list_num, d.goods_name, d.color, d.size, g.buy_n, g.send FROM list d, sells g" .
				" WHERE g.sells_num='$sells_num' AND d.list_num=g.list_num ORDER BY d.list_num;";
		if ($result = pg_query($db,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$list_num = $list['list_num'];
				$goods_name = $list['goods_name'];
				$color = $list['color'];
				$size = $list['size'];
				$buy_n = $list['buy_n'];
				$send = $list['send'];

				if (!$color) { $color = "--"; }
				if (!$size) { $size = "--"; }
				if ($send == 0) { $jyo = "準備中"; }
				if ($send == 1) { $jyo = "発送済み"; }
				if ($send == 2) { $jyo = "キャンセル"; }

				$html .= <<<WAKABA
          <TR align="center">
            <TD>
            	<span class="history-prod-label">商品番号</span>
            	<span class="history-prod-label-num">$list_num</span>
            </TD>
            <TD>
            	<span class="history-prod-label">商品名</span>$goods_name
            </TD>
            <TD>
            	<span class="history-prod-label">カラー</span>$color
            </TD>
            <TD>
            	<span class="history-prod-label">サイズ</span>$size
            </TD>
            <TD>
            	<span class="history-prod-label">注文数</span>$buy_n
            </TD>
            <TD>
            	<span class="history-prod-label">発送状態</span>$jyo
            </TD>
          </TR>

WAKABA;
			}
		}

		$html .= <<<WAKABA
        </TBODY>
      </TABLE>

  <center>
      <FORM action="$PHP_SELF" method="POST">
        <button type="submit" class="btn btn-submit">戻る</button>
      </FORM>
  </center>

WAKABA;

	}

	return $html;

}
?>
