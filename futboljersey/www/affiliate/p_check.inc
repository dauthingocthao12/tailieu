<?PHP
//	アフィリエイトポイント確認


//	デフォルト	ポイント詳細（年毎）
function pc_defaults($mode,$action,$method,$af_num) {
global $PHP_SELF,$conn_id,$afrefere_table,$afuser_table,$appoint_table,$application_table,$set_point_mon,$change_point;

	$first_year = $last_year = date("Y");

	//	クリック数
	$sql  = "SELECT YEAR(click_time) AS year, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num'" .
			" GROUP BY YEAR(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$click = $list['click'];
			$CLICK[$year] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT YEAR(order_day) AS year, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num'" .
			" GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$count = $list['count'];
			$COUNT[$year] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT YEAR(order_day) AS year, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$all_point = $list['all_point'];

			$ALL[$year] = $all_point;
			if ($first_year > $year) { $first_year = $year; }
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT YEAR(order_day) AS year, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day'" .
			" GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$dec_point = $list['dec_point'];
			$DEC[$year] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT YEAR(appli_day) AS year, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' GROUP BY YEAR(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$pay_point = $list['pay_point'];
			$PAY[$year] = $pay_point;
		}
	}

	$html = "ポイント詳細（年毎）";

	if ($ALL) {
		$html .= <<<WAKABA
ポイント獲得履歴
<table border="0" cellspacing="1" bgcolor="#666666" width="600">
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="mode" value="p_check">
<input type="hidden" name="action" value="mon">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>年</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

		for($year=$last_year; $year>=$first_year; $year--) {
			//	クリック数
			$click = $CLICK[$year];
			if ($click < 1) { $click = 0; }

			//	売上件数
			$count = $COUNT[$year];
			if ($count < 1) { $count = 0; }

			//	獲得確定ポイント
			$point = $DEC[$year];
			if ($point < 1) { $point = 0; }

			//	獲得予定ポイント
			$indent_point = $ALL[$year] - $DEC[$year];

			//	変換ポイント
			$pay_point = $PAY[$year];

			//	有効ポイント
			$now_point = $point - $pay_point;
			if ($now_point < 1) { $now_point = 0; }

			$click_ = number_format($click);
			$count_ = number_format($count);
			$point_ = number_format($point);
			$indent_point_ = number_format($indent_point);
			$pay_point_ = number_format($pay_point);
			$now_point_ = number_format($now_point);

			$a_click += $click;
			$a_count += $count;
			$a_indent_point += $indent_point;
			$a_point += $point;
			$a_pay_point += $pay_point;
			$a_now_point += $now_point;

			$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center"><input type="submit" name="year" value="{$year}">年</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
		}

		$a_click = number_format($a_click);
		$a_count = number_format($a_count);
		$a_indent_point = number_format($a_indent_point);
		$a_point = number_format($a_point);
		$a_pay_point = number_format($a_pay_point);
		$a_now_point = number_format($a_now_point);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</form>
</table>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td>
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
  </tbody>
</table>

WAKABA;

	}

	return $html;

}



//	デフォルト	ポイント詳細（月毎）
function pc_mon($mode,$action,$method,$af_num) {
global $PHP_SELF,$conn_id,$afrefere_table,$afuser_table,$appoint_table,$application_table,$set_point_mon,$change_point;

	$year = trim($_POST['year']);

	//	クリック数
	$sql  = "SELECT MONTH(click_time) AS mon, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num' AND YEAR(click_time)='$year'" .
			" GROUP BY MONTH(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$click = $list['click'];
			$CLICK[$mon] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT MONTH(order_day) AS mon, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$count = $list['count'];
			$COUNT[$mon] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT MONTH(order_day) AS mon, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$all_point = $list['all_point'];

			$ALL[$mon] = $all_point;
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT MONTH(order_day) AS mon, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1' AND YEAR(order_day)='$year'" .
			" AND send_day<'$order_day'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$dec_point = $list['dec_point'];

			$DEC[$mon] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT MONTH(appli_day) AS mon, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' AND YEAR(appli_day)='$year'" .
			" GROUP BY MONTH(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$pay_point = $list['pay_point'];

			$PAY[$mon] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(YEAR(order_day)) AS first_year FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$first_year = $list['first_year'];
	}
	if ($first_year < $year) {
        $back_year = $year - 1;
		$first = <<<WAKABA
      <td align="left">
       <form action="$PHP_SELF" method="POST">
       <input type="hidden" name="mode" value="p_check">
       <input type="hidden" name="action" value="mon">
       <input type="hidden" name="year" value="$back_year">
       <input type="submit" value="&lt;&lt;">
      </td>
      </form>

WAKABA;
	}

	$now_year = date("Y");
	if ($now_year > $year) {
		$next_year = $year + 1;
		$next = <<<WAKABA
      <td align="left">
       <form action="$PHP_SELF" method="POST">
       <input type="hidden" name="mode" value="p_check">
       <input type="hidden" name="action" value="mon">
       <input type="hidden" name="year" value="$next_year">
       <input type="submit" value="&gt;&gt;">
      </td>
      </form>

WAKABA;
	}

	$html = "ポイント詳細（月毎）<br />";

	$html .= <<<WAKABA
<table border="0">
  <tbody>
    <tr align="center">
{$first}
      <td>ポイント獲得履歴　{$year}年</td>
{$next}
    </tr>
  </tbody>
</table>

<table border="0" cellspacing="1" bgcolor="#666666" width="600">
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="mode" value="p_check">
<input type="hidden" name="action" value="day">
<input type="hidden" name="year" value="$year">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>月</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

	for($mon=1; $mon<=12; $mon++) {
		//	クリック数
		$click = $CLICK[$mon];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$mon];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$mon];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$mon] - $DEC[$mon];

		//	変換ポイント
		$pay_point = $PAY[$mon];

		//	有効ポイント
		$now_point = $point - $pay_point;
		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
		$a_now_point += $now_point;

		$mon = sprintf('%02d',$mon);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center"><input type="submit" name="mon" class="btn-standard" value="{$mon}">月</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
	$a_now_point = number_format($a_now_point);

	$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</form>
</table>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td>
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
    <tr align="right">
      <td>
      <form action="$PHP_SELF" method="POST">
      <input type="hidden" name="mode" value="p_check">
      <input type="submit" value="年一覧に戻る">
      </td>
      </form>
    </tr>
  </tbody>
</table>

WAKABA;

	return $html;

}



//	デフォルト	ポイント詳細（日毎）
function pc_day($mode,$action,$method,$af_num) {
global $PHP_SELF,$conn_id,$afrefere_table,$afuser_table,$appoint_table,$application_table,$set_point_mon,$change_point;

	$year = trim($_POST['year']);
	$mon = trim($_POST['mon']);
	$dis_time = mktime(0,0,0,$mon,1,$year);

	//	クリック数
	$sql  = "SELECT DAY(click_time) AS day, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num' AND YEAR(click_time)='$year' AND MONTH(click_time)='$mon'" .
			" GROUP BY DAY(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$click = $list['click'];
			$CLICK[$day] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT DAY(order_day) AS day, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$count = $list['count'];
			$COUNT[$day] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT DAY(order_day) AS day, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$all_point = $list['all_point'];
			$ALL[$day] = $all_point;
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT DAY(order_day) AS day, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" AND send_day<'$order_day'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$dec_point = $list['dec_point'];
			$DEC[$day] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT DAY(appli_day) AS day, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2'" .
			" AND YEAR(appli_day)='$year' AND MONTH(appli_day)='$mon'" .
			" GROUP BY DAY(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$pay_point = $list['pay_point'];
			$PAY[$day] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(order_day) AS first_day FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$first_day = $list['first_day'];
		list($f_year,$f_mon,$f_day) = explode("-",$first_day);
		$first_time = mktime(0,0,0,$f_mon,1,$f_year);
	}

	if ($first_time < $dis_time) {
		$back_mon = $mon - 1;
		if ($back_mon < 1) {
			$back_mon = 12;
			$back_year = $year - 1;
		}
		else {
			$back_year = $year;
		}
		$first = <<<WAKABA
      <td align="left">
       <form action="$PHP_SELF" method="POST">
       <input type="hidden" name="mode" value="p_check">
       <input type="hidden" name="action" value="day">
       <input type="hidden" name="year" value="$back_year">
       <input type="hidden" name="mon" value="$back_mon">
       <input type="submit" value="&lt;&lt;">
      </td>
      </form>

WAKABA;
	}

	$now_time = mktime(0,0,0,date("m"),1,date("Y"));
	if ($now_time > $dis_time) {
		$next_mon = $mon + 1;
		if ($next_mon > 12) {
			$next_mon = 1;
			$next_year = $year + 1;
		}
		else {
			$next_year = $year;
		}
		$next = <<<WAKABA
      <td align="left">
       <form action="$PHP_SELF" method="POST">
       <input type="hidden" name="mode" value="p_check">
       <input type="hidden" name="action" value="day">
       <input type="hidden" name="year" value="$next_year">
       <input type="hidden" name="mon" value="$next_mon">
       <input type="submit" value="&gt;&gt;">
      </td>
      </form>

WAKABA;
	}

	$html = "ポイント詳細（日毎）<br />";

	$html .= <<<WAKABA
<table border="0">
  <tbody>
    <tr align="center">
{$first}
      <td>ポイント獲得履歴　{$year}年{$mon}月</td>
{$next}
    </tr>
  </tbody>
</table>

<table border="0" cellspacing="1" bgcolor="#666666" width="600">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>日</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

	$last_day = date("t",$dis_time);
	for($day=1; $day<=$last_day; $day++) {
		//	クリック数
		$click = $CLICK[$day];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$day];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$day];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$day] - $DEC[$day];

		//	変換ポイント
		$pay_point = $PAY[$day];

		//	有効ポイント
		$now_point = $point - $pay_point;
		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
		$a_now_point += $now_point;

		$day = sprintf('%02d',$day);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center">{$day}日</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
	$a_now_point = number_format($a_now_point);

	$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</table>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td colspan="2">
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
    <tr align="right">
      <td>
      <form action="$PHP_SELF" method="POST">
      <input type="hidden" name="mode" value="p_check">
      <input type="hidden" name="action" value="mon">
      <input type="hidden" name="year" value="$year">
      <input type="submit" value="{$year}年の月一覧に戻る">
      </td>
      </form>
      <td>
      <form action="$PHP_SELF" method="POST">
      <input type="hidden" name="mode" value="p_check">
      <input type="submit" value="年一覧に戻る">
      </td>
      </form>
    </tr>
  </tbody>
</table>

WAKABA;

	return $html;

}
?>
