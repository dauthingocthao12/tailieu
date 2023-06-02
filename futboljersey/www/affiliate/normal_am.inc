<?PHP
//	デフォルト　アフィリエイト会員
function normal_am($af_num) {
global $PHP_SELF,$conn_id,$afuser_table,$appoint_table,$application_table,$set_point_mon,$change_point;

	//	総ポイント
	$sql =  "SELECT SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$all_point = $list['all_point'];
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day';";
//			" WHERE af_num='$af_num' AND state='1' AND send_day<'$order_day';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$dec_point = $list['dec_point'];
	}

	//	支払い情報(全て)
	$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$pay_point = $list['pay_point'];
	}

	//	獲得確定ポイント
	$point = $dec_point - $pay_point;
	if ($point < 1) { $point = 0; }

	//	獲得予定ポイント
	$indent_point = $all_point - $dec_point;

	$point_ = number_format($point);
	$indent_point_ = number_format($indent_point);

	$html = <<<WAKABA
<table width="750px">
<TR>
<TH class="cate2">今現在のアフィリエイトポイント</TH>
</TR>
<TR>
<TD>
<BLOCKQUOTE><BR>
確定ポイント：{$point_}pt<br>

<br>

予定ポイント：{$indent_point_}pt<br>
<br>
<form action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="p_check">
詳細をチェックしたい人は「詳細閲覧」ボタンを押して下さい。<br>
<input type="submit" value="詳細閲覧">
</form>
</BLOCKQUOTE>
</TD>
</TR>
<TR>
<th class="cate2">ポイント変換請求</th>
</TR>

WAKABA;

	if ($point >= $change_point) {
		$appli_msg = <<<WAKABA
確定ポイントが {$point_}pt あります。<br>
<form action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="p_appli">
変換する場合は、「変換する」ボタンを押し申請手続きをお願い致します。<br />
<input type="submit" value="変換する">
</form>

WAKABA;
	}
	else {
		$change_point_ = number_format($change_point);
		$appli_msg = <<<WAKABA
確定ポイント {$change_point_}ptに達していないため変換出来ません。<br>
<br>

WAKABA;
	}

	$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD>
            <BLOCKQUOTE><BR>
$appli_msg
<form action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="p_appli_change">
変換申請の変更や、変換状況・履歴をみたい場合は、「変更・履歴」ボタンを押してください。
<input type="submit" value="変更・履歴">
</form>
</BLOCKQUOTE>
</TD>
</TR>
<TR>
<TH class="cate2">アフィリエイト設定方法</TH>
</TR>
<TR>
<TD>
<table style="border:0px;">
    <tr>
      <td style="border:0px;">アフィリエイトページリンク作成可能なページの下方には、以下の画像の様にリンクが表示されます。</td>
    </tr>
    <tr>
      <td align="center" style="border:0px;"><img src="/affiliate/image/make_link.gif" width="590" height="24" border="0"></td>
    </tr>
    <tr>
      <td style="border:0px;">作成するのリンクを押し、表示されたページに従ってリンクデーターをご利用下さい。</td>
    </tr>
    <tr>
      <td style="border:0px;"><br>
      </td>
    </tr>
    <tr>
      <td style="border:0px;">画像リンク・テキストリンク・メールリンクと種類がございますのでご利用状況にあわせて選択しご利用下さい。</td>
    </tr>
    <tr>
      <td style="border:0px;">各、リンクの説明の以下の画像の部分に当たる部分をコピー＆ペーストしご利用下さい。</td>
    </tr>
    <tr>
      <td align="center" style="border:0px;"><img src="/affiliate/image/copy.gif" width="536" height="148" border="0"></td>
    </tr>
    <tr>
      <td align="right" style="border:0px;">※リンクのURLや画像のURLは変更しないようお願い致します。</td>
    </tr>
    <tr>
      <td style="border:0px;"><br>
      </td>
    </tr>
    <tr>
      <td style="border:0px;">ログインされております会員によって表示されるアドレスの一部が違いますので必ずご自身のアカウントでログインし作成して下さい。<br>
      (リンクURLの中の/i_xxxx/の部分のxxxxがあなたのIDであることをご確認して下さい。)
      </td>
    </tr>
</table>
<br>
</TD>
</TR>
</TABLE>
<p><a href="/affiliate.php?mode=kiyaku">アフィリエイト規約</a></p>

WAKABA;

	return $html;

}
?>
